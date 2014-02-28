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
#include "moteur.h"

//Variable global pour USART
volatile uint8_t donneeRecue, numBuffer=0, numBufferEnvoi=0;
volatile uint8_t buffer [TAILLE_BUFFER][TAILLE_BUFFER]={{0}};
uint8_t debug=0;

//Variable de Temps
volatile int8_t flag5ms=0;

//Variable de calcul de la vitesse des moteurs
volatile uint16_t somme_vitesse[2] = {0, 0};
volatile uint8_t nombre_echantillon[2] = {0, 0};
float vitesseMoteurCorr[2]={0.0,0.0};
uint16_t vitesseMoteur[2]={0,0};
float dutyCycle[2]={0,0};


//Variable de calibration des moteurs
uint16_t vMaxPlus[2]={0,0}, vZeroPlus[2]={0,0}, vMaxMoins[2]={0,0}, vZeroMoins[2]={0,0};
float aPlus[]={0,0}, bPlus[]={0,0}, aMoins[]={0,0}, bMoins[]={0,0};
//Variable de commande du Robot
float vitesseCommande=0, angleCommande=0;

//Variable de Test à suprimmer
uint8_t test1=0;

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
	//USART_Debug('4');
	//USART_Transmettre(DEBUT_DEBUG);
	flag5ms=0;

	while(1)
    {
		//Reception de la commande
		Lire_Trame(&vitesseCommande, &angleCommande);
		
		if (flag5ms!=0) //Toutes les 5ms on déclanche le calcul du PWM
		{
			//Test de diode clignotant toute les 500ms (à supprimer)
			test1++;
			if (test1>100)
			{
				test1=0;
				PORTB^=0b10000000;
			}
			flag5ms=0;
			
			// On calcul la moyenne de la vitesse non corrigé des 5ms prec
			moyenne_Moteur(vitesseMoteur, somme_vitesse, nombre_echantillon); //Attention vitesse moteur est encore une valeure entre 0 et 1023
			
			//Moteur gauche
			if ((PORTA & 0b00100000) == 0)//Si le moteur tourne en marche avant
			{
				if (vitesseMoteur[GAUCHE] > vZeroPlus[GAUCHE]) //
				{
					//vitesseMoteurCorr contient la vitesse moteur corrigé des défaults des moteurs et est compris dans l'interval [-1.0; 1.0]
					vitesseMoteurCorr[GAUCHE]=(float)vitesseMoteur[GAUCHE]*aPlus[GAUCHE]+bPlus[GAUCHE];
				}
				else
				{
					vitesseMoteurCorr[GAUCHE]=0.0;
				}
			}
			else
			{
				if (vitesseMoteur[GAUCHE] > vZeroMoins[GAUCHE]) //Si le moteur tourne en marche arrière
				{
					vitesseMoteurCorr[GAUCHE]=(float)vitesseMoteur[GAUCHE]*aMoins[GAUCHE]+bMoins[GAUCHE];
				}
				else
				{
					vitesseMoteurCorr[GAUCHE]=0.0;
				}
			}
			
			//Moteur Droit
			if ((PORTA & 0b00100000) == 0)//Si le moteur tourne en marche avant
			{
				if (vitesseMoteur[DROIT] > vZeroPlus[DROIT])
				{ 
					vitesseMoteurCorr[DROIT]=(float)vitesseMoteur[DROIT]*aPlus[DROIT]+bPlus[DROIT]; 
				}
				else
				{
					vitesseMoteurCorr[DROIT]=0.0;
				}
			}
			else
			{
				if (vitesseMoteur[GAUCHE] > vZeroMoins[GAUCHE]) //Si le moteur tourne en marche arrière
				{
					vitesseMoteurCorr[GAUCHE]=-((float)vitesseMoteur[GAUCHE]*aMoins[GAUCHE]+bMoins[GAUCHE]);
				}
				else
				{
					vitesseMoteurCorr[GAUCHE]=0.0;
				}
			}
	    } 
		
		//Calcul du duty cycle 
		CalculPWM(vitesseCommande, angleCommande, vitesseMoteurCorr[GAUCHE], vitesseMoteurCorr[DROIT], &dutyCycle[GAUCHE], &dutyCycle[DROIT]);
		
	}
}
