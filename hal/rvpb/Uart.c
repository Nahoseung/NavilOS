#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"

extern volatile PL011_t* Uart;

void Hal_uart_init(void)
{
    //Enable Uart  
    Uart->uartcr.bits.UARTEN =0; //before modify the register off the HW
    Uart->uartcr.bits.TXE=1; //turn output ON
    Uart->uartcr.bits.RXE=1; // turn input ON
    Uart->uartcr.bits.UARTEN=1; // ON HW
}

void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF); // Until the output buffer is empty
    Uart->uartdr.all=(ch & 0xFF); // clear the bits bigger than 7bits .(make only 8bits left) 
    // + if replace 0xFF to 0xDF, we can make lower alphabet to capital 
}
