#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <time.h> 

#include "payload.h"

#define PORT 5555
#define BUFFSIZE 512

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, client_addr; 
	socklen_t client_len;
	int readbytes = 0;

    char recvBuff[BUFFSIZE];
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr)); 
	bzero(&client_addr, sizeof(client_addr));
	bzero(recvBuff, sizeof(recvBuff));

    serv_addr.sin_family = AF_INET; // IPv4 Address
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming address for the machine would do
    serv_addr.sin_port = htons(PORT); 

	/* Bind and listen on a socket for incoming connections */
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    listen(listenfd, 20); 

    printf("\nServer listening for connections!\n");
    
	struct payload p;
	char filename[20];
	FILE *outfile;

    while(1)
    {
		client_len = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr,
						&client_len); 
		printf("\nAccepted a connection from %s:%d\n",
				inet_ntoa(client_addr.sin_addr),
				(int) ntohs(client_addr.sin_port));

		while ((readbytes=read(connfd, recvBuff, sizeof(recvBuff))) > 0)
		{
			recvBuff[readbytes] = '\0';

			/* Read the data from buffer into payload */
			sscanf(recvBuff, "%d %f", &p.bot_id, &p.reading);
			/* Open a file for writing the readings */
			sprintf(filename, "bot_%d.csv", p.bot_id);
			printf("Writing to %s\n", filename);
			outfile = fopen(filename, "a");
			/* Write the data */
			fprintf(outfile, "%d,%f\n", p.bot_id, p.reading);
			/* Close file */
			fclose(outfile);
		}

		if(readbytes <= 0)
		{
			if (readbytes == 0) {
				printf("\nClient went away :/");
				close(connfd);
			} else {
				perror("\n Read error \n");
			}
		} 
		printf("\n\n");
    }
	close(listenfd);
	return 0;
}
