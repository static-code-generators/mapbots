#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 

#include "payload.h"

#define PORT 5555
#define BUFFSIZE 512

int main(int argc, char *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <Server IP address>\n",argv[0]);
        return -1;
    } 
	
    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET; // IPv4 Address
    serv_addr.sin_port = htons(PORT); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) //convert IPv4 and IPv6 addresses from text to binary form
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    } 

	char sendBuff[BUFFSIZE];
	int sentbytes;
	struct payload p;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
	   printf("\nConnection Failed \n");
	   return -1;
	} 
	else {
		printf("\nConnected.\nPlease enter the bot id and reading\n");
	}

	bzero(sendBuff, sizeof(sendBuff));

	/* Read the payload contents from stdin */
	scanf("%d %f", &p.bot_id, &p.reading);	
	/* Write them to the buffer */
	snprintf(sendBuff, sizeof(sendBuff), "%d %f", p.bot_id, p.reading);
	/* Write the buffer to the socket */
	printf("\nSending to server..");
	sentbytes = write(sock, sendBuff, sizeof(sendBuff));

	if (sentbytes < 0) {
		perror("\nError writing to the socket");
	} else {
		printf("\nSuccessfully written to the socket!");
	}
	printf("\nClosing socket\n");
	close(sock);

    return 0;
}
