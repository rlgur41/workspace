#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUFFER 30


/* int select(int maxfd, fd_set *readset, fd_set *writeset,
              fd_set *exceptset, const struct timeval *timeout);

        if it works, return over 0, besides, return -1

                                                        */

int main(){

        fd_set dread, temp;
        int rst, str_len;
        char message[BUFFER];
        struct timeval time;

        FD_ZERO(&dread);
        FD_SET(0, &dread);


        while(1){
                temp = dread;

                time.tv_sec = 5;
                time.tv_usec = 5000;

                rst = select(1, &temp, 0, 0, &time);

                switch(rst){

                        case 0:
                                puts("select avaliable");
                                break;
                        case -1:
                                puts("TIME OUT");
                                break;
                        default:
                                if(FD_ISSET(0, &temp)){
                                        str_len = read(0, message, BUFFER);
                                        message[str_len] = 0;
                                        printf("message >> %s\n", message);
                        }
                }
        }
}
              
