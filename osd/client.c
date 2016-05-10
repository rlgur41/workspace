#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUFFER 128
#define DATA_LENGTH 20
struct student{
	char name[DATA_LENGTH];
	char code[DATA_LENGTH];
};
typedef struct student student;


void def_input(student, int); 
void def_search(int);
void def_print(int);

char header[BUFFER] = { '\0' };

int main(int argc, char *argv[]){

	int sock;
	struct sockaddr_in server_addr;
	int num;
	student s;

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
				def_search(sock);
				break;
			case 3:
				def_print(sock);
				break;
			default:
				close(sock);	
				exit(1);
		}
	}
}
	
void def_input(student s, int sock){
	char str[] = "add";
	write(sock, str, strlen(str)); 
	puts("Input func =========================================");
	fputs("input name (ex:kihyuk) >>", stdout); 
	fgets(s.name, sizeof(s.name),  stdin);
	s.name[strlen(s.name) - 1] = '\0';
	fputs("input code (ex:20155106) >>", stdout);
	fgets(s.code, sizeof(s.code), stdin);
	s.code[strlen(s.code) - 1] = '\0';

	memset(header, 0, sizeof(header));
	sprintf(header, "%s\n%s", s.name, s.code);
	
	write(sock, header, strlen(header));
	
	puts("====================================================");
	

}

void def_search(int sock){
	char msg[] = "search";
	FILE *fp;

	puts("Search func =========================================");
	write(sock, msg, strlen(msg)); 
	
	fputs("Input code what you want to find >> ", stdout);
	fgets(header, sizeof(header), stdin);
	header[strlen(header) - 1] = '\0';
	write(sock, header, strlen(header));
	read(sock, header, sizeof(header)); 

	printf("Match Code(If it does not exist, print 'NOPE') : %s\n", header); 
	puts("====================================================");
}

void def_print(int sock){
	char msg[] = "print"; 
	char *ptr;
	puts("Print func ======================================");
	write(sock, msg, strlen(msg));
 	
	read(sock, header, sizeof(header));
	
	puts("All data print ================================");
	printf("All Data :\n%s", header);
	puts("===============================================");
	puts("================================================");
}

void clearBuffer(){
	while(getchar() != '\n');
}		
	
	
	
