#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER 128

static char from_kernel[BUFFER] = { 0 };

int main(int argc, char *argv[]){

        int fd;
        int rst;

        if((fd = open("/dev/md5char", O_RDWR)) < 0){
                printf("Can't open file\n");
                return 0;
        }

        printf("\nMessage that will send to Kernel level : %s\n", argv[1]);

        rst = write(fd, argv[1], strlen(argv[1]));
        if(rst  < 0){
                printf("write error\n");
                return 0;
        }

        puts("Starting receive message");

        rst = read(fd, from_kernel, BUFFER);
        if(rst < 0){
                printf("read error\n");
                return 0;
        }

        printf("\nreceived string : %s\n", from_kernel);
        puts("--------------------------------------------");
        return 1;



}
