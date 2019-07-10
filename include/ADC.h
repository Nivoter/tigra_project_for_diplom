/*
 * ADC.h
 *
 *  Created on: 30 мая 2019 г.
 *      Author: Nivoter
 */

#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_

//#define ADC1_NUM_CHANNELS 2
//#define ADC1_BUF_DEPTH 1
#include <common.h>
#include <ch.h>
#include <hal.h>
#include <steering_wheel.h>
extern int16_t DinCur[2];
extern int16_t DinADC[2];

extern uint16_t value;
extern uint16_t current_value;
int16_t get_wheel_pos(void);
int16_t get_current(void);
void initADC(void);


#endif /* INCLUDE_ADC_H_ */
