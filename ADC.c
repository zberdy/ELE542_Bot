/*
 * ADC.c
 *
 * Created: 2014-01-29 10:53:20
 *  Author: KOLY-453
 */ 

#include "ADC.h"
extern volatile uint16_t somme_vitesse[];
extern volatile uint8_t nombre_echantillon[];
	
void ADC_Init(void) 
{
	//- Fréquence de l’horloge de l’ADC = 125 kHz
	//- Mode d’opération = Freerun
	//- Référence de voltage = AREF
	//- Mode de traitement des acquisitions = Interruption
	ADMUX	= (0 << REFS1) | (0 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2)	| (0 << MUX1) | (0 << MUX0);
	ADCSRA	= (1 << ADEN) | (0 << ADSC) | (1 << ADATE) | (0 << ADIF) | (1 << ADIE) |  (1 << ADPS2)	|  (1 << ADPS1)	 |  (1 << ADPS0); 
	SFIOR	= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0) | (0 << 4)	   |  (0 << ACME) | (0 << PUD)	| (0 << PSR2)	 | (0 << PSR10);
	ADCSRA |= (1 << ADSC); //On lance l'aquisition en mode free running
	ADMUX  ^= (1 << MUX0); //On change immédiatement de canal pour la prochaine conversion
}

ISR(ADC_vect)
{
	int16_t vitesse_echantillon;
	
	ADMUX ^= (1  << MUX0); // le changement de canal débute avant la conversion
	vitesse_echantillon = ADCL;
	vitesse_echantillon |= (ADCH << 8);
	
	switch(ADMUX & (1 << MUX0)) // ADMUX = 0 --> moteur gauche, ADMUX = 1 --> moteur droit 
	{
		case GAUCHE:
		{
			if ( (PORT_ADC & DIR_G) != 0)
			{
				somme_vitesse[GAUCHE] -= vitesse_echantillon;
				nombre_echantillon[GAUCHE]++;
			}
			else
			{
				somme_vitesse[GAUCHE] += vitesse_echantillon;
				nombre_echantillon[GAUCHE]++;
			}
			break;
		}
		case DROIT:
		{
			if ( (PORT_ADC & DIR_D) != 0)
			{
				somme_vitesse[DROIT] -= vitesse_echantillon;
				nombre_echantillon[DROIT]++;
			}
			else
			{
				somme_vitesse[DROIT] += vitesse_echantillon;
				nombre_echantillon[DROIT]++;
			}
			break;
		}
	}
}

void moyenne_Moteur(uint16_t vitesseMoteur[], volatile uint16_t somme_vitesse_copie[], volatile uint8_t nombre_echantillon_copie[])
{
	nombre_echantillon[GAUCHE]=0; //Une fois l'opération de moyenne faite on réinitialise les données à 0
	somme_vitesse[GAUCHE]=0;
	nombre_echantillon[DROIT]=0;
	somme_vitesse[DROIT]=0;
	vitesseMoteur[DROIT]=somme_vitesse_copie[DROIT]/nombre_echantillon_copie[DROIT];
	vitesseMoteur[GAUCHE]=somme_vitesse_copie[GAUCHE]/nombre_echantillon_copie[GAUCHE];	
}
