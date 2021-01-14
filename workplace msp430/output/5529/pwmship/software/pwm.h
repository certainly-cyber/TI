/*
 * pwm.h
 *
 *  Created on: 2020Äê9ÔÂ13ÈÕ
 *      Author: certainly
 */

#ifndef SOFTWARE_PWM_H_
#define SOFTWARE_PWM_H_
void pwm_init();
void setcompare_motor(int pwm);
void setcompare_steer(int pwm);
extern int steerpwm;




#endif /* SOFTWARE_PWM_H_ */
