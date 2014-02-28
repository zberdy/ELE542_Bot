#include "USART.h"

extern volatile uint8_t donneeRecue, numBuffer, numBufferEnvoi, debug;
extern volatile uint8_t buffer[][TAILLE_BUFFER];

void USART_Init()
{
	UBRRH = 0x00;
	UBRRL = UART_BAUD_SELECT; //(uint8_t)(F_CPU / 153600) - (uint8_t)1;
	//- vitesse = 9600 Baud - 1 bit d’arrêt - Mode : XXXXXXXXXXX
	//- 8 bits de données - pas de parité
	UCSRA = (0 << RXC) | (0 << TXC)	| (1 << UDRE) | (0 << FE) | (0 << DOR) | (0 << PE) | (0 << U2X)	| (0 << MPCM);
	UCSRB = (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (1 << RXEN)  |  (1 << TXEN) | (0 << UCSZ2)  | (0 << RXB8)	| (0 << TXB8);
	UCSRC = (1 << URSEL) | (0 << UMSEL) | (0 << UPM1) | (0 << UPM0) | (0 << USBS) | (1 << UCSZ1) | (1 << UCSZ0) | (0 << UCPOL);	 
}

//Intéruption pour la reception des trames de commandes
ISR(USART_RXC_vect)
{
	donneeRecue=UDR;
	//PORTB^=0x80;
	if (debug==0)
	{
		USART_Envoie(donneeRecue);
	}
}


//Intéruption pour l'envoi des données via l'USART
ISR(USART_UDRE_vect)
{
	if (buffer[numBufferEnvoi][1]!=0)
	{
		UDR=buffer[numBufferEnvoi][0];
		buffer[numBufferEnvoi][1]=0;
		numBufferEnvoi=(numBufferEnvoi+1)%(TAILLE_BUFFER);
	}
	else
	{
		UCSRB &= ~(1 << UDRIE); //Lorsqu'on à envoyé notre donnée dans l'UDR on désactive l'intéruption
	}
	
}

//Système de buffer pour USART
void USART_Envoie(uint8_t donneeEnvoi)
{
	//PORTB=(PORTB & 0x80)|(~numBuffer);
	buffer[numBuffer][0]=donneeEnvoi;
	buffer[numBuffer][1]=1;
	numBuffer=(numBuffer+1)%TAILLE_BUFFER;
	UCSRB|=(1 << UDRIE);
}

//Fonction pour envoyer une donnée(1octet) dans la console
//de commande de notre robot
void USART_Debug(uint8_t donneeDebug)
{
	debug=1;
	USART_Envoie(DEBUT_DEBUG);
	USART_Envoie(donneeDebug);
	USART_Envoie(FIN_DEBUG);
	debug=0;
}


//Fonction de lecture des trames de commande
void Lire_Trame(uint8_t *vitesse, uint8_t *angle)
{
	uint8_t etat = 1;
	switch (etat)
		{

		case 1:
			{
				if (donneeRecue == 0xF1) // réception d'une commande normale
				{
					etat = 2;
					break;
				}
			}

		case 2:
			{
				*vitesse = donneeRecue;
				etat = 3;
				break;
			}

		case 3:
			{
			
				*angle = donneeRecue;
				etat = 0;
				break;
			}
		}

}




