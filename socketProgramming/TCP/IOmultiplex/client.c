#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define BUFFER 1024
void error(const char* str){

	fputs(str, stderr);
	perror(" >> ");
}

int main(int argc, char* argv[]){

	int sock, str_len;
	struct sockaddr_in server_addr;
	char message[BUFFER];
	if(argc != 3)
		error("argument");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error("Socket configuration error");
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;	
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error("connect error");
	else
		puts("connected..");

	while(1){	
	
	fputs("INPUT >>", stdout);
	fgets(message, BUFFER, stdin);

	write(sock, message, strlen(message));
	
	str_len = read(sock, message, BUFFER - 1);
	
	message[str_len] = 0;

	printf("message >> %s", message);
	}
	close(sock);
	return 1;
}	
