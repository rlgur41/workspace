#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFFER 30

int main(){

	int fd[2]; // fd[0] for receive, fd[1] for send	
	const char *str1 = "hello?";
	const char *str2 = "world";
	
	char buff[BUFFER];

	pipe(fd);
	
	pid_t pid = fork();

	if(pid == 0){
		write(fd[1], str1, strlen(str1));
		sleep(3);
		read(fd[0], buff, BUFFER);
		printf("child message >> %s \n", buff);
	}
	else{
		read(fd[0], buff, BUFFER);
		printf("parent message >> %s \n", buff);	
		write(fd[1], str2, strlen(str2));
		sleep(4);
	}

	return 1;
}


 
