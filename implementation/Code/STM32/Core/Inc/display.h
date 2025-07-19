#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm32f4xx_hal.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "config.h"
#include <stdbool.h>
#include <pincode.h>

/* Display Functions */
void Display_Init(void);
void Display_BootMessage(void);
void Display_SystemStatus(bool system_armed);
void Display_LockedScreen(uint32_t remaining_seconds);
void Display_EnterPincode(const PincodeState *state);
void Display_AccessGranted(void);
void Display_AccessDenied(void);
void Display_AlarmScreen(bool toggle);
void Display_Clear(void);
void Display_Update(void);
void Display_IPAddress(const char *ip);
void Display_I2CScanResult(uint8_t address, uint8_t position);

#endif /* DISPLAY_H */
