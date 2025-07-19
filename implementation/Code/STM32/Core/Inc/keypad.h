#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief Scans the keypad for a pressed key.
 * @return The character of the pressed key, or '\0' if no key is pressed.
 */
char scan_keypad(void);

/**
 * @brief Handles the action associated with a pressed key.
 * @param key The character of the pressed key.
 */
void handle_keypress(char key);

#endif // KEYPAD_H
