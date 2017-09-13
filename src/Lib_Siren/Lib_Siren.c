/*
 * Lib_Siren.c
 *
 *  Created on: Mar 4, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Lib_Siren.h"

#include "../Lib_PWM/Lib_PWM.h"
/*******************************************
*               D E F I N E                *
********************************************/
#define TIME_SIREN		(60*5)		// 5 minutes, In Second

#define TIME_WARNING	500
#define FREQ_BUZZER		100   	// In us , Pas mal 1000 , trop 50000
#define NBE_BIP			8


// Test Sound PM
#define	PERIOD_DO		7575757 			// In ns, F = 132 Hz
#define	DUTY_DO			(PERIOD_DO/2)


/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
/*
 ============================================
 Function     : Start_Thread_Warning()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Start_Thread_Warning(){

	// Declarations Variables
	pthread_t thread_id;

	// Thread Execute Warning Sound System On
	pthread_create (&thread_id, NULL, &Thread_WarningSytemON, NULL);

}

/*
 ============================================
 Function     : Thread_WarningSytemON()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * Thread_WarningSytemON(){
	int i, i2;
	int iTimeDelay = 100000;
	int iPeriod = PERIOD_DO;

	int iDuty = ( iPeriod / 2 );
	// Test Buzzer with PWM
	Lib_pwm_control( iPeriod, iDuty );
	Lib_pwm_start();

	for( i2 = 0 ; i2 < 5 ; i2++){
		for( i = 0 ; i < NBE_BIP; i++){
			//WarningSystemOn();
			usleep(iTimeDelay);
			//Lib_pwm_stop();
			//usleep(iTimeDelay);

			//iDuty =
			Lib_pwm_control( iPeriod, iDuty);
			//Lib_pwm_start();

			// If The Interrupter is switch to OFF, Stop Buzzer
			if (stateInterrupter == STATE_OFF){
				break;
			}
			iPeriod = iPeriod/ 2 ;
			iDuty = iPeriod / 2 ;
		}
		iPeriod = PERIOD_DO;
		iDuty = ( iPeriod / 2 );
		sleep(1);
	}

	// Test Buzzer with PWM
	Lib_pwm_stop();

	return NULL;
}

/*
 ============================================
 Function     : WarningSystemOn()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void WarningSystemOn(){
	int i;
	for(i = 0; i <TIME_WARNING ; i ++){
		beh_BBB_gpio_WritePin(BUZZER, SIREN_ON );
		usleep(FREQ_BUZZER);
		beh_BBB_gpio_WritePin(BUZZER, SIREN_OFF );
		usleep(FREQ_BUZZER);
	}
}

/*
 ============================================
 Function     : Thread_Siren()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void* Thread_Siren(){

	int iLoop = 0;
	// Delays before Stop siren , Or Switch Interrupter to Off to Kill the siren
	do{
		sleep(1);
		iLoop++;
	}while((iLoop < TIME_SIREN )&&(stateInterrupter != STATE_OFF));

	// Stop Siren
	Stop_Siren();

	return NULL;
}

/*
 ============================================
 Function     : Start_Siren()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Start_Siren(){

	// Declarations Variables
	pthread_t thread_id;

	// Set Siren On
	beh_BBB_gpio_WritePin(SIREN, SIREN_ON );

	// Thread Execute Read Captor
	pthread_create (&thread_id, NULL, &Thread_Siren, NULL);

}

/*
 ============================================
 Function     : Stop_Siren()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Stop_Siren(){
	// Set Siren Off
	beh_BBB_gpio_WritePin(SIREN, SIREN_OFF );
}

/*
 ============================================
 Function     : Init_Siren()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Init_Siren(){

	// Init Buzzer
	beh_BBB_gpio_conf_dir(BUZZER, OUTPUT);

	// Init Buzzer V2 , With PWM
	Lib_pwm_init();

	// Init Siren Ouput
	beh_BBB_gpio_conf_dir(SIREN, OUTPUT);

	// Set Siren Off
	Stop_Siren();
}


