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
	char message[BUFFER];
	int str_len, i;
	socklen_t addr_size;

	struct sockaddr_in my_addr, your_addr;

	if(argc != 2){
		error("argument error !");
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error("socket configuration error");

	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(atoi(argv[1]));

	if(bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1)
		error("bind error");

	for(i = 0; i < 3; i++){
		sleep(5);
		addr_size = sizeof(your_addr);
		str_len = recvfrom(sock, message, BUFFER, 0, (struct sockaddr*)&your_addr, &addr_size);

		printf("Message[%d] >> %s\n ", i, message);
	}

	close(sock);
	return 0;

} 

	
	
