#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(){

	pid_t pid;
	
	gval++;

	pid = fork();

	if(pid == 0)
		gval++;
	else
		gval--;
	
	if(pid == 0)
	printf("child process gval >> %d\n", gval);
	else
	printf("parent process gval >> %d\n", gval);


	
}
