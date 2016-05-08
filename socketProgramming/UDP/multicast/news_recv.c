#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER 1024

void error(const char* str){
	fputs(str, stderr);
	perror(" >> ");
}

int main(int argc, char* argv[]){
	
	int sock, str_len;
	char message[BUFFER];
	struct sockaddr_in recvf_addr;
	struct ip_mreq join_addr;

	if(argc != 3)
		error("arguments");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&recvf_addr, 0, sizeof(recvf_addr));
	
	recvf_addr.sin_family = AF_INET;
	recvf_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	recvf_addr.sin_port = htons(atoi(argv[1]));

	if(bind(sock, (struct sockaddr*)&recvf_addr, sizeof(recvf_addr)) == -1)
		error("bind error");

	join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	join_addr.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_addr, sizeof(join_addr));

	while(1){
		str_len = recvfrom(sock, message, BUFFER-1, 0, NULL, 0 );

		if(str_len < 0)
			break;
		message[str_len] = 0;
		
		fputs(message, stdout);
	}
	close(sock);
	return 0;
} 











	
