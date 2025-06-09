#include "io.h"
#include "system.h"
#include "uart.h"

typedef struct {
	IO_IDX ioRx;					// RX pin io index
	IO_IDX ioTx;					// TX pin io index
	
	uint32_t ckUART;			// Clock maske değeri
	USART_TypeDef *pUSART;		// CMSIS periph. pointer to struct.
	
} UART_CFG;

static UART_CFG _uCfg[] = {
	{ IOP_U1RX, IOP_U1TX, RCC_APB2Periph_USART1, USART1 },
	{ IOP_U2RX, IOP_U2TX, RCC_APB1Periph_USART2, USART2 },
	{ IOP_U3RX, IOP_U3TX, RCC_APB1Periph_USART3, USART3 },
	
};

// UART çevresel birimini başlatır
// 1. parametre: UART index (0, 1, 2)
// 2. parametre: baudrate
// 8-N-1	8 data bit, Parity bit yok, 1 tane de stop bit
void UART_Init(UART_PERIPH idx, int baud)
{
	USART_InitTypeDef uInit;
	
	// Adım 1) I/O uçlarını yapılandırılır
	/*
	Hangi ucun nasıl yapılandırılacağı Reference Manual içinde (GPIOs and AFIOs) yazar.
	*/
	IO_Init(_uCfg[idx].ioRx, IO_MODE_INPUT);		// Rx ucu input floating olmalı
	
	IO_Init(_uCfg[idx].ioTx, IO_MODE_ALTERNATE);	// Tx ucu alternate function push-pull olmalı 
	
	// Adım 2) UART çevresel birim için clock sağlanır
	if (idx == UART_1)
		RCC_APB2PeriphClockCmd(_uCfg[idx].ckUART, ENABLE);
	else
		RCC_APB1PeriphClockCmd(_uCfg[idx].ckUART, ENABLE);
	
	// Adım 3) Init yapısı başlatılır
	
	// Adım 4) Çevresel birim aktive edilir (Periph -> ON)
	
	
}