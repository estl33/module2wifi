/*
 * TouchInterrupt.c
 *
 *  Created on: Feb 14, 2018
 *      Author: f3o0b
 */

#include "TouchInterrupt.h"
#include "TouchEventHandler.h"
#include "Renderer.h"

	
	
static void timer_isr (void * context)
{
  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
  //printf("IN ISR\n");
  IOWR_ALTERA_AVALON_PIO_DATA(LED_PIO_BASE, 1);

 // volume_control(data_in, reading_in, reading_in_prev, reading_in_prev_1, reading_in_next);
//
//  handleTouch(&touchHandler);
//  partialRender(&renderer);
}


void Init_Interrupt(){
	  //Timer Initialization
	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
	  IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

	  //This can be changed to alter timer freq
	  IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x9f00);//870);//0x9f00);
	  IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0024);

	  //Register ISR for timer event
	  alt_ic_isr_register(TIMER_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_IRQ, timer_isr, 0, 0);


	  //Start timer and begin the work
	  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

	  printf("Init interrupt\n");
}
