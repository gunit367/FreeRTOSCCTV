/*
 * vdma.c - Contains code to help manage the VDMA engine
 *
 * This code is adapted from code originally authored by Lauri Vosandi,
 * the original version of the code can be found on his blog at
 * https://lauri.xn--vsandi-pxa.com/hdl/zynq/book.html
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "xparameters.h"
#include "xil_printf.h"

#include "vdma.h"

#define VDMA_DEMO_VGA_WIDTH 640
#define VDMA_DEMO_VGA_HEIGHT 480
#define VDMA_DEMO_VGA_PIXEL_BYTES 4

/*
 * Fill handle structure with VDMA management information
 * See above struct definition for what goes in each field.
 */
int vdma_setup(vdma_handle *handle, int width, int height, int pixelLength)
{
    handle->width=width;
    handle->height=height;
    handle->pixelLength=pixelLength;
    handle->fbLength=pixelLength*width*height;

    handle->control_registers = (unsigned int*) XPAR_AXIVDMA_0_BASEADDR;
    handle->fb1Addr = (char*)FRAME_BUFFER_1_ADDR;
    handle->fb2Addr = (char*)FRAME_BUFFER_2_ADDR;
    handle->fb3Addr = (char*)FRAME_BUFFER_3_ADDR;

    /* Init all frame buffers to white */
    memset(handle->fb1Addr, 255, (handle->width)*(handle->height)*(handle->pixelLength));
    memset(handle->fb2Addr, 255, (handle->width)*(handle->height)*(handle->pixelLength));
    memset(handle->fb3Addr, 255, (handle->width)*(handle->height)*(handle->pixelLength));
    return 0;
}

void vdma_set(vdma_handle *handle, int num, unsigned int val)
{
    handle->control_registers[num>>2]=val;
}

void vdma_halt(vdma_handle *handle)
{
    vdma_set(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER, VDMA_CONTROL_REGISTER_RESET);
    vdma_set(handle, OFFSET_VDMA_MM2S_CONTROL_REGISTER, VDMA_CONTROL_REGISTER_RESET);
}

unsigned int vdma_get(vdma_handle *handle, int num)
{
    return handle->control_registers[num>>2];
}

int vdma_stopped(vdma_handle *handle)
{
	unsigned int status;
	status = vdma_get(handle, OFFSET_VDMA_S2MM_STATUS_REGISTER);
	if (status & VDMA_STATUS_REGISTER_HALTED)
	{
		return 1;
	}

	status = vdma_get(handle, OFFSET_VDMA_MM2S_STATUS_REGISTER);
	if (status & VDMA_STATUS_REGISTER_HALTED)
	{
		return 1;
	}

	status = vdma_get(handle, OFFSET_VDMA_MM2S_CONTROL_REGISTER);
	if (!(status & VDMA_CONTROL_REGISTER_START))
	{
		return 1;
	}

	status = vdma_get(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER);
	if (!(status & VDMA_CONTROL_REGISTER_START))
	{
		return 1;
	}
	return 0;
}

void vdma_status_dump(int status)
{
    if (status & VDMA_STATUS_REGISTER_HALTED) xil_printf("\thalted\n"); else xil_printf("\trunning\n");
    if (status & VDMA_STATUS_REGISTER_VDMAInternalError) xil_printf("\tvdma-internal-error\n");
    if (status & VDMA_STATUS_REGISTER_VDMASlaveError) xil_printf("\tvdma-slave-error\n");
    if (status & VDMA_STATUS_REGISTER_VDMADecodeError) xil_printf("\tvdma-decode-error\n");
    if (status & VDMA_STATUS_REGISTER_StartOfFrameEarlyError) xil_printf("\tstart-of-frame-early-error\n");
    if (status & VDMA_STATUS_REGISTER_EndOfLineEarlyError) xil_printf("\tend-of-line-early-error\n");
    if (status & VDMA_STATUS_REGISTER_StartOfFrameLateError) xil_printf("\tstart-of-frame-late-error\n");
    if (status & VDMA_STATUS_REGISTER_FrameCountInterrupt) xil_printf("\tframe-count-interrupt\n");
    if (status & VDMA_STATUS_REGISTER_DelayCountInterrupt) xil_printf("\tdelay-count-interrupt\n");
    if (status & VDMA_STATUS_REGISTER_ErrorInterrupt) xil_printf("\terror-interrupt\n");
    if (status & VDMA_STATUS_REGISTER_EndOfLineLateError) xil_printf("\tend-of-line-late-error\n");
    xil_printf("\tframe-count:%d\n", (status & VDMA_STATUS_REGISTER_FrameCount) >> 16);
    xil_printf("\tdelay-count:%d\n", (status & VDMA_STATUS_REGISTER_DelayCount) >> 24);
    xil_printf("\n");
}

void vdma_s2mm_status_dump(vdma_handle *handle)
{
    int status = vdma_get(handle, OFFSET_VDMA_S2MM_STATUS_REGISTER);
    xil_printf("S2MM status register (%08x):\n", status);
    vdma_status_dump(status);
}

void vdma_mm2s_status_dump(vdma_handle *handle)
{
    int status = vdma_get(handle, OFFSET_VDMA_MM2S_STATUS_REGISTER);
    xil_printf("MM2S status register (%08x):\n", status);
    vdma_status_dump(status);
}

void vdma_control_dump(int status)
{
	if (status & VDMA_CONTROL_REGISTER_START) xil_printf("\tRunning\n"); else xil_printf("\tStopped\n");
	if (status & VDMA_CONTROL_REGISTER_RESET) xil_printf("\tReset in Progress\n"); else xil_printf("\tNormal Operation\n");

	xil_printf("\n");
}

void vdma_s2mm_control_dump(vdma_handle *handle)
{
	int status = vdma_get(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER);
	xil_printf("S2MM control register (%08x):\n", status);
	vdma_control_dump(status);
}

void vdma_mm2s_control_dump(vdma_handle *handle)
{
	int status = vdma_get(handle, OFFSET_VDMA_MM2S_CONTROL_REGISTER);
	xil_printf("S2MM control register (%08x):\n", status);
	vdma_control_dump(status);
}

void vdma_dump_all_registers(vdma_handle *handle)
{
	vdma_s2mm_status_dump(handle);
	vdma_mm2s_status_dump(handle);
	vdma_s2mm_control_dump(handle);
	vdma_mm2s_control_dump(handle);
}

void vdma_start_triple_buffering(vdma_handle *handle)
{
    // Reset VDMA
    vdma_set(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER, VDMA_CONTROL_REGISTER_RESET);
    vdma_set(handle, OFFSET_VDMA_MM2S_CONTROL_REGISTER, VDMA_CONTROL_REGISTER_RESET);

    // Wait for reset to finish
    while((vdma_get(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER) & VDMA_CONTROL_REGISTER_RESET)==4);
    while((vdma_get(handle, OFFSET_VDMA_MM2S_CONTROL_REGISTER) & VDMA_CONTROL_REGISTER_RESET)==4);

    // Clear all error bits in status register
    vdma_set(handle, OFFSET_VDMA_S2MM_STATUS_REGISTER, 0);
    vdma_set(handle, OFFSET_VDMA_MM2S_STATUS_REGISTER, 0);

    // Do not mask interrupts
    vdma_set(handle, OFFSET_VDMA_S2MM_IRQ_MASK, 0xf);

    int interrupt_frame_count = 3;

    // Start both S2MM and MM2S in triple buffering mode
    vdma_set(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER,
        (interrupt_frame_count << 16) |
        VDMA_CONTROL_REGISTER_START |
        VDMA_CONTROL_REGISTER_GENLOCK_ENABLE |
        VDMA_CONTROL_REGISTER_INTERNAL_GENLOCK |
        VDMA_CONTROL_REGISTER_CIRCULAR_PARK);
    vdma_set(handle, OFFSET_VDMA_MM2S_CONTROL_REGISTER,
        (interrupt_frame_count << 16) |
        VDMA_CONTROL_REGISTER_START |
        VDMA_CONTROL_REGISTER_GENLOCK_ENABLE |
        VDMA_CONTROL_REGISTER_INTERNAL_GENLOCK |
        VDMA_CONTROL_REGISTER_CIRCULAR_PARK);


    while((vdma_get(handle, OFFSET_VDMA_S2MM_CONTROL_REGISTER) & VDMA_CONTROL_REGISTER_START)==0 ||
    	  (vdma_get(handle, OFFSET_VDMA_S2MM_STATUS_REGISTER) & VDMA_STATUS_REGISTER_HALTED)==1) {
        xil_printf("Waiting for VDMA to start running...\n");
        sleep(1);
    }

    // Extra register index, use first 16 frame pointer registers
    vdma_set(handle, OFFSET_VDMA_S2MM_REG_INDEX, 0);

    // Write physical addresses to control register
    vdma_set(handle, OFFSET_VDMA_S2MM_FRAMEBUFFER1, (int)handle->fb1Addr);
    vdma_set(handle, OFFSET_VDMA_MM2S_FRAMEBUFFER1, (int)handle->fb1Addr);
    vdma_set(handle, OFFSET_VDMA_S2MM_FRAMEBUFFER2, (int)handle->fb2Addr);
    vdma_set(handle, OFFSET_VDMA_MM2S_FRAMEBUFFER2, (int)handle->fb2Addr);
    vdma_set(handle, OFFSET_VDMA_S2MM_FRAMEBUFFER3, (int)handle->fb3Addr);
    vdma_set(handle, OFFSET_VDMA_MM2S_FRAMEBUFFER3, (int)handle->fb3Addr);

    // Write Park pointer register
    vdma_set(handle, OFFSET_PARK_PTR_REG, 0);

    // Frame delay and stride (bytes)
    vdma_set(handle, OFFSET_VDMA_S2MM_FRMDLY_STRIDE, handle->width*handle->pixelLength);
    vdma_set(handle, OFFSET_VDMA_MM2S_FRMDLY_STRIDE, handle->width*handle->pixelLength);

    // Write horizontal size (bytes)
    vdma_set(handle, OFFSET_VDMA_S2MM_HSIZE, handle->width*handle->pixelLength);
    vdma_set(handle, OFFSET_VDMA_MM2S_HSIZE, handle->width*handle->pixelLength);

    // Write vertical size (lines), this actually starts the transfer
    vdma_set(handle, OFFSET_VDMA_S2MM_VSIZE, handle->height);
    vdma_set(handle, OFFSET_VDMA_MM2S_VSIZE, handle->height);
}



int vdma_demo()
{
	int i;
	vdma_handle handle;

	xil_printf("------ Starting VDMA Demo ------\n");
	// Setup VDMA handle and memory-mapped ranges
	vdma_setup(&handle, VDMA_DEMO_VGA_WIDTH, VDMA_DEMO_VGA_HEIGHT, VDMA_DEMO_VGA_PIXEL_BYTES);

	vdma_dump_all_registers(&handle);

	// Start triple buffering
	xil_printf("-----Starting VDMA Buffering -----\n");
	vdma_start_triple_buffering(&handle);

	vdma_dump_all_registers(&handle);

	xil_printf("Running VDMA Buffering...\n");
	// Run for 100 seconds, just monitor status registers
	for(i=0; i<100; i++)
	{
		sleep(1);
		if (vdma_stopped(&handle))
		{
			xil_printf("ERROR! VDMA Stopped Unexpectedly.\n");
			vdma_dump_all_registers(&handle);
			break;
		}
	}
	xil_printf("\n\n-----Stopping VDMA Buffering -----\n");

	// Halt VDMA and unmap memory ranges
	vdma_halt(&handle);

	vdma_dump_all_registers(&handle);

	xil_printf("----- End of VDMA Demo -----\n");

	return 0;
}
