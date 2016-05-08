#include <stdio.h>
#include <pthread.h>


pthread_mutex_t mutex;

void * thread_inc(void * arg){
	int *value = (int*)arg;	
	int i;

	for(i = 0; i < 1000; i++){
		pthread_mutex_lock(&mutex);
		*value += 1;
		printf("value inc >> %d\n", *value);
		pthread_mutex_unlock(&mutex);
	}

	return NULL;

}

void* thread_dec(void * arg){
	int *value = (int*)arg;
	int i;

	for(i = 0; i < 1000; i++){
		pthread_mutex_lock(&mutex);
		*value -= 1;
		printf("value dec >> %d\n", *value);
		pthread_mutex_unlock(&mutex);
	}
	
	return NULL;
		
}


int main(){
	int share = 0;
	pthread_t thread_id[2];
	
	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&thread_id[0], NULL, thread_inc, (void*)&share);  
	pthread_create(&thread_id[1], NULL, thread_dec, (void*)&share); 

	puts("join enter");	
	pthread_join(thread_id[0], NULL);
	puts("inc end dec start");
	pthread_join(thread_id[1], NULL);
	puts("join end");
 	pthread_mutex_destroy(&mutex);
	
	return 1;
}
