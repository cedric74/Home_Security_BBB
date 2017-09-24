/*
 * Lib_Syslog.c
 *
 *  Created on: Sep 23, 2017
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Lib_Syslog.h"

#include "../Utils/Utils.h"
/*******************************************
*               D E F I N E                *
********************************************/


/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
const int HourTimeDay 	= 20;
const int MinTimeDay 	= 00;

#define WAIT_1_MIN		60		// In Second

const char * dailyReportFile = "/home/debian/Desktop/dailyReportFile.txt";

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
FILE * 		fpLog;
pthread_t 	thread_id_syslog;

/*******************************************
*	      L O C A L  F U N C T I O N S      *
********************************************/
void * 			Thread_Send_Daily_Syslog();
static int 		CheckTime(void);
static void 	Send_Syslog_By_Mail();

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/

/*
 ============================================
 Function     : Init_Syslog
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Init_Syslog()
{
	// Thread Execute Send Daily Report
	pthread_create (&thread_id_syslog, NULL, &Thread_Send_Daily_Syslog, NULL);

}

/*
 ============================================
 Function     : Thread_Send_Daily_Syslog()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * Thread_Send_Daily_Syslog(){

	while(1){
		sleep(WAIT_1_MIN);

		if(CheckTime() == TRUE){
			if( Internet_Connection_Status() == FALSE){
				Syslog_Message("PROBLEM_SEND_DAILY RAPPORT, ", 28);
				Syslog_Message("NO_CONNECTION, ", 15);
			}else{
				Send_Syslog_By_Mail();
			}

			// Sleep 1 minute to avoid to send several report the same day
			sleep(WAIT_1_MIN);
		}
	}
	return NULL;
}


/*
 ============================================
 Function     : CheckTime()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static int CheckTime(void){

	// Declarations Variables
	time_t rawtime;
	struct tm * timeinfo;

	// Instructions
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	//printf(" %s" , asctime(timeinfo));

	// Check if it's the right time of day
	if( ((timeinfo->tm_hour) == HourTimeDay) && ((timeinfo->tm_min) == MinTimeDay)){
		return	TRUE;
	}

	return	FALSE;
}

/*
 ============================================
 Function     : Send_Syslog_By_Mail()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static void Send_Syslog_By_Mail(){
	char buffer[200];
	snprintf(buffer , 200, "mpack -s \"Daily Report\" /home/debian/Desktop/dailyReportFile.txt ");
	strcat(buffer,tabUser[MAIN_USER].sAddress);
	int iReturn =system(buffer);

	if(iReturn == ERROR){
    	 //perror("Failed to invoke mpack");
    }else{
    	system("rm /home/debian/Desktop/dailyReportFile.txt");

    	// Create File Log With Version
    	Syslog_Message(START_FILE, SIZE_STRING);
     }
}

/*
 ============================================
 Function     : File_Log()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Syslog_Message(char string[50], int iLength){

	// Instructions
	fpLog = fopen ( dailyReportFile, "a");

	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	fwrite(string , 1 , iLength , fpLog );
	fwrite(asctime (timeinfo) , 1 , 25 , fpLog );

	fclose(fpLog);
}


/*
 ============================================
 Function     : Send_Syslog_To_Supervisor()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Send_Syslog_To_Supervisor(){
	int socketFile 	= create_Socket(PORT_FILE);
	int newSockFile = accept_client_connection(socketFile);
	send_binary(newSockFile, dailyReportFile);
	close_socket(socketFile, newSockFile);
}
