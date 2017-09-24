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

const char * FILE_SYSLOG = "/home/debian/Desktop/daily_Syslog.txt";

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
pthread_t 	thread_id_syslog;

/*******************************************
*	      L O C A L  F U N C T I O N S      *
********************************************/
void * 			Thread_Send_Daily_Syslog();
static int 		Check_Time_Send_Syslog(void);
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

		if(Check_Time_Send_Syslog() == TRUE){
			if( Internet_Connection_Status() == FALSE){
				Syslog_Message("PROBLEM_SEND_DAILY RAPPORT, ");
				Syslog_Message("NO_CONNECTION, ");
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
 Function     : Check_Time_Send_Syslog()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
static int Check_Time_Send_Syslog(void){

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
    	Syslog_Message(START_FILE);
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
void Syslog_Message(char *message)
{

	// Declarations Variables
	FILE * 	fp_syslogLog;
	time_t rawtime;
	struct tm * timeinfo;
	int iLength_message = 0;
	// Instructions

	// Open file, Append Mode
	fp_syslogLog = fopen ( FILE_SYSLOG, "a");
	if(fp_syslogLog == NULL)
	{
		printf("Can't open file '%s,", FILE_SYSLOG);
		return;
	}

	// Get time
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	// Write line into the file
	iLength_message =  strlen(message);
	fwrite(message, 1, iLength_message, fp_syslogLog);

	// Add Timestamp inot the file
	fwrite(asctime (timeinfo) , 1 , strlen(asctime (timeinfo)), fp_syslogLog);


	// Close file
	fclose(fp_syslogLog);

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
	send_binary(newSockFile, FILE_SYSLOG);
	close_socket(socketFile, newSockFile);
}
