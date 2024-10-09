#ifndef HALINTERRUPT_H_
#define HALINTERRUPT_H_
#include "stdint.h"

#define INTERRUT_HANDLER_NUM    255

typedef void (*InterHdlr_fptr)(void); //pointer for the interrupt handler function

void Hal_interrupt_init(void);
void Hal_interrupt_enable(uint32_t interrupt_num);
void Hal_interrupt_disable(uint32_t interrupt_num);
void Hal_interrupt_register_handler(InterHdlr_fptr handler, uint32_t interrput_num);
void Hal_interrupt_run_handler(void);

#endif /*HALINTERRUPT_H_*/