/*
 * Utils.c
 *
 *  Created on: Sep 24, 2017
 *      Author: cedrictoncanier
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Utils.h"

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
*	      L O C A L  F U N C T I O N S      *
********************************************/

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
/*
 ============================================
 Function     : Internet_Connection_Status()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int Internet_Connection_Status(){
	int iret = system("ping -c1 google.com");
	if(iret !=0 ){
		printf("\n No Network Connection \n");
		return FALSE;
	}

	printf("\n Network Connection OK\n\n");
	return TRUE;
}


