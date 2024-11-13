#include "stdint.h"
#include "memio.h"
#include "Interrupt.h"
#include "HalInterrupt.h"
#include "armcpu.h" 

extern volatile GicCput_t* GicCpu;
extern volatile GicDist_t* GicDist;

static InterHdlr_fptr sHandlers [INTERRUT_HANDLER_NUM]; //arr that can save function(interrupt handler) pointer 255.

void Hal_interrupt_init(void)
{
    GicCpu->cpucontrol.bits.Enable = 1; //ctrl ON
    GicCpu->prioritymask.bits.Prioritymask = GIC_PRIORITY_MASK_NONE; //set priority mask register . to let all irq
    GicDist->distributorctrl.bits.Enable = 1; //ctrl ON
    
    for(uint32_t i =0; i<INTERRUT_HANDLER_NUM; i++)
    {
        sHandlers[i]=0; //init the handler address to NULL 
    }

    enable_irq();
}

/*
#define GIC_IRQ_START           32
#define GIC_IRQ_END             95
*/

void Hal_interrupt_enable(uint32_t interrupt_num)
{
    if((interrupt_num < GIC_IRQ_START) || (GIC_IRQ_END < interrupt_num))
    {
        return ; // UND interrupt number;
    }

    uint32_t bit_num = interrupt_num - GIC_IRQ_START ; //because interrupt num start from 32.

    if(bit_num < GIC_IRQ_START)
    {
        SET_BIT(GicDist->setenable1, bit_num);        
    }

    else
    {
        bit_num-= GIC_IRQ_START;
        SET_BIT(GicDist->setenable2, bit_num);
    }
}

void Hal_interrupt_disable(uint32_t interrupt_num)
{
    if((interrupt_num < GIC_IRQ_START) ||(GIC_IRQ_END < interrupt_num))
    {
        return ; // UND interrupt number;
    }

    uint32_t bit_num = interrupt_num - GIC_IRQ_START ; //because interrupt num start from 32.

    if(bit_num < GIC_IRQ_START)
    {
        CLR_BIT(GicDist->setenable1, bit_num);
    }

    else
    {
        bit_num-= GIC_IRQ_START;
        CLR_BIT(GicDist->setenable2, bit_num);
    }
}

void Hal_interrupt_register_handler(InterHdlr_fptr handler, uint32_t interrupt_num)
{
    sHandlers[interrupt_num]=handler;
}

void Hal_interrupt_run_handler(void)
{
    uint32_t interrupt_num = GicCpu->interruptack.bits.InterruptID;

    if(sHandlers[interrupt_num] != NULL) // handle error
    {
        sHandlers[interrupt_num]();
    }

    GicCpu->endofinterrupt.bits.InterruptID = interrupt_num ;
}