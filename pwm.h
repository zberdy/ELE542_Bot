/**
 * \file    PWM.h
 * \author	Hadrien Beck
 * \author	Daniel Thiffeault
 * \version 1.0
 * \date    28 janvier 2014
 * \brief   Fonctions de prototypages pour le module PWM. Contient tous les prototypes
 *          macros, structures et variables globales.
 */

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU ((uint32_t)16000000)
#endif

#define PWM_FREQ_ENTREE	((uint32_t)16000000) 
#define PWM_FREQ_CIBLE	((uint32_t)200)
#define PWM_PRESCALE	((uint32_t)8) 

#ifndef GAUCHE
#define GAUCHE 0
#endif

#ifndef DROIT
#define DROIT 1
#endif

#define DIR_G1	2
#define DIR_G2	3
#define PWM_G	4
#define PWM_D	5
#define DIR_D1	6
#define DIR_D2	7
#define CAL     4
#define REGIME_TRANSITOIRE 500 //Temps d'attente avant de lancer l'aquisition
#define ECHANTILLON_INIT 100 //Nombre d'échantillon voulu pour la calibration des moteurs
/* Macros */

#define PWM_TOP (uint16_t)(((PWM_FREQ_ENTREE / PWM_PRESCALE) / PWM_FREQ_CIBLE) - 1) //



//volatile uint8_t PWM_fin_cycle = 0;

/* Fonctions */

void PWM_Init(void);
void PWM_Ecrire(uint8_t cycleG, uint8_t cycleD);
void PWM_calibrer(void);

#endif /* PWM_H_ */
