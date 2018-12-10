/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "xparameters.h"
#include "xil_printf.h"

#include "gpio/gpio.h"
#include "vdma.h"

#define mainVDMA_STATUS_PRIORITY			( tskIDLE_PRIORITY + 4 )
#define mainBLINK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_S			( 1000 / portTICK_PERIOD_MS )
#define mainBLINK_FREQUENCY					( 2000 / portTICK_PERIOD_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find it empty */
#define mainQUEUE_LENGTH					( 1 )
#define mainSTATUS_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )

/* The LED toggled by the Rx task. */
#define mainTASK_LED						( 0 )

#define VDMA_DEMO_VGA_WIDTH 640
#define VDMA_DEMO_VGA_HEIGHT 480
#define VDMA_DEMO_VGA_PIXEL_BYTES 4

static QueueHandle_t xQueue = NULL;
int w = 0;

vdma_handle handle;

void vdma_status_check_task(void *pvParameters)
{
	TickType_t xNextWakeTime;
	( void ) pvParameters;
	int i = 0;
	xNextWakeTime = xTaskGetTickCount();


	for( ;; )
	{
		vTaskDelayUntil(&xNextWakeTime, 2 * mainQUEUE_SEND_FREQUENCY_S);
		if (vdma_stopped(&handle))
		{
			xil_printf("ERROR! VDMA Stopped Unexpectedly.\n");
			vdma_dump_all_registers(&handle);
			break;
		}

		i = (i + 1) % 5;
		if (i == 0)
		{
			vdma_dump_all_registers(&handle);
		}
	}
}

static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const unsigned long ulValueToSend = 100UL;

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_S / (2*w) );

		/* Send to the queue - causing the queue receive task to unblock and
		toggle the LED.  0 is used as the block time so the sending operation
		will not block - it shouldn't need to block as the queue should always
		be empty at this point in the code. */
		xQueueSend( xQueue, &ulValueToSend, 0U );
	}
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;
const unsigned long ulExpectedValue = 100UL;

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	for( ;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/*  To get here something must have been received from the queue, but
		is it the expected value?  If it is, toggle the LED. */
		if( ulReceivedValue == ulExpectedValue )
		{
			gpioToggleLED( mainTASK_LED );
			ulReceivedValue = 0U;
		}
	}
}
/*-----------------------------------------------------------*/

static void prvQueueChangeBlink(void *pvParameters)
{
	TickType_t xNextWakeTime;
	( void ) pvParameters;
	xNextWakeTime = xTaskGetTickCount();
	for(;;)
	{
		w += 2;
		if (w > 18)
		{
			w = 2;
		}
		vTaskDelayUntil( &xNextWakeTime, mainBLINK_FREQUENCY );
	}

}

void main_vdma()
{
	xil_printf("------ Starting VDMA Demo ------\n");
	// Setup VDMA handle and memory-mapped ranges
	vdma_setup(&handle, VDMA_DEMO_VGA_WIDTH, VDMA_DEMO_VGA_HEIGHT, VDMA_DEMO_VGA_PIXEL_BYTES);
	vdma_dump_all_registers(&handle);

	// Start triple buffering
	xil_printf("-----Starting VDMA Buffering -----\n");
	vdma_start_triple_buffering(&handle);
	vdma_dump_all_registers(&handle);

	xil_printf("Running VDMA Buffering...\n");

	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

	if( xQueue != NULL )
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */
		xTaskCreate( prvQueueReceiveTask,				/* The function that implements the task. */
							"Rx", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
							configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
							NULL, 								/* The parameter passed to the task - not used in this case. */
							mainQUEUE_RECEIVE_TASK_PRIORITY, 	/* The priority assigned to the task. */
							NULL );								/* The task handle is not required, so NULL is passed. */

		xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );
		xTaskCreate( prvQueueChangeBlink, "CG", configMINIMAL_STACK_SIZE, NULL, mainBLINK_TASK_PRIORITY, NULL );
		xTaskCreate( vdma_status_check_task, "VDMA Status Check", configMINIMAL_STACK_SIZE, NULL, mainVDMA_STATUS_PRIORITY, NULL);
		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was either insufficient FreeRTOS heap memory available for the idle
	and/or timer tasks to be created, or vTaskStartScheduler() was called from
	User mode.  See the memory management section on the FreeRTOS web site for
	more details on the FreeRTOS heap http://www.freertos.org/a00111.html.  The
	mode from which main() is called is set in the C start up code and must be
	a privileged mode (not user mode). */
	for( ;; );

	xil_printf("\n\n-----Stopping VDMA Buffering -----\n");

	// Halt VDMA and unmap memory ranges
	vdma_halt(&handle);

	vdma_dump_all_registers(&handle);

	xil_printf("----- End of VDMA Demo -----\n");
}
