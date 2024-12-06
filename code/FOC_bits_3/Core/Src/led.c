#include "led.h"
#include "stm32f4xx_hal.h"

/**
 * Inicializar el LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_init(GPIO_TypeDef *port, uint16_t pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

/**
 * Inicializar el LED verde
 */
void led_green_init(void) {
    led_init(GPIOB, GPIO_PIN_0);  // LED verde (PB0)
}

/**
 * Inicializar el LED rojo
 */
void led_red_init(void) {
    led_init(GPIOB, GPIO_PIN_1);  // LED rojo (PB1)
}

/**
 * Encender el LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_on(GPIO_TypeDef *port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

/**
 * Apagar el LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_off(GPIO_TypeDef *port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

/**
 * Alternar el estado del LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
// Alternar el estado del LED
void led_toggle(GPIO_TypeDef *port, uint16_t pin) {
    HAL_GPIO_TogglePin(port, pin);
}

void led_green_on(void) {
    led_on(GPIOB, GPIO_PIN_0);  // LED verde (PB0)
}

void led_green_off(void) {
    led_off(GPIOB, GPIO_PIN_0);  // LED verde (PB0)
}

void led_red_on(void) {
    led_on(GPIOB, GPIO_PIN_1);  // LED rojo (PB1)
}

void led_red_off(void) {
    led_off(GPIOB, GPIO_PIN_1);  // LED rojo (PB1)
}

void togle_green(void) {
    led_toggle(GPIOB, GPIO_PIN_0);  // LED verde (PB0)
}

void togle_red(void) {
    led_toggle(GPIOB, GPIO_PIN_1);  // LED rojo (PB1)
}
