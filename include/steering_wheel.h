/*
 * steering_wheel.h
 *
 *  Created on: 30 мая 2019 г.
 *      Author: Nivoter
 */

#ifndef INCLUDE_STEERING_WHEEL_H_
#define INCLUDE_STEERING_WHEEL_H_

#include <ch.h>
#include <hal.h>
#include <ADC.h>
#include <Odometry.h>
#define PWM1_P9 PAL_LINE( GPIOE, 9 )
#define PWM1_P11 PAL_LINE( GPIOE, 11 )
#define PWM1_MODE_P9 PAL_MODE_ALTERNATE(1)
#define PWM1_MODE_P11 PAL_MODE_ALTERNATE(1)
#define PWMDriver1 &PWMD1
#define PWM1_CHANNEL_P9 0
#define PWM1_CHANNEL_P11 1
#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
extern int16_t Curr;
extern bool almost_turned;
extern bool turn_enable;
extern bool turn_dir;
extern float CurrP;
extern int16_t Curr;
extern float kp;
extern float kd;
extern float ki;
extern int16_t turn_to;
extern bool turned;
extern float Dima_speed;
extern int16_t prev_err;
extern double I;
extern int16_t PID;
extern float P;
extern float D;
extern int8_t max_err;
extern int16_t now_err;


void lldMotorInit(void);
static THD_WORKING_AREA(waTURNING_WHEELS, 256);        // just for checking
static THD_FUNCTION(TURNING_WHEELS, arg);
int8_t TurnMotor(void);
void steerUnitCSSetPosition(int32_t position);
void lldSetPowerMotor(int8_t powerPrct);

#endif /* INCLUDE_STEERING_WHEEL_H_ */
