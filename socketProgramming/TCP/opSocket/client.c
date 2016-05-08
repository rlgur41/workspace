#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define BUFFER 1024
#define INT_SIZE 4
void error(const char* str){

	fputs(str, stderr);
	perror(" >> ");
}

int main(int argc, char* argv[]){

	int sock;
	int op_cnt, result, i;	
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
		printf("connecting to the server : %s\n", argv[1]);
 
	while(1){
	
	fputs("operand count >> ", stdout);
	scanf("%d",  &op_cnt);
	message[0] = (char)op_cnt;

	for(i = 0; i < op_cnt; i++){
	printf("inputed operand count  >> %d\n", op_cnt);
	printf("Operand %d  >> ", i + 1);
	scanf("%d",(int*)&message[INT_SIZE * i + 1]); 
	}

	fgetc(stdin);
	printf("Operand >> ");
	scanf("%c", &message[INT_SIZE * op_cnt + 1] );

	if(write(sock, message, INT_SIZE * op_cnt + 2) <= 0){	
		error("message does not send to server");
	}
	
		
	read(sock, &result, INT_SIZE);

	printf("result >> %d\n", result);
	}
	
	close(sock);
	return 1;
	
}	
