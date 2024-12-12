/*
 * motor_bldc.h
 *
 *  Created on: Nov 22, 2024
 *      Author: bruno
 */

#ifndef INC_MOTOR_BLDC_H_
#define INC_MOTOR_BLDC_H_

void motor_reset(void);
uint8_t motor_read_hall(uint8_t orientation);
void motor_rotate(uint8_t orientation);
uint8_t motor_detect_hall_orientation(void);
uint8_t motor_get_hall(void);
void motor_calculate_speed(void);
void motor_init(void);
void motor_on_off(uint8_t on);
uint8_t motor_get_hall(void);
void motor_set_speed(uint8_t desired_speed_rpm);
void motor_set_duty(uint32_t duty);
uint8_t motor_get_speed_rpm(void);
int32_t motor_get_duty(void);

#define HALL_STATE_A 0b101
#define HALL_STATE_B 0b100
#define HALL_STATE_C 0b110
#define HALL_STATE_D 0b010
#define HALL_STATE_E 0b011
#define HALL_STATE_F 0b001

#define MOTOR_POLES 46.0

#endif /* INC_MOTOR_BLDC_H_ */
