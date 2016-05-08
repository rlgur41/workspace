#include <unistd.h>
#include <stdio.h>
#include <signal.h>
// void (*signal(int signo, void(*func)(int)))(int)

void timeout(int signal){

	if(signal == SIGALRM)
		puts("Time out requested");

//	alarm(2);
}
void keycontrol(int signal){

	if(signal == SIGINT)
		puts("ctrl c was inputed");
}
int main(){

	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);

	for(i = 0; i < 3; i++){
		puts("wait...");
			sleep(100);
		}
	return 0;
}

