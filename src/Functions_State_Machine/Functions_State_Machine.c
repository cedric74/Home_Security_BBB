/*
 * Functions_State_Machine.c
 *
 *  Created on: Mar 4, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Functions_State_Machine.h"

/*******************************************
*               D E F I N E                *
********************************************/
#define SLEEP_100_MS			100000
#define SLEEP_10_MS				10000

#define STOP_THREAD				1

#define	SEND_INFO_EACH_SEC		5	// In Second

#define VIDEO_OK				1

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
cmdApplication 	u8Command 			= NO_CMD;
pthread_t 		thread_Send_Data 	= 0;

int 			ret_thread 			= 0;

int 			iStopthread         = 0;
int 			iStopCommand		= 0;

int 			socketData;
int 			socketVideo;
int				newSockData;
int				newSockVideo;

unsigned char	bVideo;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
/*
 ============================================
 Function     : Init_State_Machine()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Init_State_Machine(){

	pthread_t threadId_StateMachine;

	// Thread Execute Read Command
	pthread_create (&threadId_StateMachine, NULL, &Thread_State_Machine, NULL);

	// Init socketVideo
	bVideo = 0;

}

/*
 ============================================
 Function     : Thread_State_Machine()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * Thread_State_Machine(void * p){

	// Declarations Variables
	pthread_t threadId_ReadCommand;

	do{
		///printf(" ******* Start Socket ******* \n\n");
		// Create Socket Server
		socketData 	= create_Socket(PORT_NUM);
		newSockData = accept_client_connection(socketData);

		// Thread Execute Read Command
		pthread_create (&threadId_ReadCommand, NULL, &Thread_Read_Command, NULL);

		// Thread Execute Send Data
		StartThread_Send_Data();

		// Thread Execute Read Sensor
		//StartThread_ReadSensor();			NO SENSOR AVAIBLE

		// Loop State Machine
		do{
			// State Machine
			state_machine();

			// Sleep 100 ms
			usleep(SLEEP_100_MS);

		}while(iStopCommand != 1);

		// close Thread Read Command
		pthread_join(threadId_ReadCommand, NULL);

		// close Thread Send Data
		pthread_join(thread_Send_Data, NULL);

		// Close Thread Read Sensor
		//pthread_join(threadId_Sensor, NULL);      NO SENSOR AVAIBLE

		// Close Socket Data
		close_socket(socketData, newSockData);

		// Close socket video
		if(bVideo == VIDEO_OK){
			close_socket(socketVideo, newSockVideo);
			bVideo = 0;
		}
		//printf("\n\n ******* End Main Socket ******* \n\n");

		// Clear Stop Command
		iStopCommand = 0;

	}while(1);

	return NULL;
}

/*
 ============================================
 Function     : state_machine()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
unsigned char state_machine(void){

	// Get Command From Monitoring Applications
	switch(u8Command){

		case SOUND_CMD:
			printf( " SOUND CMD \n");
			Start_Siren();
		break;

		case PICTURE_CMD:
			printf( " PICTURE CMD \n");
			if(bVideo != VIDEO_OK){
				// Socket not yet Create
				socketVideo 	= create_Socket(PORT_VIDEO);
				newSockVideo = accept_client_connection(socketVideo);
				bVideo = VIDEO_OK;
			}

			Take_Picture();
			send_binary(newSockVideo, "/home/debian/Desktop/Intrusion.jpeg");
		break;

		case VIDEO_CMD:
			printf( " VIDEO CMD \n");

		break;

		case REPORT_FILE_CMD:
			printf( " REPORT FILE CMD \n");
			Report_File_To_Supervisor();
		break;

		case TEST_CAPTOR:

		break;

		case DISCONNECT_CMD:
			printf( " DISCONNECT CMD \n");
			iStopCommand = 1;
		break ;

		case STOP_CMD:
			break ;
		default :
			break;
	}

	// Clear Command
	u8Command = NO_CMD;

	return u8Command;
}

/*
 ============================================
 Function     : Thread_Read_Command()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void* Thread_Read_Command(){


	//printf(" ******* Start Thread_Read_Command ******* \n");
	while(u8Command != DISCONNECT_CMD){
		unsigned char bData[SIZE_CMD_MESSAGE]= {0};
		int n =read_socket(newSockData , SIZE_CMD_MESSAGE, bData);
		if(n == SIZE_CMD_MESSAGE){
			u8Command = libcom_cmdAppli(bData);
		}
		// Sleep
		usleep(SLEEP_10_MS)	;
	}
	//printf(" ******* End Thread_Read_Command ******* \n");

	pthread_exit(&ret_thread);

	return NULL;
}

/*
 ============================================
 Function     : Thread_Send_Data_PC()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void* Thread_Send_Data_PC(void * p){

	// Init
	//readSensor();			NO SENSOR AVAIBLE
	//temperature;
	//humidity;

	while(iStopCommand != STOP_THREAD){

		if(iStopthread == STOP_THREAD){
			iStopthread = 0;
			StopThread_Send_Data();
		}

		strInputStatus Mess;
		libcom_InputStatus(&Mess, ptrCaptorMainDoor->stateCapt, ptrCaptorBackDoor->stateCapt, stateInterrupter, temperature,humidity);
		write_socket(newSockData, Mess.sMess, 15);	// Problem with the size of the message

		sleep(SEND_INFO_EACH_SEC);
	}

	pthread_exit(&thread_Send_Data);

	return NULL;
}

/*
 ============================================
 Function     : StopThread_Send_Data()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void StopThread_Send_Data()
{
	//printf(" Stop thread Send Data \n");
	//  Stop Thread Send Data
	pthread_exit(&thread_Send_Data);

}

/*
 ============================================
 Function     : StartThread_Send_Data()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void StartThread_Send_Data()
{
	//printf(" Start thread Send Data \n");
	// Thread Execute Send Data
	pthread_create (&thread_Send_Data, NULL, &Thread_Send_Data_PC, NULL);

}

/*
 ============================================
 Function     : StartThread_ReadSensor()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void StartThread_ReadSensor(){

	// Init Value
	temperature = 0;
	humidity = 0;

	// Thread Execute Read Command
	pthread_create (&threadId_Sensor, NULL, &Thread_ReadSensor, NULL);
}

/*
 ============================================
 Function     : Thread_ReadSensor()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void* Thread_ReadSensor(){
	while(iStopCommand != STOP_THREAD){
//		int iret = readSensor();	NO SENSOR AVAIBLE
//		if(iret != DHT_SUCCESS){	NO SENSOR AVAIBLE
			temperature	= 0;
			humidity = 0;
//		}
		sleep(TICK_READ_SENSOR);
	};

	pthread_exit(&threadId_Sensor);

	return NULL;
}


