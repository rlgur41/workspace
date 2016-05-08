#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void* thread_main(void *argument){

	int i;
	int* cnt = argument;
	char* msg = (char*)malloc(sizeof(char)*20);
	strcpy(msg, "thread running\n");
	

	for(i = 0; i < *cnt; i++){
		sleep(1);
		puts("working....");
	}
	*cnt = 1;
	return (void*)msg;
}


int main(){

	pthread_t t_id;
	void* ret;
	int thread_param = 5;

	if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0){
		puts("pthread_creat() error ");
		return -1;
	}

	if(pthread_join(t_id, &ret) != 0){
		puts("pthread_join() error");
		return -2;
	}
	 
	printf("%d\n", thread_param);
	printf("join message >> %s\n", (char*)ret);	
	free(ret);
	return 0;

}
