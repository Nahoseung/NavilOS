#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "event.h"
#include "kernel.h"


extern volatile PL011_t* Uart;

static void interrupt_handler(void)
{
    uint8_t ch = Hal_uart_get_char();
    Hal_uart_put_char(ch);
    if(ch == 'u')
    {
        Kernel_send_events(KernelEventFlag_Unlock);
        return ;
    }
    if (ch =='x')
    {
        Kernel_send_events(KernelEventFlag_CmdOut);
        return ;
    }

    Kernel_send_msg(KernelMsgQ_Task0, &ch, 1);
    Kernel_send_events(KernelEventFlag_UartIn);
    
}

void Hal_uart_init(void)
{
    //Enable Uart  
    Uart->uartcr.bits.UARTEN =0; //before modify the register off the HW
    Uart->uartcr.bits.TXE=1; //turn output ON
    Uart->uartcr.bits.RXE=1; // turn input ON
    Uart->uartcr.bits.UARTEN=1; // ON HW

    //Enable input interrupt
    Uart->uartimsc.bits.RXIM = 1;

    //Register Uart interrupt handler
    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}



void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF); // Until the output buffer is empty
    // ch=((ch&0xFF)==0x08) ? 0x42: ch;
    Uart->uartdr.all=(ch & 0xFF); // clear the bits bigger than 7bits .(make only 8bits left) 
    // + if replace 0xFF to 0xDF, we can make lower alphabet to capital 
}


uint8_t Hal_uart_get_char(void)
{
    uint32_t data;

    while (Uart->uartfr.bits.RXFE); // Check that buffer is Empty 

    data = Uart->uartdr.all; // access to hw reg is heavier tasks than to local var.

    //check for an error flag
    if(data & 0xFFFFFF00)
    {
        //Clear the error (set 아닌가 ?)
        Uart-> uartrsr.all = 0xFF;
        return 0;
    }
    // data= (data==0x08) ? 0x42: data;
    return (uint8_t) (data & 0xFF); //extract data from uartdr register


}
