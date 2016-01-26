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

void handleClient(int connfd);

int main(int argc, char *argv[])
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_len;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(&client_addr, 0, sizeof(client_addr));

	serv_addr.sin_family = AF_INET; // IPv4 Address
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming address for the machine would do
	serv_addr.sin_port = htons(PORT);

	/* Bind and listen on a socket for incoming connections */
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 20);

	printf("\nServer listening for connections!\n");

	pid_t pid;

	while(1)
	{
		client_len = sizeof(client_addr);
		connfd = accept(listenfd, (struct sockaddr*)&client_addr,
						&client_len);
		printf("\nAccepted a connection from %s:%d\n",
				inet_ntoa(client_addr.sin_addr),
				(int) ntohs(client_addr.sin_port));

		pid = fork();

		if (pid > 0) {
			/* parent */
			close(connfd);
			continue;
		}

		if (pid == 0) {
			/* child */
			close(listenfd);
			handleClient(connfd);
			break;
		}
	}
	return 0;
}

void handleClient(int connfd)
{
	int readbytes = 0;
	struct payload p;
	char filename[20];
	FILE *outfile;
	char recvBuff[BUFFSIZE];
	memset(recvBuff, 0, sizeof(recvBuff));

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
	return;
}
