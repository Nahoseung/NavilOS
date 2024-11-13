#include "stdint.h"
#include "stdbool.h"

#include "kernel.h"

void Kernel_yield(void)
{
    Kernel_task_Scheduler();
}

void Kernel_start(void)
{
    Kernel_task_start();
}