/*
 * ADC.h
 *
 * Created: 2014-01-29 10:53:44
 *  Author: KOLY-453
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>
#include <avr/interrupt.h>

#define PORT_ADC	PORTA

#ifndef GAUCHE
#define GAUCHE 0 
#endif

#ifndef DROIT
#define DROIT 1
#endif


#define VG		0
#define VD		1
#define DIR_G	2
#define DIR_D	3


/* Fonctions */

void ADC_Init(void);
void moyenne_Moteur(uint16_t vitesseMoteurCopie[], volatile uint16_t somme_vitesse_copie[], volatile uint8_t nombre_echantillon_copie[]);

//Variables externes


#endif /* ADC_H_ */
