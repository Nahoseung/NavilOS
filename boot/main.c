#include "stdint.h"

#include "HalUart.h"
#include "HalInterrupt.h"
#include "HalTimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "task.h"   
#include "kernel.h"

#define true 1


static void Hw_init(void);
static void Printf_test(void);
static void Timer_test(void);
static void Kernel_init(void);

void User_task0(void);
void User_task1(void);
void User_task2(void);

void main(void)
{
    Hw_init();

    uint32_t i=100;
    debug_printf("0x%x\n",&i);
    while(i--)
    {
        Hal_uart_put_char('-');
    }

    Hal_uart_put_char('\n');
    putstr("Hello world !\n");
    Printf_test();
    Kernel_init();
    Timer_test();
    while(1);
    

    // i=5; uint8_t ch;
    // while(i--)
    // {
    //     ch = Hal_uart_get_char();
    //     Hal_uart_put_char(ch);
    // }
    // Hal_uart_put_char('\n');
    // putstr("goodbye now we're enter into the while loop testing the keyboard interrupt \n");
    while(1);
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}

static void Printf_test(void)
{
    char* str = "printf pointer test";
    char* nullptr = 0;
    uint32_t i =5;
    uint32_t* sysctrl0 = (uint32_t*) 0x10001000;

    debug_printf("%s\n", "Hello printf");
    debug_printf("output string pointer : %s\n",str);
    debug_printf("%s is null pointer, %u number\n", nullptr, 10);
    debug_printf("%u = 5\n",i);
    debug_printf("dec=%u hex=%x\n",0xff,0xff);
    debug_printf("print zero %u\n",0);
    debug_printf("SYSCTRL0 %x\n", *(sysctrl0));
}

static void Timer_test(void){
    for (uint32_t i =0; i<3; i++){
        debug_printf("current count : %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }

}


static void Kernel_init(void)
{
    uint32_t taskId;

    Kernel_task_init();
    

    taskId = Kernel_task_create(User_task0);

    if(NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task0 creation fail \n");
    }

    taskId = Kernel_task_create(User_task1);
    if(NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task1 creation fail \n");
    }

    taskId = Kernel_task_create(User_task2);
    if(NOT_ENOUGH_TASK_NUM == taskId)
    {
        putstr("Task2 creation fail \n");
    }

    Kernel_start();
}



void User_task0(void)
{
    uint32_t local0 = 0;
    uint32_t local =0;
    uint32_t local2=0;

        while(true){
        debug_printf("User Task#0 SP=0x%x\n", &local0);
        debug_printf("User Task#0 SP=0x%x\n", &local);
        debug_printf("User Task#0 SP=0x%x\n", &local2);
        Kernel_yield(); 
        }
        


}

void User_task1(void)
{
   uint32_t local1 = 0;
   
    while(true){
        debug_printf("User Task#1 SP=0x%x\n", &local1);
        Kernel_yield();
    }

}

void User_task2(void)
{
    uint32_t local2 = 0;
    
    while(true){
        debug_printf("User Task#2 SP=0x%x\n", &local2);
        while(1);
        Kernel_yield();
    }
}



