#include "stdint.h"

#include "HalUart.h"

#include "stdio.h"

static void Hw_init(void);

void main(void)
{
    Hw_init();

    uint32_t i=100;
    while(i--)
    {
        Hal_uart_put_char('-');
    }

    Hal_uart_put_char('\n');
    putstr("Hello world !");
}

static void Hw_init(void)
{
    Hal_uart_init();
}