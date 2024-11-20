#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"
#include "stdio.h"
static KernelTcb_t* sCurrent_tcb;
static KernelTcb_t* sNext_tcb;
static KernelTcb_t sTask_list [MAX_TASK_NUM];
static uint32_t sAllocated_tcb_index;
static uint32_t sCurrent_tcb_index ;
static KernelTcb_t* Scheduler_round_robin_algorithm(void);



void Kernel_task_init(void)
{
    sAllocated_tcb_index =0;
    sCurrent_tcb_index = 0;

    for(uint32_t i =0; i<MAX_TASK_NUM; i++){
        sTask_list[i].stack_base =(uint8_t*) (TASK_STACK_START + (i*USR_STACK_SIZE));
        sTask_list[i].sp = (uint32_t) sTask_list[i].stack_base + USR_STACK_SIZE - 4;

        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t* ctx = (KernelTaskContext_t*) sTask_list[i].sp;
        ctx->pc =0;
        ctx-> spsr = ARM_MODE_BIT_SYS;
    }
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc) // for piority shceduler we need additional priority value when create task.
{
    
    KernelTcb_t * new_tcb = &sTask_list[sAllocated_tcb_index++];

    if(sAllocated_tcb_index > MAX_TASK_NUM){
        return NOT_ENOUGH_TASK_NUM;
    } // exception controll

    // new_tcb->priority = priority ; //should be filled out if use priority scheduler 
    
    KernelTaskContext_t* ctx = (KernelTaskContext_t*) new_tcb->sp;
    debug_printf("task located at 0x%x ! \n", new_tcb->sp);
    ctx->pc = (uint32_t) startFunc;
    return (sAllocated_tcb_index - 1);
}

__attribute__((naked)) void Kernel_task_context_switching(void)
{
    __asm__ ("B Save_context");
    __asm__ ("B Restore_context");
}

static __attribute__((naked)) void Save_context(void)
{
    //save context (register to stack ) //현재 스택 포인터의 위치가 어딘줄 알고 ???
    __asm__ ("PUSH {lr}"); //=sCurrent_tcb 
    __asm__ ("PUSH {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("MRS r0, cpsr");
    __asm__ ("PUSH {r0}");
    // save current stack pointer in to TCB 
    __asm__ ("LDR r0, =sCurrent_tcb");
    __asm__ ("LDR r0, [r0]");
    __asm__ ("STMIA r0!, {sp}");
}

static __attribute__ ((naked)) void Restore_context(void)
{
    // get next tasks sp
    __asm__ ("LDR r0, =sNext_tcb");
    __asm__ ("LDR r0, [r0]");
    __asm__ ("LDMIA r0!, {sp}");
    // restore context(stack to register) 
    __asm__ ("POP {r0}");
    __asm__ ("MSR cpsr, r0"); 
    __asm__ ("POP  {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__ ("POP {pc}"); // and now run(task)
}

/*
RR scheduler 
*/
static KernelTcb_t* Scheduler_round_robin_algorithm(void)
{
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index ;
    return &sTask_list[sCurrent_tcb_index];
}


void Kernel_task_start(void)
{
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_context();
}

void Kernel_task_Scheduler(void)
{
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();

    Kernel_task_context_switching();
}

/*
priority scheduler (Not Used)
*/

// static KernelTcb_t* Scheduler_priority_algorithm(void)
// {
//     KernelTcb_t* sCurrent = &sTask_list[sCurrent_tcb_index];
//     for(uint32_t i =0; i< sAllocated_tcb_index; i++)
//     {
//         KernelTcb_t* pNextTcb = &sTask_list[i];
//         if(sCurrent != pNextTcb){
//             if(pNextTcb->priority < sCurrent->priority){
//                 return pNextTcb;
//             }
//         }
//     }
//     return sCurrent_tcb_index;
// }


uint32_t Kernel_task_get_current_task_id(void)
{
    return sCurrent_tcb_index;
}

