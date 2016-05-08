#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUFFER 30

void error(const char *message){
	
	fputs(message, stderr);
	perror(">> ");

}

int main(int argc, char* argv[]){

	int sock;
	struct sockaddr_in multi_addr;
	int ttl = TTL;

	FILE *fp;

	char message[BUFFER];

	if(argc != 3)
		error("argument");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&multi_addr, 0, sizeof(multi_addr));

	multi_addr.sin_family = AF_INET;
	multi_addr.sin_addr.s_addr = inet_addr(argv[1]);
	multi_addr.sin_port = atoi(argv[2]);

	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&ttl, sizeof(ttl));

	if((fp = fopen("news.txt", "r")) == NULL)
		error("file error");
	while(!feof(fp)){
		fgets(message, BUFFER, fp);
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&multi_addr, sizeof(multi_addr)); 
	
	sleep(2);
	}
	
	close(sock);
	return 0;
}





	
