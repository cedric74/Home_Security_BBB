/*
 * Lib_Camera.c
 *
 *  Created on: Feb 28, 2015
 *      Author: cedric
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Lib_Camera.h"


/*******************************************
*               D E F I N E                *
********************************************/

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
