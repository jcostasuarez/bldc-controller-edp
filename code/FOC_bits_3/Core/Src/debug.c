#include "debug.h"
#include "led.h"
#include "stm32f4xx_hal.h"

typedef struct {
    int times;              // Número total de parpadeos
    int current_blink;      // Parpadeos realizados
    uint32_t last_tick;     // Última marca de tiempo
    int state;              // Estado actual (0 = inicial, 1 = encendido, 2 = apagado)
} DebugBlinkState;

// Variable estática, privada a debug.c
static DebugBlinkState debug_blink_state = {0, 0, 0, 0};

// Implementación de las funciones
void debug_blink(int times) {
    debug_blink_state.times = times;
    debug_blink_state.current_blink = 0;
    debug_blink_state.last_tick = HAL_GetTick();
    debug_blink_state.state = 0;
}

void debug_blink_update(void) {
    uint32_t current_tick = HAL_GetTick();

    switch (debug_blink_state.state) {
        case 0: // Estado inicial
            if (debug_blink_state.current_blink < debug_blink_state.times) {
                led_red_on();  // Enciende el LED rojo
                debug_blink_state.last_tick = current_tick;
                debug_blink_state.state = 1;
            }
            break;
        case 1: // LED encendido
            if (current_tick - debug_blink_state.last_tick >= 100) {
                led_red_off();  // Apaga el LED rojo
                debug_blink_state.last_tick = current_tick;
                debug_blink_state.state = 2;
            }
            break;
        case 2: // LED apagado
            if (current_tick - debug_blink_state.last_tick >= 100) {
                debug_blink_state.current_blink++;
                debug_blink_state.state = 0;
            }
            break;
    }
}