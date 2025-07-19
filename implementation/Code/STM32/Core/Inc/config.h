#ifndef CONFIG_H
#define CONFIG_H

/* System Configuration */
#define PINCODE_LENGTH 4
#define MAX_PIN_ATTEMPTS 3
#define LOCKOUT_TIME_MS 30000
#define KEY_DEBOUNCE_MS 500
#define REED_DEBOUNCE_MS 200
#define MAIN_LOOP_PERIOD_MS 250

//ESP32 Communication related
#define MAX_CONNECTION_ATTEMPTS 3
#define DATA_SIZE 20
#define I2C_ADDR 0x5C
#define ESP32_EN GPIO_PIN_5	// Reset
#define ESP32_EN_Port GPIOB

/* Pin Definitions */
#define REED_SWITCH_PIN    GPIO_PIN_8
#define BUZZER_PIN         GPIO_PIN_10
#define BUTTON_PIN         GPIO_PIN_13

// Integrated LED
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

// ============================================
/*RC522 RFID Module Pin Configuration
 ============================================
 | RC522 Pin     | STM32 Pin     | Description             |
 |---------------|---------------|-------------------------|
 | VCC           | 3.3V          | Power supply (3.3V)     |
 | GND           | GND           | Ground                  |
 | RST           | PC2           | Reset pin               |
 | SDA/CS        | PD2           | Chip Select (SPI CS)    |
 | MOSI          | PC12          | Master Out Slave In     |
 | MISO          | PC11          | Master In Slave Out     |
 | SCK           | PC10          | SPI Clock               |*/
#define RC522_Rst_Pin GPIO_PIN_2
#define RC522_Rst_GPIO_Port GPIOC
#define RC522_CS_Pin GPIO_PIN_2
#define RC522_CS_GPIO_Port GPIOD

// PIR Sensor
#define PIR_PIN GPIO_PIN_7
#define PIR_PORT GPIOB

// Keypad Rows (PB15, PB1, PB2, PB10)
#define ROW1_PIN   GPIO_PIN_15
#define ROW2_PIN   GPIO_PIN_1
#define ROW3_PIN   GPIO_PIN_2
#define ROW4_PIN   GPIO_PIN_10
#define ROW_PORT   GPIOB

// Keypad Columns (PC6, PC7, PC4, PC5)
#define COL1_PIN   GPIO_PIN_6
#define COL2_PIN   GPIO_PIN_7
#define COL3_PIN   GPIO_PIN_4
#define COL4_PIN   GPIO_PIN_5
#define COL_PORT   GPIOC

// Keypad matrix definition
static const char keypad[4][4] = {
		{ '1', '2', '3', 'A' },
		{ '4', '5', '6', 'B' },
		{ '7', '8', '9', 'C' },
		{ '*', '0', '#', 'D' }
};

#endif /* CONFIG_H */
