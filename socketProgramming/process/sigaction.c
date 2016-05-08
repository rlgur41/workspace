#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int signal){
	
	if(signal == SIGALRM)
		puts("time out was requested");
	alarm(2);

}

int main(){
	int i;
	struct sigaction act;

	/*int sigaction(int signo, const struct sigaction * act,
			struct sigaction * oldact) */		
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, 0);
	
	alarm(2);

	for(i = 0; i < 3; i++){
		puts("wait..");
		sleep(100);
	}
	return 0;
}
