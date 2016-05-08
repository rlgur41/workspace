#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER 30

void error(const char* message){

	fputs(message, stderr);
	perror(" >> ");
}

int main(int argc, char* argv[]){

	int sock;
 	char msg1[] = "Hello";
 	char msg2[] = "My name is";
 	char msg3[] = "UDP";
	socklen_t your_addr_size;
	struct sockaddr_in your_addr;

	if(argc != 3){
		error("argument error !");
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error("socket configuration error");

	memset(&your_addr, 0, sizeof(your_addr));
	your_addr.sin_family = AF_INET;
	your_addr.sin_addr.s_addr = inet_addr(argv[1]);
	your_addr.sin_port = htons(atoi(argv[2]));

	sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&your_addr, sizeof(your_addr));
	sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&your_addr, sizeof(your_addr));
	sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&your_addr, sizeof(your_addr));
	close(sock);
	return 0;

} 

	
	
