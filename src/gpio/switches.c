///*
// * switches.c
// *
// *  Created on: Oct 10, 2018
// *      Author: Grant
// */
//
///* Scheduler includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//
///* Demo includes. */
//#include "switches.h"
//
///* Xilinx includes. */
//#include "xgpio.h"
//
//#define switchesDIRECTION_INPUT	( 0x0f )
//#define switchesOUTPUT_ENABLED	( 1 )
//#define switchesAXI_CHANNEL		( 1 )
//
///*-----------------------------------------------------------*/
//
//static XGpio xGpioSwitch;
//
///*-----------------------------------------------------------*/
//
//void vSwitchesInitialise( void )
//{
//XGpio_Config *pxConfigPtr;
//BaseType_t xStatus;
//
//	/* Initialize the GPIO driver. */
//	pxConfigPtr = XGpio_LookupConfig( XPAR_GPIO_1_DEVICE_ID );
//	xStatus = XGpio_CfgInitialize( &xGpioSwitch, pxConfigPtr, pxConfigPtr->BaseAddress );
//	configASSERT( xStatus == XST_SUCCESS );
//	( void ) xStatus; /* Remove compiler warning if configASSERT() is not defined. */
//
//	/* Enable switches as inputs */
//	XGpio_SetDataDirection( &xGpioSwitch, switchesAXI_CHANNEL, switchesDIRECTION_INPUT );
//}
///*-----------------------------------------------------------*/
//
//uint32_t vSwitchesGet( void )
//{
//	return XGpio_DiscreteRead( &xGpioSwitch, switchesAXI_CHANNEL) & 0x0f;
//}
///*-----------------------------------------------------------*/
//
//
//
