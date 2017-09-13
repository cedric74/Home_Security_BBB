/*
 * Lib_Com.c
 *
 *  Created on: Mar 25, 2015
 *      Author: cedric.toncanier
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Lib_Com.h"

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
 Function     : libcom_InputStatus()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void libcom_InputStatus(strInputStatus * ptr, unsigned char captor1, unsigned char captor2, unsigned char interrupter, float fTemp,float fHum){

	ptr->bType = INPUTS_STATUS;

	ptr->bInputs =  ((captor1 & 0x1) +((captor2 & 0x1)<<1) + ((interrupter & 0x1) <<2) );
	ptr->fTemp = fTemp;
	ptr->fHum  = fHum;
	ptr->bsize = sizeof(strInputStatus);


	// Calcul Cheksum
	ptr->bChecksum = (unsigned char)( (ptr->bType) + (ptr->bInputs) + fTemp + fHum + ptr->bsize);

	// Format Message
	sprintf(ptr->sMess ,"%1d%1d%2.2f%2.2f%1d%1d", ptr->bType, ptr->bInputs, ptr->fTemp, ptr->fHum, ptr->bsize, ptr->bChecksum);

}

/*
 ============================================
 Function     : libcom_cmdAppli()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
cmdApplication 	libcom_cmdAppli(unsigned char  * cmd){
	strCmdAppli  CmdAppli;
	CmdAppli.bType		= cmd[0];
	CmdAppli.bTypeCmd	= cmd[1];
	CmdAppli.bParam		= cmd[2];
	CmdAppli.bsize		= cmd[3];
	CmdAppli.bChecksum	= cmd[4];

	if(CmdAppli.bType == CMD_APPLI){
		// check if Chksum ok
		unsigned char bchksum = CmdAppli.bType+ CmdAppli.bTypeCmd+ CmdAppli.bParam + CmdAppli.bsize ;
		if(bchksum == CmdAppli.bChecksum){
			return CmdAppli.bTypeCmd;
		}
	}

	return NO_CMD;
}
