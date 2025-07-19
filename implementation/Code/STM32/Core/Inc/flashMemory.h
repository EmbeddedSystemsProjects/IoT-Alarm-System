#ifndef FLASH_MEMORY_H
#define FLASH_MEMORY_H
#include <stdint.h>

#define address1 0x08040000 //Address to save the password of user 1
#define address2 0x08040040 //Address to save the password of user 2

/**
 * @brief Saves a 32-bit unsigned integer to flash memory.
 *
 * This function erases a specific sector of the STM32's internal flash
 * and writes a 32-bit value at a fixed address.
 *
 * **Warning:** This will erase the entire sector (sector 6), so any other data
 * stored there will be lost. Flash programming is forever (well, mostly).
 *
 * @param data The 32-bit unsigned integer to be stored in flash memory.
 */
void SaveToFlash(uint32_t data, uint32_t address);

/**
 * @brief Reads a 32-bit unsigned integer from flash memory.
 *
 * This function reads the previously saved 32-bit value from a fixed flash memory address.
 *
 * @return The 32-bit unsigned integer retrieved from flash memory.
 */
uint32_t ReadFromFlash(uint32_t address);
#endif // FLASH_MEMORY_H
