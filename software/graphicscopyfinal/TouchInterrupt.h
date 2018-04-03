#ifndef TOUCHINTERRUPT_H_
#define TOUCHINTERRUPT_H_

#include <stdio.h>
#include <string.h>
#include "sys/alt_flash.h"

#include "sys/alt_irq.h"
#include "system.h"

#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"

//Base address from Qsys
#define TIMER_BASE 0x2080
#define TIMER_IRQ_INTERRUPT_CONTROLLER_ID 4
#define TIMER_IRQ 4
#define LED_PWM_BASE 0x2010
#define LED_PIO_BASE 0x2010
#define HEX0_1_PIO_BASE (volatile char *) 0x2030
#define HEX2_3_PIO_BASE (volatile char *) 0x2040
#define HEX4_5_PIO_BASE (volatile char *) 0x2050

static void timer_isr (void * context);
void Init_Interrupt();


#endif /* TOUCHINTERRUPT_H_ */
