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

    // Debug: Check if ID is valid
    if (id == 0xFFFF || id == 0) {
        debug_blink(5, 500); // Blink 5 times with 500ms interval to indicate error
    }

    // Disable OC (OverCurrent protection)
    if (drv8301_write_reg(2, 0x0430) != HAL_OK) {
        debug_blink(3, 300); // Blink 3 times with 300ms interval to indicate write error
    }
}

/**
 * Write to DRV8301 register
 * @param reg Register to write to
 * @param data Data to write
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef drv8301_write_reg(int reg, int data) {
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t out = 0;

    // Validación de parámetros
    if (reg < 0 || reg > 15) {
        debug_blink(2, 200); // Blink 2 times with 200ms interval to indicate invalid register
        return HAL_ERROR;  // Registro fuera de rango
    }
    if (data < 0 || data > 0x07FF) {
        debug_blink(2, 200); // Blink 2 times with 200ms interval to indicate invalid data
        return HAL_ERROR;  // Datos fuera de rango
    }

    out |= (reg & 0x0F) << 11;
    out |= data & 0x07FF;

    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(&hspi3, (uint8_t*) &out, 1, 10000);
    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);

    if (status != HAL_OK) {
        debug_blink(4, 400); // Blink 4 times with 400ms interval to indicate SPI error
    }

    return status;
}

/**
 * Read from DRV8301 register
 * @param reg Register to read from
 * @return Register value or 0xFFFF if error
 */
uint16_t drv8301_read_reg(int reg) {
    uint16_t in = 0;
    uint16_t out = 0;
    HAL_StatusTypeDef status;

    // Validación de parámetros
    if (reg < 0 || reg > 15) {
        debug_blink(2, 200); // Blink 2 times with 200ms interval to indicate invalid register
        return 0xFFFF; // Registro fuera de rango
    }

    out |= (1 << 15); // Indicar operación de lectura
    out |= (reg & 0x0F) << 11;

    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(&hspi3, (uint8_t*) &out, 1, 10000);
    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);

    if (status != HAL_OK) {
        debug_blink(4, 400); // Blink 4 times with 400ms interval to indicate SPI error
        return 0xFFFF;
    }

    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Receive(&hspi3, (uint8_t*)&in, 1, 10000);
    HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);

    if (status != HAL_OK) {
        debug_blink(4, 400); // Blink 4 times with 400ms interval to indicate SPI error
        return 0xFFFF;
    }

    // Debug: Check if received data is valid
    if (in == 0xFFFF || in == 0) {
        debug_blink(3, 300); // Blink 3 times with 300ms interval to indicate invalid data
    }

    return in;
}
