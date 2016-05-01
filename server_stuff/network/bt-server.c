#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "payload.h"

#define BUFFSIZE 512

void handleClient(int fd);

int main() {
    char byte;
    int fd = open("/dev/tty.HC-05-DevB", O_RDWR);
    handleClient(fd);
    return 0;
}

void handleClient(int fd)
{
    int readbytes = 0, correctread;
    struct payload p;
    char filename[20];
    FILE *outfile;
    char recvBuff[BUFFSIZE];
    memset(recvBuff, 0, sizeof(recvBuff));

    while ((readbytes=read(fd, recvBuff, sizeof(recvBuff))) > 0) {
        recvBuff[readbytes] = '\0';

        /* Read the data from buffer into payload */
        correctread = sscanf(recvBuff, "%d %f %f %f %f",
                             &p.bot_id, &p.reading,
                             &p.loc.x, &p.loc.y, &p.loc.theta);
        /* If data is in correct format save it into csv */
        if (correctread == 5) {
            /* Open a file for writing the readings */
            sprintf(filename, "bot_%d.csv", p.bot_id);
            printf("Writing to %s\n",
                    filename);
            outfile = fopen(filename, "a");
            /* Write the data */
            fprintf(outfile, "%d,%f,%f,%f,%f\n", p.bot_id, p.reading,
                    p.loc.x, p.loc.y, p.loc.theta);
            /* Close file */
            fclose(outfile);
        /* Else send error message to the client */
        } else {
            char * error_str = "Wrong input detected. Send in format \"\%d \%f \%f \%f \%f\"\n"; 
            write(fd, error_str, strlen(error_str) + 1);
            printf("bad format\n");
        }
    }
}
