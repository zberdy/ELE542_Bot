#ifndef USART_H_
#define USART_H_


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU ((uint32_t)16000000)
#define BAUD_RATE ((uint32_t)9600)
#define UART_BAUD_SELECT (uint8_t)(F_CPU/(BAUD_RATE << 4) - 1)
#define TAILLE_BUFFER 5
#define DEBUT_DEBUG '#'
#define FIN_DEBUG	'#'
#define CST_ROT     0.03490658504 //=2.pi/180



void USART_Init(void);
void USART_Envoie(uint8_t donneeEnvoi);
void Lire_Trame(float *vitesse, float *angle);
void USART_Debug(uint8_t donneeDebug);

#endif 
