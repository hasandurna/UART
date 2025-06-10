#ifndef __UART_H
#define __UART_H

#ifdefine __cplusplus
extern "C" {
#endif

 typedef enum {
	 UART_1,
	 UART_2,		// 0,1,2 tam sayısına karşılık gelen değerler
	 UART_3,
 } UART_PERIPH;

enum {
   UART_ERR_NOISE = -1,
   UART_ERR_OVERRUN = -2,
   UART_ERR_FRAMING = -3,
   UART_ERR_PARITY = -4,
};

void UART_Init(UART_PERIPH idx, int baud);
void UART_Send(UART_PERIPH idx, unsigned char val);

int UART_DataReady(UART_PERIPH idx);
int UART_Recv(UART_PERIPH idx);


#ifdef __cplusplus
}
#endif

#endif
