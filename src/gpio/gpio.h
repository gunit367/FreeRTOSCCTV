/*
 * gpio.h
 *
 *  Created on: Nov 26, 2018
 *      Author: Grant
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

void gpioInitialise( void );
void gpioSetLED( UBaseType_t uxLED, BaseType_t xValue );
void gpioToggleLED( unsigned portBASE_TYPE uxLED );


#endif /* SRC_GPIO_H_ */
