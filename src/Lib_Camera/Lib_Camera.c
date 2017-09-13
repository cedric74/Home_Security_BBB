/*
 * Lib_Camera.c
 *
 *  Created on: Feb 28, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <pthread.h>
#include "Lib_Camera.h"
#include "../Lib_socket/Lib_socket.h"


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

/*******************************************
*               D E F I N E                *
********************************************/
// Port Socket
#define PORT_NUM_CAM		51234
#define HOST_ADDRESS		"192.168.0.100"
#define SIZE_MAX_BUFFER		255

#define TIME_SLEEP_SEC		60 // 1 minute

#define STR_MOTION_ON	"MOTION_ON"
#define STR_MOTION_OFF	"MOTION_OFF"

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
unsigned char bCamMotion;

int 			socketCamera;
int				newSockCamera;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void * Thread_Control_Motion(void * p);
/*
 ============================================
 Function     : Start_Thread_Pic()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Start_Thread_Pic(){
	// Declarations Variables
	pthread_t thread_id;

	// Thread Execute Read Captor
	pthread_create (&thread_id, NULL, &ThreadPic, NULL);
}

/*
 ============================================
 Function     : ThreadPic()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * ThreadPic(){

	Take_Picture();

	return NULL;

}

/*
 ============================================
 Function     : Take_Picture()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Take_Picture(){
	Save_Pic();
	system("ffmpeg -f v4l2 -r 1 -vcodec mjpeg -i /dev/video0 -y -f  image2 \-vcodec mjpeg -vframes 1 /home/debian/Desktop/Intrusion.jpeg");
}

/*
 ============================================
 Function     : Save_Pic()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Save_Pic(){

	char buffer[300];
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime(buffer, 300, "mv /home/debian/Desktop/Intrusion.jpeg	 /home/debian/Desktop/SavedPics/Pic_%F_%H_%M_%S.jpeg", timeinfo);
	printf("Entry : %s \n", buffer);
	system(buffer);
}

/*
 ============================================
 Function     : Start_Thread_Control_Motion()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Start_Thread_Control_Motion(void){
	// Declarations Variables
	pthread_t thread_id;

	// Init Cam Motion State
	bCamMotion = MOTION_OFF;

	printf("Thread_Control_Motion, Start Thread Outside\n");

	// Thread Execute Read Captor
	pthread_create (&thread_id, NULL, &Thread_Control_Motion, NULL);

}


/*
 ============================================
 Function     : Thread_Control_Motion()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void * Thread_Control_Motion(void * p){

	unsigned char bCmdSend = MOTION_OFF;
	char cBuffer[SIZE_MAX_BUFFER]={0};
	struct sockaddr_in servAddress;
	int iSock;

	printf("Thread_Control_Motion, Start Thread Inside\n");
	// Create Socket Server
//	socketCamera 	= create_Socket(PORT_NUM_CAM);
//	newSockCamera = accept_client_connection(socketCamera);
//	if (newSockCamera < 0 ){
//		printf("Thread_Control_Motion, Accept Fail \n");
//		return NULL;
//	}else{
//		printf("Thread_Control_Motion, Accept OK \n");
//	}

	iSock= Socket_Com_Create_Udp(0, CLIENT_SOCKET, HOST_ADDRESS, PORT_NUM_CAM, &servAddress);

	for(;;){

		if(bCamMotion == MOTION_ON){
			// Part Activate Motion
			if(bCmdSend == MOTION_OFF){
				// Send Command Activate
				memset(cBuffer,0, SIZE_MAX_BUFFER);
				strcpy(cBuffer , STR_MOTION_ON);
				int iLen = strlen(STR_MOTION_ON);
				cBuffer[iLen] = '\0';

				//write_socket(newSockCamera, cBuffer, iLen);
				Socket_Com_Send_Udp(iSock, cBuffer, iLen, &servAddress);
				// TODO socket cmd  Activate
				printf("Thread_Control_Motion, Send Cmd Motion ON, str: %s, size: %d\n", cBuffer, iLen);
				bCmdSend = MOTION_ON;
			}


		}else{
			// Part Deactivate Motion
			if(bCmdSend == MOTION_ON){
				// Send Command Deactivate State
				// TODO socket cmd Deactivate
				memset(cBuffer,0, SIZE_MAX_BUFFER);
				strcpy(cBuffer , STR_MOTION_OFF);
				int iLen = strlen(STR_MOTION_OFF);
				cBuffer[iLen] = '\0';

				//write_socket(newSockCamera, cBuffer, iLen);
				Socket_Com_Send_Udp(iSock, cBuffer, iLen, &servAddress);
				printf("Thread_Control_Motion, Send Cmd Motion ON, str: %s, size: %d\n", cBuffer, iLen);
				bCmdSend = MOTION_OFF;
			}
		}
		// Sleep
		sleep(TIME_SLEEP_SEC);
	}
	return NULL;
}

/*
 ============================================
 Function     : Lib_Cam_Active_Motion()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Cam_Active_Motion(){
	bCamMotion =  MOTION_ON;
}

/*
 ============================================
 Function     : Lib_Cam_Deactivate_Motion()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void Lib_Cam_Deactivate_Motion(){
	bCamMotion = MOTION_OFF;
}



