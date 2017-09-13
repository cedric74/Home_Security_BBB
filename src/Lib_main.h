/*
 ============================================================================
 Name        : Lib_main.h
 Author      : Cedric T.
 Version     : 1.0
 Date		 : 03/02/2015
 Description :
 ============================================================================
 */
#ifndef _LIB_H
#define _LIB_H

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Functions_Alert/Functions_Alert.h"
#include "Functions_WatchDog/Functions_WatchDog.h"
#include "Functions_State_Machine/Functions_State_Machine.h"

#include "Lib_Detect/Lib_Detect.h"
#include "Lib_Siren/Lib_Siren.h"
#include "Lib_Config/Lib_Config.h"

/*******************************************
*               D E F I N E                *
********************************************/
#define START_FILE			"V1.22, DEBUG, 19 June,  "
#define SIZE_STRING			23

#define DEBUG				1

/*******************************************
*   T Y P E D E F   &  C O N S T A N T E   *
********************************************/
/*******************************************
*	 G L O B A L   V A R I A B L E S  	   *
********************************************/

/*******************************************
*	        F U N C T I O N S   	       *
********************************************/
void 			Init(void);


#endif
