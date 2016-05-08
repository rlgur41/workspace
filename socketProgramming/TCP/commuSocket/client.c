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

	int sock;
	int str_len, recv_len, recv_cnt = 0;
	struct sockaddr_in server_addr;
	char message[BUFFER];
	if(argc != 3)
		error("argument error");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error("Socket configuration error");
	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;	
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		error("connect error");
		exit(1);
	}
	else
		printf("connecting to the server : %s\n", argv[2]);
 
	while(1){
	memset(message, 0, sizeof(message));
	fputs("Inputs text(q to exit) >>", stdout);
	fgets(message, BUFFER, stdin);
	
	if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		break;

	str_len = write(sock, message, strlen(message));
	
	if(str_len <= 0)
		error("message does not send to server");

	while(recv_cnt < str_len){
		recv_len = read(sock, &message[recv_cnt], sizeof(message) - 1);
		recv_cnt  += recv_len;

		if(recv_len  == -1)
			error("read error");
	}
	message[recv_cnt] = 0;
	printf("message >> %s", message);
	}
	
	close(sock);
	return 1;
}	
