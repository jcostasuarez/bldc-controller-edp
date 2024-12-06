#include "drv8301.h"
#include "debug.h"

void drv8301_init(void) {

	uint16_t id = 0;

	// Enable Driver
	HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

	id = drv8301_read_reg(1);

	// Disable OC (OverCurrent protection)
	drv8301_write_reg(2, 0x0430);
	drv8301_write_reg(2, 0x0430);
}

/**
 * Write to DRV8301 register
 * @param reg Register to write to
 * @param data Data to write
 */
void drv8301_write_reg(int reg, int data) {

	HAL_StatusTypeDef status = HAL_OK;
	uint16_t out = 0;

	// Validación de parámetros
    if (reg < 0 || reg > 15) return;  // Dirección fuera de rango
    if (data < 0 || data > 0x07FF) return;  // Datos fuera de rango

	out |= (reg & 0x0F) << 11;
	out |= data & 0x7FF;

	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &out, 1, 10000);
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
}

uint16_t drv8301_read_reg(int reg) {
	uint16_t in = 0;
	uint16_t out = 0;

	out |= (1 << 15);
	out |= (reg & 0x0F) << 11;
	out |= 0x807F;

	if (reg != 0) {
		HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi3, (uint8_t*) &out, 1, 10000);
		HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
	}


	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Receive(&hspi3, (uint8_t *) &in, 1, 10000);
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);

	return in;
}
