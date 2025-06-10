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

// bu dosyanın bildirimini uart.h içinde yapmayı unutma
void UART_Init(UART_PERIPH idx, int baud)				
{
	USART_InitTypeDef uInit;
	
	// Adım 1) I/O uçlarını yapılandırılır
	/*
	Hangi ucun nasıl yapılandırılacağı Reference Manual içinde (GPIOs and AFIOs) yazar.
	*/
	IO_Init(_uCfg[idx].ioRx, IO_MODE_INPUT);		// Rx ucu input floating olmalı
	
	IO_Init(_uCfg[idx].ioTx, IO_MODE_ALTERNATE);    	// Tx ucu alternate function push-pull olmalı 
	
	// Adım 2) UART çevresel birim için clock sağlanır
	if (idx == UART_1)
		RCC_APB2PeriphClockCmd(_uCfg[idx].ckUART, ENABLE);
	else
		RCC_APB1PeriphClockCmd(_uCfg[idx].ckUART, ENABLE);
	
	// Adım 3) Init yapısı başlatılır ve çevresel birime uygulanır	Aşağıdaki adımlar için STM32xx_USART.h dosyasını iyi incele. 
	uInit.USART_BaudRate = baud;
	uInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uInit.USART_Mode = USART_Mode_RX | USART_Mode_Tx;
	uInit.USART_Parity = USART_Parity_No;
	uInit.USART_StopBits = USART_StopBits_1;
	uInit.USART_WordLenght = USART_WordLenght_8b;

	// yukarda yazdıgımız parametreleri USART1'e initaliaze etmiş olduk
	USART_Init(_uCfg[idx].pUSART, &uInit);				// ilk parametre yerine doğrudan USART1 de yazabilirdik
	
	// Adım 4) Çevresel birim aktive edilir (Periph -> ON)
	USART_Cmd((_uCfg[idx].pUSART, ENABLE);				// ilk parametre yerine doğrudan USART1 de yazabilirdik
	
	}

// UART çevresel birimine veri gönderir
void UART_Send(UART_PERIPH idx, unsigned char val)
{
	// İki farklı metod ile bu işlemi yapabiliriz ilk strateji şöyleydi:
	// 1) TDR yükleme için uygun mu?
	while (!USART_GetFlagStatus(_uCfg[idx].pUSART, USART_FLAG_TXE));	// TDR dolu ise bekle, boş ise yüklemeyi yap
										// Notlar 315. satır bunu anlatıyor
	// 2) Yüklemeyi yap
	// Not: Verinin gitmesini beklemiyoruz, sadece yükleme yapıp çıkıyoruz
	USART_SendData(_uCfg[idx].pUSART, val);
	
}



// UART çevresel birimine veri gönderir
void UART_Send2(UART_PERIPH idx, unsigned char val)
{
	// İki farklı metod ile bu işlemi yapabiliriz ikinci strateji şöyle:

	// 1) Önce Yüklemeyi yap
	USART_SendData(_uCfg[idx].pUSART, val);
	
	// 2) Veri gidene kadar bekle
	while (!USART_GetFlagStatus(_uCfg[idx].pUSART, USART_FLAG_TC));	         // Transmission Complete (TC) == 1 mi? değilse bekle 
										// Notlar 350. satır bunu anlatıyor
	
	
}



// Belirtilen  UART biriminin RDR'sinde veri var mı?
int UART_DataReady(UART_PERIPH idx)			
{
	return USART_GetFlagStatus(_uCfg[idx].pUSART, USART_FLAG_RXNE);     // Receive Data Register Not Empty = 1 ise boş değil, veri var demektir
	
}

int UART_Recv(UART_PERIPH idx)
{
	int ret;
	USART_TypeDef *pUART = _uCfg[idx].pUSART;
	while (!UART_DataReady(idx));					// Veri olmadığı müddetçe fonksiyon bloke olur

	if (USART_GetFlagStatus(pUART, USART_FLAG_ORE | USART_FLAG_NE 
				| USART_FLAG_FE | USART_FLAG_PE ));      // Veriyi okumadan hata var mı diye hızlıca kontrol et
	
	// eğer hata varsa hatanın tam olarak ne oldugunu tespit edelim
	{
	  if (USART_GetFlagStatus(pUART, USART_FLAG_NE))
		  ret = UART_ERR_NOISE;
	  else if (USART_GetFlagStatus(pUART, USART_FLAG_ORE))
		  ret = UART_ERR_OVERRUN;	
	  else if (USART_GetFlagStatus(pUART, USART_FLAG_FE))
		  ret = UART_ERR_FRAMING;
	  else if (USART_GetFlagStatus(pUART, USART_FLAG_PE))
		  ret = UART_ERR_PARITY;

	  USART_ReceiveData(pUART);		// dummy okuma, tek amacı statü registerindeki hata bitini sıfırlamak 
	}				       // aksi halde bir sonraki veri temiz bile olsa önceden kalan hata biti işi karıştırır.
	else
	 ret = USART_ReceiveData(pUART);
	 return ret;
}

void UART_putch(UART_PERIPH idx, unsigned char c)
{
	if (c == '\n'); 					// newline karakteri tespit edilirse, satır başı (carrige return) karakteri (\r) önce gönderilir, ardından gerçek karakter gönderilir
		UART_Send(idx, '\r');		      		// Bu, terminalin doğru bir şekilde yeni bir satıra geçmesini sağlar
	
	UART_Send(idx, c);


	// UART_Send yazan kısmı UART_Send2 yazarsan 2. stratejinin çalışam mantığını gözlemleyebilirsin
}

 
