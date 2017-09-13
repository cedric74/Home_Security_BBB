/*
 * Lib_Config.h
 *
 *  Created on: Apr 28, 2015
 *      Author: Cedric.Toncanier
 */

#ifndef SRC_LIB_CONFIG_LIB_CONFIG_H_
#define SRC_LIB_CONFIG_LIB_CONFIG_H_

/*******************************************
*			  I N C L U D E 			   *
********************************************/

/*******************************************
*               D E F I N E                *
********************************************/
#define 	ERROR			-1
#define 	MAX_USER		 5

#define		MAIN_USER		0

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
typedef struct {
	char		sAddress[50];
	char		sNumPhone[12];
}sUSer;

/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/
sUSer tabUser[MAX_USER];

unsigned char u8NbUSer;

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
int Lib_Config_Load(void);


#endif /* SRC_LIB_CONFIG_LIB_CONFIG_H_ */
