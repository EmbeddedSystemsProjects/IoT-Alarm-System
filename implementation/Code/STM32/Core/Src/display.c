#include "display.h"
#include <stdio.h>

/* External I2C handle for communication */
extern I2C_HandleTypeDef hi2c1;

/**
 * @brief Initialize the OLED display.
 */
void Display_Init(void) {
	ssd1306_Init();
}

/**
 * @brief Display boot message sequence on the screen.
 * Shows booting progress and readiness.
 */
void Display_BootMessage(void) {
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Smart Alarm System", Font_7x10, White);

	ssd1306_SetCursor(30, 30);
	ssd1306_WriteString("Booting up...", Font_6x8, White);
	Display_Update();
	HAL_Delay(1500);

	ssd1306_Fill(White);
	ssd1306_SetCursor(30, 26);
	ssd1306_WriteString("System Ready", Font_7x10, Black);
	Display_Update();
	HAL_Delay(1000);

	Display_Clear();
}

/**
 * @brief Display the system's current status (armed/disarmed) and user options.
 * @param system_armed True if system is armed, false if disarmed.
 */
void Display_SystemStatus(bool system_armed) {
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("System Status:", Font_7x10, White);
	ssd1306_SetCursor(10, 25);	if (system_armed) {
		ssd1306_WriteString("ARMED", Font_7x10, White);
		ssd1306_SetCursor(10, 50);
		ssd1306_WriteString("Press B to disarm", Font_6x8, White);
	} else {
		ssd1306_WriteString("DISARMED", Font_7x10, White);
		ssd1306_SetCursor(10, 50);
		ssd1306_WriteString("Press A to arm", Font_6x8, White);
	}

	Display_Update();
}

/**
 * @brief Display a lockout screen with remaining time after failed PIN attempts.
 * @param remaining_seconds Number of seconds left before unlock.
 */
void Display_LockedScreen(uint32_t remaining_seconds) {
	char msg[30];

	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("SYSTEM LOCKED", Font_7x10, White);

	sprintf(msg, "Wait %lu seconds", remaining_seconds);
	ssd1306_SetCursor(10, 30);
	ssd1306_WriteString(msg, Font_7x10, White);

	Display_Update();
}

/**
 * @brief Display the PIN code entry screen using the PincodeState structure.
 *        Shows masked digits entered by the user and provides user instructions.
 *
 * @param state Pointer to the current PIN code state.
 */
void Display_EnterPincode(const PincodeState *state) {
	char display[PINCODE_LENGTH + 1] = { 0 };

	// Fill the screen with black background
	ssd1306_Fill(Black);

	// Display the header text
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Enter PINCODE:", Font_7x10, White);

	// Display user instruction
	ssd1306_SetCursor(10, 50);
	ssd1306_WriteString("Press D to Enter", Font_6x8, White);

	// Fill the display buffer with asterisks for each entered digit
	for (uint8_t i = 0; i < state->pincode_position; i++) {
		display[i] = '*';
	}

	// Display the masked pincode in the center area
	ssd1306_SetCursor(50, 30);
	ssd1306_WriteString(display, Font_11x18, White);

	// Update the display to show changes
	Display_Update();
}

/**
 * @brief Display access granted screen.
 */
void Display_AccessGranted(void) {
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 20);
	ssd1306_WriteString("ACCESS", Font_11x18, White);
	ssd1306_SetCursor(30, 40);
	ssd1306_WriteString("GRANTED", Font_11x18, White);
	Display_Update();
	HAL_Delay(2000);
}

/**
 * @brief Display access denied screen.
 */
void Display_AccessDenied(void) {
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 20);
	ssd1306_WriteString("ACCESS", Font_11x18, White);
	ssd1306_SetCursor(30, 40);
	ssd1306_WriteString("DENIED", Font_11x18, White);
	Display_Update();
	HAL_Delay(2000);
}

/**
 * @brief Display the alarm screen with a blinking effect.
 * @param toggle If true, display black background with white text, otherwise inverted.
 */
void Display_AlarmScreen(bool toggle) {
	ssd1306_Fill(toggle ? Black : White);
	ssd1306_SetCursor(35, 26);
	ssd1306_WriteString("ALARM!", Font_7x10, toggle ? White : Black);
	Display_Update();
}

/**
 * @brief Display the device IP address on the screen.
 * @param ip String containing the IP address.
 */
void Display_IPAddress(const char *ip) {
	ssd1306_SetCursor(0, 40);
	ssd1306_WriteString("IP:", Font_7x10, White);
	ssd1306_SetCursor(30, 40);
	ssd1306_WriteString(ip, Font_6x8, White);
	Display_Update();
	HAL_Delay(1000);
}

/**
 * @brief Display the result of an I2C address scan.
 * @param address I2C device address found.
 * @param position Line position on the screen to display the result.
 */
void Display_I2CScanResult(uint8_t address, uint8_t position) {
	char buffer[16];
	sprintf(buffer, "Found: 0x%02X", address);
	ssd1306_SetCursor(0, 16 + position * 10);
	ssd1306_WriteString(buffer, Font_6x8, White);
	Display_Update();
	HAL_Delay(1000);
}

/**
 * @brief Clear the display (fill with black).
 */
void Display_Clear(void) {
	ssd1306_Fill(Black);
}

/**
 * @brief Update the display buffer to the screen.
 */
void Display_Update(void) {
	ssd1306_UpdateScreen();
}
