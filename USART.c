#include "USART.h"

extern volatile uint8_t donnee;
volatile uint8_t caractere;

void USART_Init()
{
	UBRRH = 0x00;
	UBRRL = UART_BAUD_SELECT; //(uint8_t)(F_CPU / 153600) - (uint8_t)1;
	//- vitesse = 9600 Baud - 1 bit d’arrêt - Mode : XXXXXXXXXXX
	//- 8 bits de données - pas de parité
	UCSRA = (0 << RXC) | (0 << TXC)	| (1 << UDRE) | (0 << FE) | (0 << DOR) | (0 << PE) | (0 << U2X)	| (0 << MPCM);
	UCSRB = (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (1 << RXEN)  |  (1 << TXEN) | (0 << UCSZ2)  | (0 << RXB8)	| (0 << TXB8);
	
	UCSRC =  (1 << URSEL) | (0 << UMSEL) | (0 << UPM1) | (0 << UPM0) | (0 << USBS) | (1 << UCSZ1) | (1 << UCSZ0) | (0 << UCPOL);
	//UCSRC = 0x86;
	DDRB=0xFF;
	 
}


uint8_t USART_Recevoir(void)
{

	while ( !(UCSRA & (1 << RXC)) ) // détermine si la réception est complètée
		;

	donnee = UDR; // réception complétée
	
	switch (donnee)
	{
		case DEBUT_DEBUG:
		case FIN_DEBUG:
		{
			break;
		}
		default:
		{
			USART_Transmettre(donnee);
			break;
		}
	}
	
	return donnee;

}


void USART_Transmettre(uint8_t caractere)
{

	while ( !(UCSRA & (1 << UDRE)) )										
		;
	UDR = caractere;

}


void Lire_Trame(uint8_t *vitesse, uint8_t *angle)
{
	uint8_t etat = 1;
	switch (etat)
		{

		case 1:
			{
				if (donnee == 0xF1) // réception d'une commande normale
				{
					etat = 2;
					break;
				}
			}

		case 2:
			{
				*vitesse = donnee;
				etat = 3;
				break;
			}

		case 3:
			{
			
				*angle = donnee;
				etat = 0;
				break;
			}
		}

}

ISR(USART_RXC_vect)
{
	donnee = UDR; // réception complétée
	PORTB^=1;
	switch (donnee)
	{
		case DEBUT_DEBUG:
		case FIN_DEBUG:
		{
			break;
		}
		default:
		{
			UDR = donnee;
			break;
		}
	} 	


}
