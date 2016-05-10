#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFFER 128
#define DATA_LENGTH 20

//=================================================
//Linked List Init, methods implementation start at 73 ~ 150 Line
struct Node{
	char name[DATA_LENGTH];
	char code[DATA_LENGTH];
	struct Node* next;
};

struct List{
	struct Node *head;
};typedef struct List List;

struct Node* node(List*, int);
struct Node* create_Node(char*, char*);
void addFirst(List*, char*, char*);
void addLast(List*, char*, char*);
void add(List*, int, char*, char*);
void del(List*, int);
struct Node* search(List*, char *);
void print(List*);
void send_print(List*, char*);
//=================================================

//================================================
//Encrypt & Decrypt
void encode(char *);
void decode(char *, int);
//=================================================

int main(int argc, char* argv[]){

	List *list;

	int i = 0, str_len = 0;
	char recv_header[2][DATA_LENGTH];
	char message[BUFFER];
	char *ptr;
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_size;

	struct Node *temp;

	FILE *fp, *fp1;

	list = (List*)malloc(sizeof(list));
	list->head = NULL;

	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(server_socket == -1)
		perror(">> ");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		perror(">> ");

	if(listen(server_socket, 5) == -1)
		perror(">> ");
	
	client_addr_size = sizeof(client_addr);
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
	
	if(client_socket == -1)
		perror(">> ");
	
	fp = fopen("data.txt", "a");
	fp1 = fopen("data.txt", "r");
	
	if(fp1 != NULL){	
		while(fgets(message, BUFFER, fp1) != NULL) {
		
			strcpy(recv_header[0], message);
			decode(recv_header[0], 10);
			recv_header[0][strlen(recv_header[0]) - 1] = '\0';
			fgets(message, BUFFER, fp1);
		

			strcpy(recv_header[1], message);
			decode(recv_header[1], 10);
			recv_header[1][strlen(recv_header[1]) - 1] = '\0';
			addLast(list, recv_header[0], recv_header[1]);
		}
		puts("previous Data ------------------------ ");
		print(list);	
		puts("-------------------------------------");
	}
	

	while(1){		
		i = 0;
		
		memset(message, 0, sizeof(message));
		str_len = read(client_socket, message, sizeof(message) - 1);

		if(str_len <= 0){
			perror(">> ");
			break;
		}
	
		if(!strcmp(message, "add")){
			memset(message, 0, sizeof(message));
			read(client_socket, message, sizeof(message));	
			ptr = strtok(message, "\n");	
			while(ptr != NULL){
				puts(ptr);
				strcpy(recv_header[i], ptr);

				encode(recv_header[i]);
				fprintf(fp, "%s\n", recv_header[i]);
				decode(recv_header[i++], 10);
				
				ptr = strtok(NULL, "\n");
			}
			addLast(list, recv_header[0], recv_header[1]);
			puts("------------------Data were added-----------------------\nrecently data list :\n");
			print(list);
		
			puts("-------------------------------------------------------");
		}
		else if(!strcmp(message, "search")){

			memset(message, 0, sizeof(message));
	
			read(client_socket, message, sizeof(message));
			printf("%d", str_len);
			temp = search(list, message);

			if(temp == NULL)
				strcpy(message, "Nope");
			else
				strcpy(message, temp->name);

			write(client_socket, message, sizeof(message));
		}
		else if(!strcmp(message, "print")){
			memset(message, 0, sizeof(message));

			send_print(list, message);
								
			write(client_socket, message, strlen(message)); 	
		}
	}	

	close(client_socket);
	fclose(fp1);
	fclose(fp);
	close(server_socket);
	
	return 1;
}
void encode(char *str){
	int key = 10; //1010
	
	while(*str){
		*str^=key;
		str++;
	}
	printf("encoded, >> %s\n", str);
}

void decode(char *str, int key){
	
	while(*str){
		*str^=key;
		str++;
	}
	printf("decode, >> %s\n", str);
}
//LinkedList implementation ========================================
struct Node* create_Node(char *name, char *code){
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

	strcpy(newNode->name, name);
	strcpy(newNode->code, code);
	
	return newNode;
}

void addFirst(List *list, char *name, char *code){
	struct Node* newNode = create_Node(name, code);
	
	if(list->head == NULL){
		list->head = newNode;
		newNode->next = NULL;
	}
	else{
		newNode->next = list->head;
		list->head = newNode;
	}
}

void addLast(List *list, char *name, char *code){
	struct Node* temp = list->head;
	struct Node* newNode;
	
	if(list->head == NULL){
		addFirst(list, name, code);
	}
	else{
	        newNode = create_Node(name, code);
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = newNode;
		newNode->next = NULL;
	}
}

void add(List *list, int i, char *name, char *code){
	struct Node *newNode, *temp;
	if(i == 0){
		addFirst(list, name, code);
	}
	else{
		newNode = create_Node(name, code);
		temp = node(list, i-1);
		newNode->next = temp->next;
		temp->next = newNode;
	}
}

void del(List *list, int i){
	struct Node* temp = node(list, i-1);
	struct Node* temp1 = temp->next;
	
	temp->next = temp1->next;
	free(temp1);
}

struct Node* search(List *list, char *code){
	struct Node* temp = list->head;

	while(temp != NULL){
		if(!strcmp(temp->code, code)){
			return temp;
		}
		temp = temp->next;
	}
	return NULL;	 
}

struct Node* node(List *list, int i){
	struct Node* temp = list->head;
	while(i-- > 0){
		temp = temp->next;
	}
	return temp;
}
void send_print(List *list, char *str){
	struct Node* temp = list->head;
	while(temp != NULL){
		strcat(str, temp->name);
		strcat(str, "\n");
		strcat(str, temp->code);
		strcat(str, "\n");

		temp = temp->next;
	}
}
void print(List *list){
	struct Node* temp;
	temp = list->head;
	
	puts("Linked List ----------------");	
	while(temp != NULL){
		printf("name : %s\ncode : %s\n", temp->name, temp->code);
		temp = temp->next;
	}
	puts("----------------------------");
}
//LinkedList implementation end
//=====================================================================

		
