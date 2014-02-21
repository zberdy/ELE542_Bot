/*
 * robot.c
 *
 * Created: 2014-01-23 14:03:21
 *  Author: KOLY-453
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "pwm.h"
#include "ADC.h"

volatile int8_t donnee=0, flag5ms=0;
volatile uint16_t somme_vitesse[2] = {0, 0};
volatile uint8_t nombre_echantillon[2] = {0, 0};
uint16_t vMaxPlus[2]={0,0}, vZeroPlus[2]={0,0}, vMaxMoins[2]={0,0}, vZeroMoins[2]={0,0}, vitesseMoteur[2]={0,0};
uint8_t vitesseCommande=0, angleCommande=0;

int main(void)
{
	sei();   
	USART_Init();
	ADC_Init();
	PWM_Init();
	PWM_calibrer();
	USART_Transmettre(DEBUT_DEBUG);
	flag5ms=0;
	
	while(1)
    {
	//Reception de la commande
	Lire_Trame(&vitesseCommande, &angleCommande):
	
	if (flag5ms!=0) //Toutes les 5ms on déclanche le calcul du PWM
	{
		moyenne_Moteur(vitesseMoteur, somme_vitesse, nombre_echantillon);// On calcul la moyenne de la vitesse non corrigé des 5ms prec
		if (vitesseMoteur[GAUCHE]=<vZeroPlus[GAUCHE])
		{
			vitesseMoteur[GAUCHE]=0;
		}
		else
		{
			vitesseMoteur[GAUCHE]=vitesseMoteur[GAUCHE]*a[GAUCHE]+b[GAUCHE];
		}
		
		if (vitesseMoteur[DROIT]=<vZeroPlus[DROIT])
		{
			vitesseMoteur[DROIT]=0;
		}
		else
		{
			vitesseMoteur[DROIT]=vitesseMoteur[DROIT]*a[DROIT]+b[DROIT];
		}
    }
}
