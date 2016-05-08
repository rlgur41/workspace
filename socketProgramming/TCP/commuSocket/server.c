#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUFFER 1024
void  error(const char* str){
	fputs(str, stderr);
	perror(" >> ");
}
int main(int argc, char* argv[]){


	int server_socket, client_socket;
	int str_len;
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
	
	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
	
	if(client_socket == -1)
		error("accept error");

	while(1){

		fputs("Input text(q to exit) >>", stdout);
		fgets(message, BUFFER, stdin);

		if(!strcmp(message, "Q") || !strcmp(message, "q"))
			break;

		str_len = write(client_socket, message, sizeof(message));
		
		if(str_len <= 0)
			error("write error");
		
		str_len = read(client_socket, message, BUFFER-1);
		if(str_len <= 0)
			error("read error");
		
		printf("message >> %s", message);
		
		
	}

	close(server_socket);
	close(client_socket);
	return 1;
}
