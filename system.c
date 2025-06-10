#include <stdio.h>

#include "io.h"
#include "system.h"
#include "lcd.h"
#include "uart.h"

#define _STDIN 0
#define _STDOUT 1
#define _STDERR 2

UART_PERIPH _conUART = UART_1;


void Sys_ClockTick(void)
{
  ++_TmTick;
}

void Sys_ConsoleInit(void)
{
  LCD_Init();
  // UART_Init(_conUART, 115200);
}

void _putch(unsigned char c)
{
  LCD_putch(c);
  // UART_putch(_conUART, c);
}

