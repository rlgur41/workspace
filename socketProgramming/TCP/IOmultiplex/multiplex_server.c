#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#define BUFFER 1024
void  error(const char* str){
	fputs(str, stderr);
	perror(" >> ");
}

int main(int argc, char* argv[]){
	
	fd_set dread, temp;
	int fd_max, fd_num, i;
	int server_socket, client_socket;
	int str_len;
	
	struct timeval time;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_size;

	char message[BUFFER]; 
	
	if(argc != 2)
		error("you need input IP");

	server_socket = socket(PF_INET, SOCK_STREAM, 0);

	if(server_socket == -1)
		error("configuration socket error");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error("bind error");

	if(listen(server_socket, 5) == -1)
		error("listen error");
	
	FD_ZERO(&dread);
	FD_SET(server_socket, &dread);
	fd_max = server_socket + 1;
	
	while(1){

		time.tv_sec = 5;
		time.tv_usec = 5000;
		temp = dread;
			
		if((fd_num = select(fd_max, &temp, 0, 0, &time)) == -1)
			break;
				
		if(fd_num == 0)
			continue;
			
		for(i = 0; i < fd_max; i++){
			if(FD_ISSET(i, &temp)){
				if(i == server_socket){
					client_addr_size = sizeof(client_addr);
					client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
					FD_SET(client_socket, &dread);

					if(fd_max <= client_socket)
						fd_max = client_socket;

					printf("connected client : %d\n", client_socket);
				}
				else{
					str_len = read(i, message, BUFFER);
					if(str_len == 0){
						FD_CLR(i, &dread);
						close(i);
						
						printf("closed socket : %d\n", i);
					}
					else{
						write(i, message, str_len);
					}
				}
			}
		}
	}
		close(server_socket);
		return 1;
}
