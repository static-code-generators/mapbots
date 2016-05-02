#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "payload.h"

#define BUFFSIZE 255

void handleClient(int fd);

int main() {
    char byte;
    int fd = open("/dev/tty.HC-05-DevB", O_RDWR);
    int readbytes = 0, correctread;
    FILE *outfile;
    char recvBuff[BUFFSIZE];
    memset(recvBuff, 0, sizeof(recvBuff));
    outfile = fopen("data.csv", "wb+");

    while ((readbytes=read(fd, recvBuff, sizeof(recvBuff))) > 0) {
        fwrite(recvBuff, sizeof(recvBuff), 1, outfile);
    }

    return 0;
}
