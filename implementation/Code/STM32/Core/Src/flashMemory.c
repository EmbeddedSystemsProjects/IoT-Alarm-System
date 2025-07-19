#include "flashMemory.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Saves a 32-bit word to the specified Flash memory address.
 * @param data The 32-bit data to be written to Flash.
 * @param address The Flash memory address where the data will be stored.
 */
void SaveToFlash(uint32_t data, uint32_t address) {
	// Unlock the Flash memory for write access
	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef eraseInit;
	uint32_t SectorError = 0;

	// Configure the erase parameters for one sector
	eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	eraseInit.Sector = FLASH_SECTOR_6;
	eraseInit.NbSectors = 1;

	// Erase the specified Flash sector
	HAL_FLASHEx_Erase(&eraseInit, &SectorError);

	// Program the Flash memory with the specified data at the given address
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);

	// Lock the Flash memory to prevent further writes
	HAL_FLASH_Lock();
}

/**
 * @brief Reads a 32-bit unsigned integer from flash memory.
 * @return The 32-bit unsigned integer retrieved from flash memory.
 */
uint32_t ReadFromFlash(uint32_t address) {
	return *(uint32_t*) 0x08040000;
}
