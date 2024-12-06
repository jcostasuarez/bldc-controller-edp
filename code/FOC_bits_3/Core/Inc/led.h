#ifndef LED_H
#define LED_H

#include "stm32f4xx_hal.h"  // Asegúrate de incluir el archivo de cabecera adecuado para tu microcontrolador


#define ROJO 0
#define VERDE 1

// Prototipos de funciones

/**
 * Inicializar el LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_init(GPIO_TypeDef *port, uint16_t pin);

/**
 * Inicializar el LED verde
 */
void led_green_init(void);

/**
 * Inicializar el LED rojo
 */
void led_red_init(void);

/**
 * Encender el LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_on(GPIO_TypeDef *port, uint16_t pin);

/**
 * Apagar el LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_off(GPIO_TypeDef *port, uint16_t pin);

/**
 * Alternar el estado del LED
 * @param port Puerto del LED
 * @param pin Pin del LED
 */
void led_toggle(GPIO_TypeDef *port, uint16_t pin);

/**
 * Encender el LED verde
 */
void led_green_on(void);

/**
 * Apagar el LED verde
 */
void led_green_off(void);

/**
 * Encender el LED rojo
 */
void led_red_on(void);

/**
 * Apagar el LED rojo
 */
void led_red_off(void);

/**
 * Alternar el estado del LED verde
 */
void togle_green(void);

/**
 * Alternar el estado del LED rojo
 */
void togle_red(void);

#endif // LED_H