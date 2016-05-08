#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUFFER 30

int main(){
	
	int fd[2];
	const char *str = "Hello world!\n";
	char message[BUFFER];
	pid_t pid;

	pipe(fd);
	pid = fork();
	if(pid == 0)
		write(fd[1], str, strlen(str));
	else
		read(fd[0], message, BUFFER);
		puts(message);
	return 1;
}
