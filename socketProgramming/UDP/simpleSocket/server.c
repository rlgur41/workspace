#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER 30

void error(const char* str){

	fputs(str, stderr);
	perror(" >> ");
}

int main(int argc, char* argv[]){

	int server_socket;
	char message[BUFFER];
	int str_len;
	socklen_t client_addr_size;

	struct sockaddr_in server_addr, client_addr;

	if(argc != 2){
		error("argument error");
		exit(1);
	}

	server_socket = socket(PF_INET, SOCK_DGRAM, 0);
	if(server_socket == -1)
		error("socket configuration error");
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	error("bind error");

	while(1){

		client_addr_size = sizeof(client_addr);
		str_len = recvfrom(server_socket, message, BUFFER, 0, (struct sockaddr*)&client_addr, &client_addr_size);

		sendto(server_socket, message, str_len, 0, (struct sockaddr*)&client_addr, client_addr_size);
	}

	close(server_socket);
	return 0;


}
