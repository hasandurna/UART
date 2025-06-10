#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "io.h"
#include "system.h"
#include "uart.h"

void init(void)
{
  Sys_IoInit();
  Sys_TickInit();
  Sys_ConsoleInit();

  UART_Init(UART_1, 115200);

  IO_Write(IOP_LED, 1);
  IO_Init(IOP_LED, IO_MODE_OUTPUT);
}

void Task_LED(void)
{
  
}

void Task_Print(void)
{
  static unsigned long count;
  printf("SAYI: %10lu", ++count);
  // printf("SAYI: %10lu\r", ++count);   hep aynı satıra yazar  (eski veriyi silip)
}

int main()
{

  int c;
  
// Çalışma zamanı yapılandırmaları
init();

printf("Merhaba\n");
  
// Görev çevrimi
  
 // Bloke olmayan echo örneği (veri almadığı sürece)
  
 while (1)
{
  if (UART_DataReady(UART_1))  {
    c = UART_Recv(UART_1);
    if (c < 0)                     // hata var mı diye kontrol edelim 
      printf("\nHATA: %\n", c);
    else
      printf("\n%c\n", c);
  }                    
    
} 
  
// return 0;

}
