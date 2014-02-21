/**
 * \file PWM.c
 * \brief Modulation des cycles de service du moteur.
 * \author	Hadrien Beck
 * \author Daniel Thiffeault
 * \version 1.0
 * \date 28 janvier  2014
 * \details PWM est une méthode de modulation qui permet d'ajuster le cycle
 *  de service des moteurs. Le cycle sera constamment ajusté à partir des 
 *  valeurs retournées par la fonction \a CalculPWM(). 
 */

/**
 * \fn void PWM_Init(void) 
 * \brief Configure le timer en mode fast PWM
 * \return Aucune.
 */

#include "pwm.h"
#include "ADC.h"

volatile uint16_t compteur1, compteurA, compteurB;
extern uint16_t vMaxPlus[2], vZeroPlus[2], vMaxMoins[2], vZeroMoins[2];
volatile uint8_t  calibration_req;
volatile uint8_t  calibration_rdy;
extern volatile uint8_t nombre_echantillon[];
extern volatile uint16_t somme_vitesse[];
extern volatile uint8_t flag5ms;

void PWM_Init(void) {
	
	compteur1 = 0;
	compteurA = 0;
	compteurB = 0;
	DDRB = 0xFF;
	//- Fast PWM
	//PWMA = moteur droit - Interruption de débordement
	//- Niveau haut au débordement
	//- Niveau bas à la comparaison
	// Configuration mode 14 (Fast PWM avec ICR1)
	DDRD = (1 << PIND4) | (1 << PIND5);  //Pin 4 et 5 configurées en sortie
	TCCR1A =  (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1)  | (0 << COM1B0)   |  (0 << FOC1A)  |  (0 << FOC1B) |  (1 << WGM11) | (0 << WGM10);
	TCCR1B =  (0 << ICNC1)  | (0 << ICES1)  | (0 << 5)      |  (1 << WGM13)    |  (1 << WGM12)  | ( 0 << CS12)  |  (1 << CS11)  | (0 << CS10);
//	TCNT1 = 0x0000;
	OCR1A = 2500;                                                                                                                                                                                                                                                                  
	OCR1B = 1250;
	ICR1 = PWM_TOP;
	TIMSK = (0 << OCIE2)  | (0 << TOIE2)  | (0 << TICIE1) |   (0 << OCIE1A) | (0 << OCIE1B) |  (1 << TOIE1) | (0 << OCIE0) | (0 << TOIE0);
//	TIFR = (0 << OCF2)  | (0 << TOV2)  | (0 << ICF1)     |  (1 << OCF1A)  |  (1 << OCF1B) | (0 << TOV1)  | (1 << OCF0)	 | (0 << TOV0);
	
}


/**
 * \fn ISR(TIMER1_OVF_vect)
 * \brief Traitement de l'interruption de débordement du timer.
 */

void PWM_Ecrire(uint8_t cycleG, uint8_t cycleD)
{
	OCR1A = (uint16_t)(cycleG * PWM_TOP / 100);
	OCR1B = (uint16_t)(cycleD * PWM_TOP / 100);
}

void PWM_calibrer(void)
{
	
	//Calibration de Vmax+
	PORTB |= (1 << DIR_G1) | (0<<DIR_G2) | (0 << PWM_G) | (0<<PWM_D) | (1 << DIR_D1)| (0 << DIR_D2); //conf V+
	PORTA |= (1 << CAL);

	//Lancer aquisition
	calibration_req = 1;
	calibration_rdy =0;
	while (calibration_rdy!=1)
	;
	calibration_rdy=0;
	nombre_echantillon[GAUCHE]=0;
	nombre_echantillon[DROIT]=0;
	somme_vitesse[GAUCHE]=0;
	somme_vitesse[DROIT]=0;
	while(nombre_echantillon[GAUCHE]<ECHANTILLON_INIT && nombre_echantillon[DROIT]<ECHANTILLON_INIT)
	;//On attend que l'ADC est recolté assez d'échantillons
	moyenne_Moteur(vMaxPlus, somme_vitesse, nombre_echantillon);
	
	//Calibration de Vzero+
	PORTB |= (1 << DIR_G1) | (0 << DIR_G2) | (0 << PWM_G) | (0 << PWM_D) | (1 << DIR_D1)| (0 << DIR_D2); //conf V+
	PORTA |= (0 << CAL);
	calibration_req = 1;
	calibration_rdy =0;
	while (calibration_rdy!=1)
	;
	nombre_echantillon[GAUCHE]=0;
	nombre_echantillon[DROIT]=0;
	somme_vitesse[GAUCHE]=0;
	somme_vitesse[DROIT]=0;
	calibration_rdy=0;
	while(nombre_echantillon[GAUCHE]<ECHANTILLON_INIT && nombre_echantillon[DROIT]<ECHANTILLON_INIT)
	;//On attend que l'ADC est recolté assez d'échantillons
	moyenne_Moteur(vZeroPlus, somme_vitesse, nombre_echantillon);
	
	//Calibration de Vmax-
	PORTB |= (0 << DIR_G1) | (1 << DIR_G2) | (0 << PWM_G) | (0 << PWM_D) | (0 << DIR_D1)| (1 << DIR_D2); //conf V+
	PORTA |= (1 << CAL);
	calibration_req = 1;
	calibration_rdy =0;
	while (calibration_rdy!=1)
	;
	nombre_echantillon[GAUCHE]=0;
	nombre_echantillon[DROIT]=0;
	somme_vitesse[GAUCHE]=0;
	somme_vitesse[DROIT]=0;
	calibration_rdy=0;
	while(nombre_echantillon[GAUCHE]<ECHANTILLON_INIT && nombre_echantillon[DROIT]<ECHANTILLON_INIT)
	;//On attend que l'ADC est recolté assez d'échantillons
	moyenne_Moteur(vMaxMoins, somme_vitesse, nombre_echantillon);
	
	//Calibration de Vzero-
	PORTB |= (0 << DIR_G1) | (1 << DIR_G2) | (0 << PWM_G) | (0 << PWM_D) | (0 << DIR_D1)| (1 << DIR_D2); //conf V+
	PORTA |= (0 << CAL);
	calibration_req = 1;
	calibration_rdy =0;
	while (calibration_rdy!=1)
	;
	nombre_echantillon[GAUCHE]=0;
	nombre_echantillon[DROIT]=0;
	somme_vitesse[GAUCHE]=0;
	somme_vitesse[DROIT]=0;
	calibration_rdy=0;
	while(nombre_echantillon[GAUCHE]<ECHANTILLON_INIT && nombre_echantillon[DROIT]<ECHANTILLON_INIT)
	; //On attend que l'ADC est recolté assez d'échantillons
	moyenne_Moteur(vZeroMoins, somme_vitesse, nombre_echantillon);
}


ISR(TIMER1_OVF_vect){
	
	// Déclenché à chaque 5 ms en conformité est avec les paramètres d'initialisation 
	// de la fonction PWM_init().
	flag5ms=1;
	if (calibration_req == 1)
	{
		compteur1++;
		if (compteur1 >= REGIME_TRANSITOIRE)
		{
			calibration_req = 0;
			compteur1 = 0;
			calibration_rdy = 1;

		}
		
	}
	
	
}

/*
ISR(TIMER1_COMPA_vect){
	
	// Déclenché à chaque 5 ms en conformité est avec les paramètres d'initialisation 
	//   de la fonction PWM_init().
	
	compteurA++;
	if (compteurA >= 1600)
	{
		compteurA = 0;
		PORTB ^= 0b00000010;
	
		PWM_fin_cycle = 1;

	}
}


ISR(TIMER1_COMPB_vect){
	
	// Déclenché à chaque 5 ms en conformité est avec les paramètres d'initialisation 
	// de la fonction PWM_init().
	
	compteurB++;
	if (compteurB >= 1600)
	{
		compteurB = 0;
		PORTB ^= 00000001;
	
		PWM_fin_cycle = 1;

	}
}
*/

