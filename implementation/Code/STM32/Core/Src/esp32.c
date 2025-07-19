#include "esp32.h"
#include "display.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char device_ip[32] = { 0 };
bool wifi_connected = false;

void esp32getIP(void) {
	// Attempt to retrieve the IP address from the ESP32
	for (int attempt = 1; attempt <= MAX_CONNECTION_ATTEMPTS; attempt++) {
		uint8_t send_byte = '1'; // Command to request IP
		HAL_StatusTypeDef tx_status = HAL_I2C_Master_Transmit(&hi2c1,
				(I2C_ADDR << 1), &send_byte, 1, HAL_MAX_DELAY);

		// Check if the I2C transmission was successful
		if (tx_status != HAL_OK) {
			printf("[ERROR] I2C Transmit failed on attempt %d\n", attempt);
			HAL_Delay(500); // Add delay between attempts
			continue;
		}

		HAL_Delay(100); // Longer delay before receiving data to ensure ESP32 is ready
		// Pulisci i buffer prima della ricezione
		uint8_t rx_raw[DATA_SIZE + 1] = { 0 }; // Buffer to store received data

		// Utilizza un timeout più lungo (500ms invece di HAL_MAX_DELAY)
		HAL_StatusTypeDef rx_status = HAL_I2C_Master_Receive(&hi2c1,
				(I2C_ADDR << 1), rx_raw, DATA_SIZE, 500);

		// Check if the I2C reception was successful
		if (rx_status == HAL_OK) {
			// Assicurati che i dati siano terminati da null
			rx_raw[DATA_SIZE] = '\0';

			char ip_str[DATA_SIZE + 1] = { 0 }; // Buffer for the IP string (null-terminated)

			// Stampa i dati grezzi ricevuti per debug
			printf("[DEBUG] Raw data received: ");
			for (int i = 0; i < DATA_SIZE; i++) {
				printf("%02X ", rx_raw[i]);
			}
			printf("\n");

			// Convert raw data to a printable string
			int valid_chars = 0;
			for (int i = 0; i < DATA_SIZE; i++) {
				if (rx_raw[i] == '\n' || rx_raw[i] == '\r'
						|| rx_raw[i] == '\0') {
					break; // Stop at newline, carriage return or null
				}
				if (isprint(rx_raw[i])) {
					ip_str[valid_chars++] = rx_raw[i];
				} else {
					break; // Stop at non-printable character
				}
			}
			ip_str[valid_chars] = '\0'; // Ensure null termination

			// Validate the received IP address
			if (is_valid_ip(ip_str)) {
				strncpy(device_ip, ip_str, sizeof(device_ip)); // Save the valid IP
				device_ip[sizeof(device_ip) - 1] = '\0'; // Ensure null termination

				printf("[OK] Received valid IP: %s\n", device_ip);
				Display_IPAddress(device_ip); // Update the display with the IP
				HAL_Delay(100);
				wifi_connected = true; // Mark Wi-Fi as connected
				break;
			} else {
				printf("[WARN] Invalid IP received: %s\n", ip_str);
			}
		} else {
			printf("[ERROR] No IP received on attempt %d\n", attempt);
		}
		// If no valid IP is received, display an error
		if (attempt == MAX_CONNECTION_ATTEMPTS) {
			strcpy(device_ip, "ERROR");
			Display_IPAddress(device_ip);
		}
		HAL_Delay(500); // Longer delay between attempts
	}
}

bool is_valid_ip(const char *ip) {
	// Addizionale: controlla se la stringa è vuota o troppo corta
	if (ip == NULL || strlen(ip) < 7) { // 7 = minimo per un IP valido (1.1.1.1)
		return false;
	}

	int num, dots = 0;
	char *ptr;
	char ip_copy[32];
	strncpy(ip_copy, ip, sizeof(ip_copy));
	ip_copy[sizeof(ip_copy) - 1] = '\0';

	// Debug: stampa la stringa IP ricevuta
	printf("[DEBUG] Validating IP: '%s'\n", ip_copy);

	ptr = strtok(ip_copy, ".");
	if (ptr == NULL)
		return false;

	while (ptr) {
		// Verifica che tutti i caratteri siano cifre
		for (int i = 0; ptr[i] != '\0'; i++) {
			if (!isdigit(ptr[i]))
				return false;
		}

		num = atoi(ptr);
		if (num < 0 || num > 255)
			return false;

		ptr = strtok(NULL, ".");
		if (ptr != NULL)
			dots++;
	}

	return dots == 3;
}
