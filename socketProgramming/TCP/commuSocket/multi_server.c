#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
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
void zombie_kill(int siganl){
	int status;
	
	pid_t child = waitpid(-1, &status, WNOHANG);
	
	if(WIFEXITED(status)){
		printf("process [ %d ] is dead \n", WEXITSTATUS(status));
	}
}
int main(int argc, char* argv[]){

	pid_t pid;
	int server_socket, client_socket;
	int str_len;

	struct sigaction act;
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
	
	act.sa_handler = zombie_kill;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	sigaction(SIGCHLD, &act, 0);
	
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		error("bind error");

	if(listen(server_socket, 5) == -1)
		error("listen error");
	
	while(1){

	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
	
	if(client_socket == -1){
		error("accept error");
		continue;
	}
	else
		puts("client connectd..");	

	pid = fork();

	if(pid == 0){
		close(server_socket);
		
		while((str_len = read(client_socket, message, BUFFER)) != 0){
			write(client_socket, message, str_len);
		}
		close(client_socket);
		puts("client disconnected..");
		return 0;

	}
	else
		close(client_socket);	

	}
		
	close(server_socket);
	return 1;
}
