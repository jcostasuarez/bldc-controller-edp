#include "main.h"

#ifndef BEDDER

	#include "drv8301.h"

	void drv8301_init(void) {

		uint16_t id = 0;
		uint16_t a = 0, b = 0, c=0, d=0;

		drv8301_reset();

		a = drv8301_read_reg(0);
		b = drv8301_read_reg(1);
		c = drv8301_read_reg(2);
		d = drv8301_read_reg(3);

		// Disable OC (OverCurrent protection)
		drv8301_write_reg(2, 0x0430);
		drv8301_write_reg(2, 0x0430);
	}

	/**
	 * @brief Reset Driver
	 */
	void drv8301_reset(void) {
		// Enable Driver
		uint8_t nFault = 0;
		nFault = HAL_GPIO_ReadPin(nFAULT_GPIO_Port, nFAULT_Pin);
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, !nFault);
		HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(EN_GATE_GPIO_Port, EN_GATE_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		nFault = HAL_GPIO_ReadPin(nFAULT_GPIO_Port, nFAULT_Pin);
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, !nFault);
	}

	void drv8301_write_reg(int reg, int data) {
		uint16_t out = 0;
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

#endif // BEDDER
