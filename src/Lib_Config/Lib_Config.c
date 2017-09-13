/*
 * Lib_Config.c
 *
 *  Created on: Apr 28, 2015
 *      Author: Cedric.Toncanier
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <stdio.h>
#include <stdlib.h>

#include "Lib_Config.h"


/*******************************************
*               D E F I N E                *
********************************************/

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
typedef enum {
	NB_USER 		= 0,
	GET_ADDRESS 	= 1,
	GET_NUM			= 2,
	END				= 3,
}StateConfig;

const char * PATH_CONFIG_FILE = "/home/debian/Desktop/config_file.txt";

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

//TODO, Switch XML FIlE
/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
/*
 ============================================
 Function     : Lib_Config_Load()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int Lib_Config_Load(void){

	// Declarations Variables
	StateConfig confiState = NB_USER;
	int iIndexUser = 0;
	FILE * 		fConfig;
	char * line = NULL;
	size_t len = 0;
	ssize_t sizeLine;

	// Open File
	fConfig = fopen ( PATH_CONFIG_FILE, "r");
	if(fConfig == NULL){
		printf("-Error Open File : %s", PATH_CONFIG_FILE);
		return -1;
	}
	//	Seek to the beginning of the file
	fseek(fConfig, SEEK_SET, 0);

	// Read Line by Line

	while ((sizeLine = getline(&line, &len, fConfig)) != -1) {
		switch(confiState){
			// Get Nb Line
			case NB_USER:
				u8NbUSer =  atoi(line);
				if(fConfig == 0){
					printf("-Error Read Line 1, atoi() issue\n");
					return -1;
				}
				// Change State;
				confiState = GET_ADDRESS;
			break;
			// Get Adress
			case GET_ADDRESS:
				snprintf(tabUser[iIndexUser].sAddress , sizeLine, line);
				tabUser[iIndexUser].sAddress[sizeLine-1] = '\0';

				// Change State;
				confiState = GET_NUM;
			break;
			// Get Num
			case GET_NUM:
				snprintf(tabUser[iIndexUser].sNumPhone , sizeLine, line);
				tabUser[iIndexUser].sNumPhone[sizeLine-1] = '\0';

				if(iIndexUser == (u8NbUSer-1)){
					confiState = END;
				}else{
					// Switch to Next User
					iIndexUser++;

					// Change State;
					confiState = GET_ADDRESS;
				}

			break;
			case END:
			default:
			break;
		}
	}

	printf("Nbe User :%d\n", u8NbUSer);
	int i = 0;
	for( i = 0 ; i < u8NbUSer ; i++){
		printf("	* User %d Address :%s\n", i, tabUser[i].sAddress);
		printf("	* User %d Phone N :%s\n", i, tabUser[i].sNumPhone);
	}
	return 0;
}
