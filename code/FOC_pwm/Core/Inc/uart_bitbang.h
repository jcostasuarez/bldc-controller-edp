#ifndef INC_UART_BITBANG_H_
#define INC_UART_BITBANG_H_

#include "main.h"

#define CIRC_BUFFER_SIZE 50
#define DEVICE_ID 0xaabbccdd

uint8_t uart_read(void);
void uart_write(int32_t value, uint8_t bytes);
void uart_start_received(void);
void uart_write_next_bit(void);
void uart_read_next_bit(void);
void uart_init(void);
uint8_t uart_is_empty(void);

#endif /* INC_UART_BITBANG_H_ */
