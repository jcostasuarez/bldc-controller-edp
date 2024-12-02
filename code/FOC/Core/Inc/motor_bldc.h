/*
 * motor_bldc.h
 *
 *  Created on: Nov 22, 2024
 *      Author: bruno
 */

#ifndef INC_MOTOR_BLDC_H_
#define INC_MOTOR_BLDC_H_

void UpdatePhase(uint8_t step);
uint8_t GetHallState(void);
void UpdatePhaseFromHall(void);

#endif /* INC_MOTOR_BLDC_H_ */
