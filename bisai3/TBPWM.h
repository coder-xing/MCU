/*
 * TAPWM.h
 *
 *  Created on: 2018Äê11ÔÂ14ÈÕ
 *      Author: Jack
 */

#ifndef TBPWM_H_
#define TBPWM_H_
#include <msp430f169.h>

char TBPWM_init(char Clk, char Div, char Mode1, char Mode2);
void TBPWM_set_period(unsigned int period);
void TBPWM_set_duty(char Channel, unsigned int Duty);
void TBPWM_set_permill(char Channel, unsigned int Percent);
void TBPWM_set_period_duty(unsigned int Period, unsigned long dianya);




#endif /* TBPWM_H_ */
