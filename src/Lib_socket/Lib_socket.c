/*
 ============================================================================
 Name        : socket_server.c
 Author      : Cedric T.
 Version     : 1.0
 Date		 : 03/02/2015
 Description :
 ============================================================================
 */

/*******************************************
*			  I N C L U D E 			   *
********************************************/
#include "Lib_socket.h"

/*******************************************
*               D E F I N E                *
********************************************/
#define MODULE_NAME		"socket\0"

#define SIZE_MAX_BUFFER		255

/*******************************************
*	          F U N C T I O N S            *
********************************************/
/*
 ============================================
 Function     : create_Socket()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int create_Socket(int iPort){
	int portno;
	struct sockaddr_in serv_addr;
	int yes = 1;

	// Create Socket Server
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
		error("ERROR opening socket \n ");
		exit(1);
		//return ERROR_SOCKET;
	}

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 	iPort; //PORT_NUM;

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY;
    serv_addr.sin_port =  htons(portno);

    // lose the pesky "Address already in use" error message
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    	error("setsockopt");
    	return ERROR_SOCKET;
      }

	// Bind
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		error("ERROR on binding \n ");
		exit(1);
	}

	//printf("	* Socket Create, port : %d \n" , portno);
	//printf("	* Waiting for client Connection ..... \n");
	// Block Function
	listen(sockfd,5);

	return sockfd;
}

/*
 ============================================
 Function     : accept_client_connection()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int accept_client_connection(int sockfd){
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	//char *some_addr;

	clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0){
		error("ERROR on accept \n ");
		exit(1);
	}
    //printf(" Accept connection From a client:  " );
	//some_addr = inet_ntoa(cli_addr.sin_addr); // return the IP
	// Print IP CLient
    //printf(" %s\n", some_addr);

	return newsockfd;
}

/*
 ============================================
 Function     : write_socket()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int write_socket(int  newsockfd, char * sMessage, int iSize){

	int  n = write(newsockfd, sMessage, iSize);

	if (n < 0){
		error("ERROR writing to socket \n ");
		return ERROR_SOCKET;
	}
	return 0;
}

/*
 ============================================
 Function     : read_socket()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
int read_socket(int newsockfd, int iLength , unsigned char * bData){

	int n = read(newsockfd,bData,iLength);
    if (n < 0) {
		error("ERROR reading from socket \n");
		return ERROR_SOCKET;
	}
	return n;
}
/*
 ============================================
 Function     : close_socket()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void close_socket(int sockfd, int newsockfd){
	// Close Socket
	close(newsockfd);
    close(sockfd);
	//printf("	* Close Socket * \n");
}
/*
 ============================================
 Function     : error()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void error(const char *msg)
{
    perror(msg);
    //exit(1);
}

/*
 ============================================
 Function     : send_binary()
 Parameter    :
 Return Value : void
 Description  :
 ============================================
 */
void send_binary(int newsockfd, const char *file_name)
{

    int buff_size = 50240;		//10240;
    char buff[buff_size];
    FILE *pFile;
    size_t result;
    if ( (pFile = fopen(file_name, "rb")) == NULL){
        error("fopen error\n");
    }

    while (( result = fread( buff, 1, buff_size, pFile )) > 0 ) {
        send( newsockfd, buff, result, 0 );
    }

    fclose(pFile);
}



/*
============================================
Function     : Socket_Com_Create
Parameter    :
Return Value :
Description  :
============================================
*/
int Socket_Com_Create_Udp(unsigned char u8Type, unsigned char bDirection, const char* pHostAddress, int iPort, struct sockaddr_in *pServ_Adrr)
{
	// Declarations Variables
    int rc;
    int sockfd=0;

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  //AF_UNIX, AF_INET (TCP: SOCK_STREAM)  (UDP: SOCK_DGRAM)
    if (sockfd < 0){
    	printf("\n%s, Error: opening socket \n", MODULE_NAME);
        return 0;
    }
    printf("\n%s, Create Socket OK\n", MODULE_NAME);


    /* fill in the structure */
    memset(pServ_Adrr,0,sizeof(struct sockaddr_in));
    pServ_Adrr->sin_family=AF_INET;
    pServ_Adrr->sin_port = htons(iPort);

    // Check if Client or Server
    if(bDirection == SERVER_SOCKET){
    	// Server Socket
        pServ_Adrr->sin_addr.s_addr = htonl(INADDR_ANY);

        rc = bind(sockfd, (struct sockaddr *)pServ_Adrr, sizeof(struct sockaddr_in));
        if (rc == -1)
        {
        	printf("\n%s, Error: bind \n", MODULE_NAME);
            close(sockfd);
            return 0;
        }
		printf("\n%s, Bind  Socket OK\n", MODULE_NAME);

    }else{
    	// Client Socket
    	if (inet_aton(pHostAddress, &pServ_Adrr->sin_addr) == 0)
    	{
    		printf("\n%s, Error: Inet_Aton \n", MODULE_NAME);
    		close(sockfd);
    		return 0;
    	}
	}
	return sockfd;
}


/*
============================================
Function     : Socket_Com_Send_Udp
Parameter    :
Return Value :
Description  :
============================================
*/
int  Socket_Com_Send_Udp(int iSock, char * pMsg, int iSize, struct sockaddr_in *pServ_Adrr){
    int iTotalByte = 0;
    int iSlen = sizeof(struct sockaddr_in);

	 //send the message
    iTotalByte= sendto(iSock, pMsg, iSize, 0 , (struct sockaddr *) pServ_Adrr, iSlen);
	if(iTotalByte <=0 || (iTotalByte =! iSize)){
		printf("%s, Error send message", MODULE_NAME);
    	return iTotalByte;
	}
	return iTotalByte;
}


/*
============================================
Function     : Socket_Com_Receive_Udp
Parameter    :
Return Value :
Description  :
============================================
*/
int  Socket_Com_Receive_Udp(int iSock, char * pMsg, int iSize){
	int iTotalByte = 0;
	struct sockaddr_in  si_other;
	int iSlen= sizeof(si_other);

	// Blocking Call
    iTotalByte = recvfrom(iSock, (char*)pMsg, iSize, 0, (struct sockaddr* )&si_other, &iSlen);
    if((iTotalByte < 0)||(iTotalByte >= SIZE_MAX_BUFFER)){
    	iTotalByte =0;
	}
	pMsg[iTotalByte]='\0';
	printf("\n%s, Recv: %s , Total Bytes: %d\n", MODULE_NAME, pMsg, iTotalByte);

	return iTotalByte;
}

/*
============================================
Function     : Socket_Com_Close
Parameter    :
Return Value :
Description  :
============================================
*/
void Socket_Com_Close(int iSocket){
	/* close the socket */
	close(iSocket);
}




