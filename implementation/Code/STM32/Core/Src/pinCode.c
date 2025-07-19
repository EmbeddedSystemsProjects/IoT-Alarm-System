#include "pinCode.h"
#include "config.h"
#include "string.h"
#include "stdio.h"
#include "flashMemory.h"
#include "display.h"

extern void StopAlarm(void); // StopAlarm Function Declaration

/**
 * @brief Initialize the pincode system state.
 *
 * Resets the pincode buffer, counters, and system status.
 * This function must be called before using other pincode functionalities.
 *
 * @param state Pointer to the PincodeState structure.
 */
void Pincode_Init(PincodeState *state) {
	memset(state->entered_pincode, 0, sizeof(state->entered_pincode));
	state->pincode_position = 0;
	state->pin_attempts = 0;
	state->lockout_start = 0;
	state->system_locked = false;
	state->system_armed = false;
	state->message_sent = false;
	state->entering_pin_during_alarm = false;
}

/**
 * @brief Check the entered pincode against the stored pincode.
 *
 * Validates the entered pincode. If correct, toggles the system armed state.
 * If incorrect, increments the attempt counter and handles lockout if necessary.
 *
 * @param state Pointer to the PincodeState structure.
 */
void Pincode_Check(PincodeState *state) {
	uint32_t stored_pincode = ReadFromFlash(address1);
	char stored_pincode_str[PINCODE_LENGTH + 1];
	extern volatile uint8_t alarm_flag;

	// Convert stored pincode to string with zero-padding
	snprintf(stored_pincode_str, sizeof(stored_pincode_str), "%04lu",
			(unsigned long) stored_pincode); // Check if entered pincode matches the stored one
	if (strncmp(state->entered_pincode, stored_pincode_str, PINCODE_LENGTH)
			== 0) {
		state->pin_attempts = 0;

		// Clear entered pincode and reset position after success
		memset(state->entered_pincode, 0, sizeof(state->entered_pincode));
		state->pincode_position = 0;

		// If there is an alarm in progress or we are entering the PIN during an alarm,
		// we always disarm the system
		if (alarm_flag || state->entering_pin_during_alarm) {
			state->system_armed = false;
			state->entering_pin_during_alarm = false;

			// Debug output to verify that the system is actually disarmed
			printf(
					"Correct PIN during alarm: System DISARMED (system_armed = %d)\r\n",
					state->system_armed);// If there was an alarm going on, stop it
			if (alarm_flag) {
				printf("Stopping alarm...\r\n");
				StopAlarm();
			}
		} else {
			// In all other cases, we invert the system's armed state
			// (if disarmed we arm it, if armed we disarm it)
			state->system_armed = !state->system_armed;
			printf("Correct PIN: System %s (system_armed = %d)\r\n",
					state->system_armed ? "ARMED" : "DISARMED",
					state->system_armed);
		}

		// Show confirmation message
		Display_AccessGranted();
		// Always show the system status after checking the PIN
		printf("Updating display with status: system_armed = %d\r\n",
				state->system_armed);
		Display_SystemStatus(state->system_armed);

		// Reset message flag when system is disarmed
		if (!state->system_armed) {
			state->message_sent = false;
		}
	} else {
		state->pin_attempts++;

		// Check if max attempts exceeded to lock the system
		if (state->pin_attempts >= MAX_PIN_ATTEMPTS) {
			state->system_locked = true;
			state->lockout_start = HAL_GetTick();

			// Clear entered pincode and reset position on lockout
			memset(state->entered_pincode, 0, sizeof(state->entered_pincode));
			state->pincode_position = 0;

			Display_LockedScreen(LOCKOUT_TIME_MS / 1000);
		} else {
			Display_AccessDenied();

			// Clear entered pincode and reset position for next attempt
			memset(state->entered_pincode, 0, sizeof(state->entered_pincode));
			state->pincode_position = 0;// If we are entering the PIN during an alarm, return to the PIN entry screen
			Display_EnterPincode(state);
		}
	}
}
