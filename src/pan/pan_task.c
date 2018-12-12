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
#include "pan_task.h"
#include "xil_io.h"

//#define partstNUM_LEDS			( 1 )
//#define partstDIRECTION_OUTPUT	( 1 )
//#define partstOUTPUT_ENABLED	( 1 )
//#define partstLED_OUTPUT		( 10 )
//#define SWITCH_AXI_CHANNEL		( 1 )
#define PWM_BASE XPAR_AXI_TIMER_0_BASEADDR
#define PAN_FREQ_MS ( 10000 / portTICK_PERIOD_MS )

/*-----------------------------------------------------------*/

//static XGpio xGpio;
int state =0;

/*-----------------------------------------------------------*/

void vPanTaskInitialise( void )
{
	state =0;
/*XGpio_Config *pxConfigPtr;
BaseType_t xStatus;

	 Initialise the GPIO driver.
	pxConfigPtr = XGpio_LookupConfig( XPAR_GPIO_0_DEVICE_ID );
	xStatus = XGpio_CfgInitialize( &xGpio, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( xStatus == XST_SUCCESS );
	( void ) xStatus;  Remove compiler warning if configASSERT() is not defined.

	 Enable outputs and set low.
	XGpio_SetDataDirection( &xGpio, SWITCH_AXI_CHANNEL, 0x000f );*/
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

void pan_task( void *prvParameters)
{
   ( void ) prvParameters;
   
   TickType_t xNextWakeTime;
   xNextWakeTime = xTaskGetTickCount();
   
   for(;;)
   {
      vTaskDelayUntil( &xNextWakeTime, PAN_FREQ_MS );
      vPanTaskPWM();
   }
}


void vPanTaskPWM()
{
    //BaseType_t xSwitchState;
    uint32_t degrees = 0;
    switch (state)
    {
    	case 0:
	    	degrees = 0;
    	    state++;
        	break;
    	case 1:
	    	degrees = 30;
    	    state++;
        	break;
    	case 2:
	    	degrees = 60;
    	    state++;
        	break;
    	case 3:
	    	degrees = 90;
    	    state++;
        	break;
    	case 4:
	    	degrees = 120;
    	    state++;
        	break;
    	case 5:
	    	degrees = 150;
    	    state++;
        	break;
    	case 6:
	    	degrees = 180;
    	    state=0;
        	break;
    }
	//xSwitchState = XGpio_DiscreteRead( &xGpio, SWITCH_AXI_CHANNEL );

	/* Calculate Duty Cycle based off switch state */
//	degrees += (xSwitchState & 0x08 ) ? 90 : 0 ;
//	degrees += (xSwitchState & 0x04) ? 45 : 0;
//	degrees += (xSwitchState & 0x02) ? 22 : 0;
//	degrees += (xSwitchState & 0x01) ? 11 : 0;

	Xil_Out32(PWM_BASE + 0x14, (20e3 * (900+(10*degrees)) / 180) - 2); /* Write Duty Cycle uptime */
}



