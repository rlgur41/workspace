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

	int sock;
	char message[BUFFER];
	int str_len;
	socklen_t addr_size;

	struct sockaddr_in server_addr, from_addr;

	if(argc != 3){
		error("argument error");
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error("socket configuration error");
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	while(1){
		fputs("input message >> (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		
		if(!strcmp(message, "q\n"))
			break;
		
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
		addr_size = sizeof(from_addr);
		str_len = recvfrom(sock, message, BUFFER, 0, (struct sockaddr*)&from_addr, &addr_size);

		message[str_len] = 0;
		printf("message >> %s", message);
	}
	
	close(sock);
	return 0;


}
