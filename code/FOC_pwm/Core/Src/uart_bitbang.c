#include "uart_bitbang.h"

// Command and value being transmitted
static int32_t g_value_tx = 0;
static uint8_t g_bit_pos_tx = 0;
static uint8_t g_bits_to_tx = 0;	// Each symbol is 10 bits: START | DATA[7:0] | STOP
static uint8_t g_tx_uart_busy = 0;

// Current byte being received
static uint8_t g_data_rx = 0;
static uint8_t g_bit_pos_rx = 0;
static uint8_t g_rx_uart_busy = 0;

static uint8_t g_circ_buffer_rx[CIRC_BUFFER_SIZE];
static uint8_t g_write_circ_pos = 0;
static uint8_t g_read_circ_pos = 0;


/**
 * @brief: Initialization function. For some reason, the first symbol transmitted is always
 * 	wrong, so the initialization consists of sending a mock value.
 */
void uart_init(void) {
	uart_write(DEVICE_ID, 4);
	HAL_Delay(100);
	uart_write(DEVICE_ID, 4);
}

/**
 * @brief: Returns "True" if there are values to be read in the UART
 */
uint8_t uart_is_empty(void) {
	return g_write_circ_pos == g_read_circ_pos;
}

/**
 * @brief: Return last received data
 */
uint8_t uart_read(void) {
	uint8_t data = g_circ_buffer_rx[g_read_circ_pos];
	if (g_read_circ_pos != g_write_circ_pos) {
		g_read_circ_pos++;
		if (g_read_circ_pos == CIRC_BUFFER_SIZE) {
			g_read_circ_pos = 0;
		}
	}
	return data;
}

/**
 * @brief: Write command and value through UART.
 */
void uart_write(int32_t value, uint8_t bytes) {
	while(g_tx_uart_busy) {
		HAL_Delay(1);
	}
	g_tx_uart_busy = 1;
	g_value_tx = value;
	g_bit_pos_tx = 0;
	g_bits_to_tx = bytes*10;
	HAL_TIM_Base_Start_IT(&htim5);
}


/**
 * @brief: Signal a "start" bit form UART, and disable IRQ until end bit.
 *  Starts a timer with frequency 9600 HZ to read the next 8 bits and stop bit.
 */
void uart_start_received(void) {
	if (!g_rx_uart_busy) {
		HAL_NVIC_DisableIRQ(EXTI2_IRQn);
		g_bit_pos_rx = 0;
		g_rx_uart_busy = 1;
		g_data_rx = 0;

		HAL_TIM_Base_Start_IT(&htim4);
	}

}

/**
 * @brief: Writes the next bit of the UART transaction
 */
void uart_write_next_bit(void) {
	// Data bit
	if (g_bit_pos_tx < g_bits_to_tx) {
		if (g_bit_pos_tx % 10 == 0) {
			// START BIT
			HAL_GPIO_WritePin(UART_TX_GPIO_Port, UART_TX_Pin, GPIO_PIN_RESET);
		} else if (g_bit_pos_tx % 10 == 9) {
			// STOP BIT
			HAL_GPIO_WritePin(UART_TX_GPIO_Port, UART_TX_Pin, GPIO_PIN_SET);
			g_value_tx = g_value_tx >> 8;
		} else {
			// DATA BIT
			uint8_t shift = (g_bit_pos_tx % 10) - 1;
			HAL_GPIO_WritePin(UART_TX_GPIO_Port, UART_TX_Pin, g_value_tx & (1 << shift) );
		}
		g_bit_pos_tx++;
	} else {
		g_bit_pos_tx = 0;
		g_tx_uart_busy = 0;
		HAL_TIM_Base_Stop_IT(&htim5);
	}
}

/**
 * @brief: Reads next bit from the UART
 */
void uart_read_next_bit(void) {
	uint8_t current_state = HAL_GPIO_ReadPin(UART_RX_GPIO_Port, UART_RX_Pin);

	if (g_bit_pos_rx < 8) {
		g_data_rx |= current_state << g_bit_pos_rx;
		g_bit_pos_rx++;
	} else {
		if (g_bit_pos_rx == 8 && current_state == 1) {
			// Stop bit correct
			g_circ_buffer_rx[g_write_circ_pos] = g_data_rx;
			g_write_circ_pos++;
			if (g_write_circ_pos == CIRC_BUFFER_SIZE) {
				g_write_circ_pos = 0;
			}
		}

		HAL_TIM_Base_Stop_IT(&htim4);
		g_rx_uart_busy = 0;
		HAL_NVIC_ClearPendingIRQ(EXTI2_IRQn);
		HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	}
}
