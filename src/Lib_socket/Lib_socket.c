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


