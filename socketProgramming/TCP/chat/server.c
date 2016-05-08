#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>

#define MSGSIZE 50

enum boolean{ false, true };
typedef enum boolean boolean;

int client_cnt = 0;
int client_socks[10]; // MAXIMUM of client
pthread_mutex_t mutex;


void def_send(char * message, int str_len){
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < client_cnt; i++){
		write(client_socks[i], message, str_len);
	}
	pthread_mutex_unlock(&mutex);
}

void * client_set(void * arg){
	char message[MSGSIZE]; 
	int client_sock = *((int*)arg);
	int str_len = 0;
	int i;

	while((str_len = read(client_sock, message, sizeof(message))) != 0)
		def_send(message, str_len);

	pthread_mutex_lock(&mutex);
	for(i = 0; i < client_cnt; i++){
		if(client_socks[i] == client_sock){
			while(i < client_cnt - 1){
				client_socks[i] = client_socks[i+1];
				i++;
			}
			client_cnt--;
			break;
		}	
	}
	pthread_mutex_unlock(&mutex);
	close(client_sock);
	return NULL;
				
}
void error(char *e_message){
	fputs(e_message, stderr);
	perror(">>");
	exit(1);
}

int main(int argc, char* argv[]){
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	int client_size;
	pthread_t t_id;

	if(argc != 2){
		error("argument error");		
	}	
	
	pthread_mutex_init(&mutex, NULL);
	server_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(client_addr)) == -1)
		error("bind()");
	if(listen(server_sock, 5) == -1)
		error("listen()");
	
	while(true){
		client_size = sizeof(client_addr);	
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_size);
		
		pthread_mutex_lock(&mutex);
		client_socks[client_cnt++] = client_sock;
		pthread_mutex_unlock(&mutex);
		
		pthread_create(&t_id, NULL, client_set, (void*)&client_sock);
		pthread_detach(t_id);
		printf("------- guest entered ------\nip : %s", inet_ntoa(client_addr.sin_addr));
	}
	close(server_sock);
	return 1;
}
