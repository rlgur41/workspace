#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

struct student{
	char name[20];
	char code[20];
};
typedef struct student student;

void clearBuffer();
void def_input(student*, int); 

int main(int argc, char *argv[]){

	int sock;
	struct sockaddr_in server_addr;
	int num;
	student s[10];

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		perror("error : ");


	while(1){
		puts("------------Student Management------------------");
		printf("1. input data\n");
		printf("2. search data\n");
		printf("3. all print\n");
		printf("4. exit\n");
		printf(" >> ");
		scanf("%d", &num);
		getc(stdin);
		switch(num){
			case 1:
				def_input(s, sock);
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				close(sock);	
				exit(1);
		}
	}
}
	
void def_input(student *s, int sock){

	char header[128] = { '\0' };

	clearBuffer();
	fputs("input name (ex:kihyuk) >>", stdout); 
	fgets(s->name, sizeof(s->name),  stdin);
	//	s[i].name[strlen(s[i].name) - 1] = '\0';
	fputs("input code (ex:20155106) >>", stdout);
	fgets(s->code, sizeof(s->code), stdin);
//		s[i].code[strlen(s[i].code) - 1] = '\0';

	
		sprintf(header + strlen(header), "%s\n%s", s->name, s->code);
		printf("%s", header);
	
	
	write(sock, header, strlen(header));
	

}

void clearBuffer(){
	while(getchar() != '\n');
}		
	
	
	
