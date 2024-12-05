#include "motor_control.h"
#include "pwm_config.h"


int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init();
    Motor_Init();

    uint32_t speed_setpoint = 500; // Velocidad deseada (RPM)

    while (1) {
        Motor_ControlSpeed(speed_setpoint);
    }
}


