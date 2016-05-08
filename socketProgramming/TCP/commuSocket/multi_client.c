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
int def_read(int sock, char *buff){

	int str_len;

	while(1){
		if((str_len = read(sock, buff, BUFFER)) != 0){
			buff[str_len] = 0;
			printf("message >> %s", buff);
		}
		else
			return 0;
	}
}

int def_write(int sock, char *buff){

	while(1){
		fgets(buff, BUFFER, stdin);
		if(!strcmp(buff, "q\n")){
			shutdown(sock, SHUT_WR);
			return 1;
		}
		write(sock, buff, strlen(buff));
	}
}			

int main(int argc, char* argv[]){
	pid_t pid;
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
	
	 pid = fork();
	
	if(pid == 0)
		def_write(sock, message);
	else
		def_read(sock, message);

	close(sock);
	
	return 7;
}
