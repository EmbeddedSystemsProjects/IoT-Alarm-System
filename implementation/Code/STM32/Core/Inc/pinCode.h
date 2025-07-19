#ifndef PINCODE_H
#define PINCODE_H

#include <stdbool.h>
#include <stdint.h>
#include "config.h"

typedef struct {
	char entered_pincode[PINCODE_LENGTH + 1];
	uint8_t pincode_position;
	uint8_t pin_attempts;
	uint32_t lockout_start;
	bool system_locked;
	bool system_armed;
	bool message_sent;
	bool entering_pin_during_alarm;
} PincodeState;

void Pincode_Init(PincodeState *state);
void Pincode_Check(PincodeState *state);

#endif // PINCODE_H
