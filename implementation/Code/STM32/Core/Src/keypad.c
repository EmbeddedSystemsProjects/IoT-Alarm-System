#include "keypad.h"
#include "config.h"
#include "display.h"
#include "pinCode.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// global variables
extern char last_key;
extern uint32_t last_key_time;
extern PincodeState pincodeState;
extern volatile uint8_t alarm_flag;

/**
 * @brief Scans the keypad for a pressed key.
 * @return The character of the pressed key, or '\0' if no key is pressed.
 */
char scan_keypad(void) {
	char key = '\0';
	uint8_t row, col;

	for (row = 0; row < 4; row++) {
		HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN,
				GPIO_PIN_SET);
		switch (row) {
		case 0:
			HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_RESET);
			break;
		}

		HAL_Delay(1);

		if (HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET) {
			key = keypad[row][0];
		} else if (HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET) {
			key = keypad[row][1];
		} else if (HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET) {
			key = keypad[row][2];
		} else if (HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET) {
			key = keypad[row][3];
		}

		if (key != '\0') {
			if (key
					== last_key&& (HAL_GetTick() - last_key_time) < KEY_DEBOUNCE_MS) {
				return '\0';
			}
			last_key = key;
			last_key_time = HAL_GetTick();
			return key;
		}
	}

	return '\0';
}

/**
 * @brief Handles the action associated with a pressed key.
 * @param key The character of the pressed key.
 */
void handle_keypress(char key) {
	if (key == '\0')
		return;

	printf("Key pressed: %c\r\n", key);
	switch (key) {
	case 'A':
		if (!pincodeState.system_armed && !pincodeState.system_locked) {
			pincodeState.pincode_position = 0;
			memset(pincodeState.entered_pincode, 0,
					sizeof(pincodeState.entered_pincode));
			printf(
					"Premuto A: Mostrando schermata inserimento PIN per armare\r\n");
			Display_EnterPincode(&pincodeState);
		}
		break;
	case 'B':
		if (!pincodeState.system_locked) {
			// B è usato per disarmare, quindi deve essere mostrato solo se armato o in allarme
			if (pincodeState.system_armed || alarm_flag) {
				pincodeState.pincode_position = 0;
				memset(pincodeState.entered_pincode, 0,
						sizeof(pincodeState.entered_pincode));
				if (alarm_flag) {
					pincodeState.entering_pin_during_alarm = true;
					printf("Inserimento PIN durante allarme attivato\r\n");
				}
				Display_EnterPincode(&pincodeState);
			}
		}
		break;

	case 'C':
		pincodeState.pincode_position = 0;
		memset(pincodeState.entered_pincode, 0,
				sizeof(pincodeState.entered_pincode));
		Display_SystemStatus(pincodeState.system_armed);
		break;

	case 'D':
		if (pincodeState.pincode_position == PINCODE_LENGTH) {
			Pincode_Check(&pincodeState);
		}
		break;

	case '*':
		pincodeState.pincode_position = 0;
		memset(pincodeState.entered_pincode, 0,
				sizeof(pincodeState.entered_pincode));
		Display_EnterPincode(&pincodeState);
		break;

	case '#':
		break;

	default:
		if (pincodeState.pincode_position < PINCODE_LENGTH && isdigit(key)) {
			pincodeState.entered_pincode[pincodeState.pincode_position++] = key;
			Display_EnterPincode(&pincodeState);
		}
		break;
	}
}
