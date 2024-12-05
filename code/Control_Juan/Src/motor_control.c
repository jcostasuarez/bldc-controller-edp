#include "motor_control.h"
#include "pwm_config.h"

// Definición de la estructura para el timer
extern ADC_HandleTypeDef hadc1; // Configurado para medir corriente

float Kp = 1.0, Ki = 0.1; // Constantes del controlador PI
float integral = 0;
uint32_t MAX_CURRENT = 10; // Límite de corriente (en amperios)
uint32_t MAX_DUTY_CYCLE = 4199;
uint32_t MIN_DUTY_CYCLE = 0;

// Variables para medir la corriente del motor
volatile uint32_t current_measured = 0; // Corriente medida

// Variables para medir la velocidad del motor
volatile uint32_t speed_measured = 0;  // Velocidad medida

/** 
 * @brief Inicialización del motor
 * @param None
 * @retval None
 */
void Motor_Init(void) {
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

/** 
 * @brief Control de velocidad del motor
 * @param speed_setpoint Velocidad deseada en RPM
 * @retval None
 */
void Motor_ControlSpeed(uint32_t speed_setpoint) {
    // Medir la velocidad usando sensores Hall
    speed_measured = GetSpeedFromHall();

    // Controlador PI para velocidad
    float error = speed_setpoint - speed_measured;
    integral += error;
    float control_signal = Kp * error + Ki * integral;

    // Limitar el duty cycle
    if (control_signal > MAX_DUTY_CYCLE) control_signal = MAX_DUTY_CYCLE;
    if (control_signal < MIN_DUTY_CYCLE) control_signal = MIN_DUTY_CYCLE;

    // Medir corriente
    current_measured = ReadMotorCurrent();

    // Protección por corriente
    if (current_measured > MAX_CURRENT) {
        control_signal *= 0.9; // Reduce el duty cycle para limitar la corriente
    }

    // Aplicar el duty cycle ajustado
    PWM_SetDutyCycle(TIM_CHANNEL_1, (uint16_t)control_signal);
    PWM_SetDutyCycle(TIM_CHANNEL_2, (uint16_t)control_signal);
    PWM_SetDutyCycle(TIM_CHANNEL_3, (uint16_t)control_signal);
}

/** 
 * @brief Función para obtener la velocidad del motor a partir de sensores Hall
 * @param None
 * @retval Velocidad en RPM
 */
uint32_t GetSpeedFromHall(void) {
    // Lógica para calcular velocidad a partir de sensores Hall
    // (Usa temporizadores o interrupciones para medir tiempos entre transiciones)
    return calculated_speed;
}

/** 
 * @brief Función para leer la corriente del motor
 * @param None
 * @retval Corriente en amperios
 */
uint32_t ReadMotorCurrent(void) {
    // Leer corriente del ADC configurado
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1); // Valor en mA o mV, según la configuración
}
