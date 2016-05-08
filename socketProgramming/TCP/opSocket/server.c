#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#define BUFFER 1024
void  error(const char* str){
	fputs(str, stderr);
	perror(" >> ");
}

int cal(char op, int count, ...){
	int rst = 0;
	va_list ap;
	int i;

	va_start(ap, count);
	
		if(op  == '+'){	
			for(i = 0; i < count; i++)
				rst += va_arg(ap, int);
			}

		if(op == '*'){
			for(i = 0; i < count; i++)
				rst *= va_arg(ap, int);
			printf("%d", rst);
			}
	
	return rst;

}	

int main(int argc, char* argv[]){


	int server_socket, client_socket;
	int str_len, result;
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
	
	read(client_socket, message, BUFFER);

	switch((int)message[0]){
		case 3:
	result = cal(message[4 * (int)message[0]] + 1, 3, message[4*(int)message[0]] + 1, message[4*(int)message[0]] - 3, message[4*(int)message[0]] - 7);
		break;	
		
		
	}
	write(client_socket, &result, 5);


	close(server_socket);
	close(client_socket);
	return 1;
}
