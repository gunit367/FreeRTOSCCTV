/*
 * gpio.c
 *
 *  Created on: Nov 26, 2018
 *      Author: Grant
 */


/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Simple IO routines to control the LEDs.
 * This file is called ParTest.c for historic reasons.  Originally it stood for
 * PARallel port TEST.
 *-----------------------------------------------------------*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "gpio.h"

/* Xilinx includes. */
#include "xgpio.h"

/* #include "switches.h" */

#define partstNUM_LEDS			( 1 )
#define partstDIRECTION_OUTPUT	( 0x0f )
#define partstOUTPUT_ENABLED	( 1 )
#define partstLED_OUTPUT		( 1 )

/*-----------------------------------------------------------*/

static XGpio xGpio;
BaseType_t LEDState = 0x0f;

/*-----------------------------------------------------------*/

void gpioInitialise( void )
{
XGpio_Config *pxConfigPtr;
BaseType_t xStatus;

	/* Initialise the GPIO driver. */
	pxConfigPtr = XGpio_LookupConfig( XPAR_XGPIOPS_0_DEVICE_ID );
	xStatus = XGpio_CfgInitialize( &xGpio, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */

	/* Enable outputs and set low. */
//	XGpioPs_SetDirectionPin( &xGpio, partstLED_OUTPUT, partstDIRECTION_OUTPUT );
	XGpio_SetDataDirection( &xGpio, partstLED_OUTPUT, ~partstDIRECTION_OUTPUT );
//	XGpioPs_SetOutputEnablePin( &xGpio, partstLED_OUTPUT, partstOUTPUT_ENABLED );

//	XGpioPs_WritePin( &xGpio, partstLED_OUTPUT, 0x0 );
	XGpio_DiscreteWrite( &xGpio, partstLED_OUTPUT, 0x0 );
}
/*-----------------------------------------------------------*/

void gpioSetLED( UBaseType_t uxLED, BaseType_t xValue )
{
	( void ) uxLED;
	XGpio_DiscreteWrite( &xGpio, partstLED_OUTPUT, xValue );
}
/*-----------------------------------------------------------*/

void gpioToggleLED( unsigned portBASE_TYPE uxLED )
{

	( void ) uxLED;

	LEDState = (LEDState ^ 0x0f) & 0x0f;
	XGpio_DiscreteWrite( &xGpio, partstLED_OUTPUT, LEDState );
}



