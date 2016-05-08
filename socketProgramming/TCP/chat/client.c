#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define MSGSIZE 50
#define NAMESIZE 30 

enum boolean{ false, true };
typedef enum boolean boolean;

char name[NAMESIZE] = "[Anonymous]";
char message[MSGSIZE];

void error(char *message){
	fputs(message, stderr);
	perror(" >> ");
	exit(1);
}

void* def_send(void *arg){

	int sock = *((int*)arg);
	char header[MSGSIZE + NAMESIZE];
	
	while(true){
		fgets(message, MSGSIZE, stdin);
		if(!strcmp(message, "exit\n")){
			close(sock);
			exit(1);
		}
		sprintf(header, "%s:%s", name, message);
		write(sock, header, strlen(header));
	}
	return NULL;
}

void* def_recv(void *arg){
	int sock = *((int*)arg);
	char header[MSGSIZE + NAMESIZE];
	int str_len;
	
	while(true){
		str_len = read(sock, header, MSGSIZE + NAMESIZE-1);
		if(str_len == -1){ 
			return (void*)-1;
		}
		header[str_len] = 0;
		fputs(header, stdout);
	}
	return NULL;
}


int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in server_addr;
	pthread_t send, recv;
	void* thread_return;
	if(argc != 4){
		error("argument error");
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
	error("connected error");
	}
	sprintf(name, "[%s]", argv[3]);	
	pthread_create(&send, NULL, def_send, (void*)&sock);
	pthread_create(&recv, NULL, def_recv, (void*)&sock);
	pthread_join(send, &thread_return);
	pthread_join(recv, &thread_return);
	close(sock);
	return 0;
}
		
	

