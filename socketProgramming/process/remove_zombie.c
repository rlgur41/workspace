#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void conf_child(int signal){
	int status;
	pid_t child = waitpid(-1, &status, WNOHANG);

	if(WIFEXITED(status)){
		printf("child process dead.. ( id : %d)\n", child);
		printf("child status : %d\n", WEXITSTATUS(status));
	}
}

int main(){
	int i;
	pid_t pid;
	
	struct sigaction act;
	act.sa_handler = conf_child;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	sigaction(SIGCHLD, &act, 0);
	
	pid = fork();

	if(pid == 0){
		puts(" [child] sleep 10 sec :");
		sleep(10);
		return 10;
	}
	else{
		printf(" [parent] child process id >> %d\n", pid);

		pid = fork();
	
		if(pid == 0){
			puts(" [child] sleep 10 sec :");
			sleep(10);
			exit(20);
		}
		else{
			
			printf(" [parent] child process id >> %d\n", pid);
			
			for(i = 0; i < 5; i++){
				puts("wait...");
				sleep(5);
			}
		}
	}
	return 0;
}
	

	
	
