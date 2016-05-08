#include <stdio.h>
#include <unistd.h>

int main(){


	pid_t pid = fork();

	if(pid == 0)
		puts("It's child process\n");
	else{
		printf("child process ID >> %d \n", pid);
		sleep(30);
	}

	if(pid == 0)
		puts("End child process\n");
	else
		puts("End parent process\n");

	return 0;
}
