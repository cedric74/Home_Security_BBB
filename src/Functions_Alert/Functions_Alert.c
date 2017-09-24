/*
 * Functiond_Alerte.c
 *
 *  Created on: Feb 28, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Functions_Alert.h"
#include "../Utils/Utils.h"

/*******************************************
*               D E F I N E                *
********************************************/


/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

const char * alertFile 		 = "/home/debian/Desktop/AlertFile.txt";

const int TIME_DELAYS_ALERT = 900; // 60s *15 = 15 minnutes

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
volatile unsigned char u8DelaysAlert = FALSE;

/*******************************************
*	      L O C A L  F U N C T I O N S      *
********************************************/
static void CreateAlertFile(char cDoor[5]);
static void SendAlertFile();
static int sendSMS();

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/

/*
 ============================================
 Function     : Thread DelaysNextAlert()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * DelaysNextAlert()
{
	// Declarations Variables
	int old_cancel_state;

	// Instructions

	// Start Section critic
	pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);

	// Reset Flag Detection
	u8DelaysAlert = TRUE;

	// End Section critic
	pthread_setcancelstate (old_cancel_state, NULL);

	// Sleep
	sleep(TIME_DELAYS_ALERT);

	// Start Section critic
	pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &old_cancel_state);

	// Reset Flag Detection
	u8DelaysAlert = FALSE;

	// End Section critic
	pthread_setcancelstate (old_cancel_state, NULL);

	return NULL;
}


/*
 ============================================
 Function     : Ping_Phone()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
/*
int Ping_Phone(){

	// Declarations Variables
	int iValue = NO_PHONE;

	// Ping Cedric
	int iret = system("ping -c1  192.168.0.200");			//system("ping -c1 192.168.0.200");
	if(iret !=0 ){
		iValue = NO_PHONE;
		File_Log("Cedric No Here, ", 16);
		//printf("\n Cedric No Here \n");
	}else{
		iValue = PHONE_CEDRIC;
		File_Log("Cedric Here, ", 12);
		//printf("\n Cedric Here\n\n");
	}

	return iValue;

}*/

/*
 ============================================
 Function     : send_Alert()
 Parameter    : int iSmsok
 Return Value : int
 Description  :
 ============================================
 */
int send_Alert(int iSmsok, char strCaptor[5]){

	// Declarations Variables
	int iVal;

	// Instructions
	if( Internet_Connection_Status() == FALSE){
		Syslog_Message("PROBLEM_SEND_ALERT, ");
		Syslog_Message("NO_CONNECTION, ");

		printf("PROBLEM_SEND_ALERT/n");
		return ERROR;
	}

	printf("CreateAlertFile\n");
	// Create Alert File
	CreateAlertFile(strCaptor);

	// Test Delays Next Alert Variables
	if(u8DelaysAlert == FALSE){
		// Send Alert File by Email
		SendAlertFile();

		// Send Alert by SMS
		if(iSmsok == SMS_OK){
		iVal = sendSMS();
			if( iVal == ERROR){
				Syslog_Message("PROBLEM_SEND_ALERT, ");
				Syslog_Message("FAILED_SEND_SMS, ");
				return ERROR;
			}
			Syslog_Message("Send SMS, ");
		}

		// Launch Delays Next Alert Thread
		DelaysNextAlert();
	}

	return OK;
}

/*
 ============================================
 Function     : sendSMS()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static int sendSMS(){
	//int iLoop;
	//for(iLoop = 0 ; iLoop < u8NbUSer; iLoop++){
		char buffer[200];
		snprintf(buffer , 200, "ssmtp -s \"ALERT Email\" ");
		strcat(buffer,tabUser[MAIN_USER+1].sNumPhone);
		strcat(buffer,"@sms.fido.ca");
		printf("Test: %s", buffer);
		int iReturn =system(buffer);
		if(iReturn == ERROR){
			//perror("Failed to invoke mpack");
		    return ERROR;
		}
	//}

    return OK;
}
/*
 ============================================
 Function     : CreateAlertFile()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static void CreateAlertFile(char cDoor[5]){

	// Declarations
	const int iSizeBuffer = 32;
	time_t rawtime;
	struct tm * timeinfo;
	char cBuffer[32]={0};
	FILE * fpLog;

	// Instructions
	// Create Alert Message with Door alert
	snprintf(cBuffer , iSizeBuffer, "Home Security System Alert ");
	strcat(cBuffer, cDoor);

	// Time
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	// Open File
	fpLog = fopen ( alertFile, "w");

	// Write File
	fwrite(cBuffer , 1 , iSizeBuffer , fpLog );
	fwrite(asctime (timeinfo) , 1 , 25 , fpLog );

	// Close File
	fclose(fpLog);
}

/*
 ============================================
 Function     : SendReportFile()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static void SendAlertFile(){
	int iLoop ;

	for( iLoop = 0 ; iLoop < u8NbUSer; iLoop++){
		char buffer[200];
		snprintf(buffer , 200, "mpack -s \"Alert Home Security System\" /home/debian/Desktop/AlertFile.txt ");
		strcat(buffer,tabUser[iLoop].sAddress);
		int iReturn =system(buffer);

		//printf("File : %s \n",buffer);

		if(iReturn == ERROR){
			 //perror("Failed to invoke mpack");
		}else{
			//TODO, need to save alert file in history folder , Add index in ConfigFile
			//system("cp /home/debian/Desktop/AlertFile.txt /home/debian/Desktop/history/");
			//system("rm /home/debian/Desktop/AlertFile.txt");
		 }
	}

	system("rm /home/debian/Desktop/AlertFile.txt");
}
