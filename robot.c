/*
 * robot.c
 *
 * Created: 2014-01-23 14:03:21
 *  Author: KOLY-453
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"
#include "pwm.h"
#include "ADC.h"

//Variable global pour USART
volatile uint8_t donneeRecue, numBuffer=0, numBufferEnvoi=0;
volatile uint8_t buffer [TAILLE_BUFFER][TAILLE_BUFFER]={{0}};
uint8_t debug=0;

volatile int8_t donnee=0, flag5ms=0;
volatile uint16_t somme_vitesse[2] = {0, 0};
volatile uint8_t nombre_echantillon[2] = {0, 0};
uint16_t vMaxPlus[2]={0,0}, vZeroPlus[2]={0,0}, vMaxMoins[2]={0,0}, vZeroMoins[2]={0,0}, aPlus[2]={0,0}, bPlus[2]={0,0}, aMoins[2]={0,0}, bMoins[2]={0,0}, vitesseMoteur[2]={0,0};
uint8_t vitesseCommande=0, angleCommande=0;

int main(void)
{
	sei();   
	USART_Init();
	DDRB=0xFF;
	PORTB=0x00;
	//USART_Debug('1');
	ADC_Init();
	//USART_Debug('2');
	PWM_Init();
	USART_Debug('3');
	PWM_calibrer();
	USART_Debug('4');
	//USART_Transmettre(DEBUT_DEBUG);
	flag5ms=0;

	while(1)
    {
	
	//Reception de la commande
	Lire_Trame(&vitesseCommande, &angleCommande);
	
		if (flag5ms!=0) //Toutes les 5ms on déclanche le calcul du PWM
		{
			//USART_Debug('3');
			moyenne_Moteur(vitesseMoteur, somme_vitesse, nombre_echantillon);// On calcul la moyenne de la vitesse non corrigé des 5ms prec
			if (vitesseMoteur[GAUCHE]<=vZeroPlus[GAUCHE])
			{
				vitesseMoteur[GAUCHE]=0;
			}
			else
			{
				vitesseMoteur[GAUCHE]=vitesseMoteur[GAUCHE]*aPlus[GAUCHE]+bPlus[GAUCHE];
			}
		
			if (vitesseMoteur[DROIT]<=vZeroPlus[DROIT])
			{
				
				vitesseMoteur[DROIT]=0;
			}
			else
			{
				vitesseMoteur[DROIT]=vitesseMoteur[DROIT]*aPlus[DROIT]+bPlus[DROIT];
			}
	    }
	}
}
