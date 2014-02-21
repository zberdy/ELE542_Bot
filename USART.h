#ifndef USART_H_
#define USART_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU ((uint32_t)16000000)
#define BAUD_RATE ((uint32_t)9600)
#define UART_BAUD_SELECT (uint8_t)(F_CPU/(BAUD_RATE << 4) - 1)

#define DEBUT_DEBUG 0xFE
#define FIN_DEBUG	0xFF



void USART_Init(void);


uint8_t USART_Recevoir(void);
void USART_Transmettre(uint8_t caractere);
void Lire_Trame(uint8_t *vitesse, uint8_t *angle);

#endif 
