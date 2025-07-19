#ifndef ESP32_H
#define ESP32_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// Constants and definitions
#define MAX_CONNECTION_ATTEMPTS 3
#define DATA_SIZE 20
#define I2C_ADDR 0x5C

// Global Variable Declarations
extern char device_ip[32];
extern bool wifi_connected;

// Functions
void esp32getIP(void);
bool is_valid_ip(const char *ip);

#endif // ESP32_H
