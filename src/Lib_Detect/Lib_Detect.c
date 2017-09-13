/*
 ============================================================================
 Name        : Lib_Detect.c
 Author      : Cedric T.
 Version     : 1.0
 Date		 : 29/01/2015
 Description : 
 ============================================================================
 */

/*******************************************
*			  I N C L U D E 			   *			
********************************************/
#include "Lib_Detect.h"

/*******************************************
*               D E F I N E                *			
********************************************/


/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *			
********************************************/
volatile unsigned char 	u8DetectOn 			= 0;
static int 				iCountInter 		= 0;


/*******************************************
*	 P R O T O T Y P E   F U N C T I O N   *			
********************************************/

/*******************************************
*	          M  A I N    	               *			
********************************************/

/*
 ============================================
 Function     : main_Detect()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void main_Detect(){
	// Declarations Variables
	int old_cancel_state;
	int iSystemOn = ON;

	// Check If Intrusion
	if( u8DetectOn != INTRUSION_NO){

		// Add info in Daily Report
		if(u8DetectOn == ptrCaptorMainDoor->ePinCaptor){
			File_Log("Detect OK, Main Door, ", 22);
		}else{
			File_Log("Detect OK, Back Door, ", 22);
		}

		// Ping_Phone
		//iSystemOn = Ping_Phone();

		// Read State interrupter
		if (stateInterrupter == STATE_OFF){
			iSystemOn = OFF;
		}else{
			iSystemOn = ON;
		}

		// Check If System Is ON
		if(iSystemOn == ON){

			// Sound Alert that System ON
			Start_Thread_Warning();

			// Take A picture
			//File_Log("Take A Pic, ", 12);
			//Start_Thread_Pic();

			// Delays to Disable System with Interrupter
			printf(" DELAYS_TO_DISABLE\n");
			sleep(DELAYS_TO_DISABLE);
			if (stateInterrupter == STATE_OFF){
				iSystemOn = OFF;
			}else{
				iSystemOn = ON;
			}

			if(iSystemOn == OFF){
				// Disable System
				printf(" System OFF \n");
				File_Log("System OFF, ", 12);
			}else{
				// Alert OK
				printf(" Alert OK \n");

				// Siren ON.
				printf(" Siren ON. \n");
				File_Log("Siren  ON , ", 12);
				Start_Siren();

				// Send Alert By Mail & Sms
				if(u8DetectOn == ptrCaptorMainDoor->ePinCaptor){
					send_Alert(SMS_OK,  ptrCaptorMainDoor->sMessage);		//	SMS_OK, NO_SMS
				}else{
					send_Alert(SMS_OK, ptrCaptorBackDoor->sMessage );		//	NO_SMS
				}

			}

		}else{
			printf(" System OFF \n");
			File_Log("System OFF, ", 12);
		}

		// Print
	printf("End Alert, System in Ready Mode \n");

	// Start Section critic
	pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);

	// Reset Flag Detection
	u8DetectOn = 0;

	// End Section critic
	pthread_setcancelstate (old_cancel_state, NULL);

	}

}

/*
 ============================================
 Function     : Thread_Read_Captor()
 Parameter    : 
 Return Value : void
 Description  :
 ============================================
 */
void* Thread_Read_Input(){
	while(1){
		
		// Read Captor
		Read_Captor(ptrCaptorMainDoor);
		Read_Captor(ptrCaptorBackDoor);

		// Read Interrupter
		Read_Interrupter();

		// Delays
		usleep(WAIT_1MS);	// Each 1 ms
	}
	return NULL;
}

/*
 ============================================
 Function     : Read_Interrupter()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Read_Interrupter(){

	// Declarations Variables
	unsigned char readEntry = OFF_INTERRUPT ;

	// Instructions

	// Read Input Captor
	readEntry = beh_BBB_gpio_ReadPin(INTER_ON_OFF);

	switch(stateInterrupter){
		case STATE_OFF :
			if( readEntry == ON_INTERRUPT){
				iCountInter++;
				if( iCountInter >= COUNT_INTERRUPTER){

					// Delays 2 MIN before system switch to ON
					printf(" DELAYS_TO_ENABLE \n");
					sleep(DELAYS_TO_ENABLE);

					// Sound System ON
					Start_Thread_Warning();

					printf(" System ON, INTERRUPTER \n");
					File_Log("System switch to ON, ", 21);

					// Change State
					stateInterrupter = STATE_ON ;

					// Init iCountInter
					iCountInter = 0;
				}
			}else{
				if(iCountInter > 0 ){
					iCountInter--;
				}
			}

		break ;

		case STATE_ON :
			if( readEntry == OFF_INTERRUPT){
				iCountInter++;
				if( iCountInter >= COUNT_INTERRUPTER){

					printf(" System OFF, INTERRUPTER \n");
					File_Log("System switch to OFF, ", 22);

					// Change State
					stateInterrupter = STATE_OFF ;
				}

			}else{
				if(iCountInter > 0 ){
					iCountInter--;
				}
			}

		break;
	}
}

/*
 ============================================
 Function     : Read_Captor()
 Parameter    : 
 Return Value : void
 Description  :
 ============================================
 */
void Read_Captor(structCaptor * sCaptor){

	//TODO, Problem with Captor, Detection sometime not good
	// Declarations Variables
	unsigned char readEntry = 0 ;
	int old_cancel_state;
	
	// Instructions

	// Read Input Captor
	readEntry = beh_BBB_gpio_ReadPin(sCaptor->ePinCaptor);

	// Leave Function if Detection flag is trigger
	if(u8DetectOn != 0){
		sCaptor->icountDete = 0;
		return;
	}
	// State Machine Captor
	switch(sCaptor->stateCapt){
		case STATE_NO_DETECTION :
			if( readEntry == DETECT_OK){
				// Change State

				// Start Critic Section
				pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);
				sCaptor->stateCapt = STATE_DETECTION ;
				// End Critic Section
				pthread_setcancelstate (old_cancel_state, NULL);
				printf(" Press OK, Captor %s", sCaptor->sMessage);
			}
		break ;
		
		case STATE_DETECTION :
			if( readEntry == DETECT_OK){
				sCaptor->icountDete++;
			}else{
				sCaptor->icountDete--;

				// DEBUG
				//printf(" Detect count : %i \n", sCaptor->icountDete);
			}

			// Change State
			if(sCaptor->icountDete <= 0){
				sCaptor->stateCapt = STATE_NO_DETECTION;
			}

			if( sCaptor->icountDete >= COUNT_DETECTION){
				sCaptor->stateCapt = STATE_NO_DETECTION;
				printf(" Release OK, Count: %d, Captor %s",  sCaptor->icountDete , sCaptor->sMessage);
				sCaptor->icountDete = 0;

				// Start Critic Section
				pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);
				u8DetectOn = (unsigned char)sCaptor->ePinCaptor;
				// End Critic Section
				pthread_setcancelstate (old_cancel_state, NULL);
			}
		break;
	}
}

/*
 ============================================
 Function     : Init_Captor()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Init_Lib_Detect(){

	// Declarations Variables
	pthread_t thread_id;

	// Init Captor Main Door
	ptrCaptorMainDoor = &sCaptorMainDoor;
	ptrCaptorMainDoor->ePinCaptor = CAPTOR_MAIN_DOOR;
	ptrCaptorMainDoor->icountDete = 0;
	ptrCaptorMainDoor->stateCapt = STATE_NO_DETECTION ;
	ptrCaptorMainDoor->sMessage = "MAIN\n";
	beh_BBB_gpio_conf_dir(CAPTOR_MAIN_DOOR, INPUT_PULL_UP_EN);	//INPUT_PULL_UP_EN

	// Init Captor Back Door
	ptrCaptorBackDoor = &sCaptorBackDoor;
	ptrCaptorBackDoor->ePinCaptor = CAPTOR_BACK_DOOR;
	ptrCaptorBackDoor->icountDete = 0;
	ptrCaptorBackDoor->stateCapt = STATE_NO_DETECTION ;
	ptrCaptorBackDoor->sMessage = "BACK\n";
	beh_BBB_gpio_conf_dir(CAPTOR_BACK_DOOR, INPUT_PULL_UP_EN);	//INPUT_PULL_UP_EN

	// Init Interrupter	Input
	beh_BBB_gpio_conf_dir(INTER_ON_OFF, INPUT_PULL_UP_EN );  	//INPUT_PULL_UP_EN

	// Thread Execute Read Captor
	pthread_create (&thread_id, NULL, &Thread_Read_Input, NULL);

	// Thread Execute Send Daily Report
	pthread_create (&thread_id_Daily, NULL, &Thread_DailyReport, NULL);

}
