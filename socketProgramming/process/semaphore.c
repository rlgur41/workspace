#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 5
sem_t sem1, sem2;
int num = 0;

void* def_input(void* arg){
	int i;
	for(i = 0; i < SIZE; i++){
		sem_wait(&sem2);
		printf("Input number >> ");
		scanf("%d", &num);
		sem_post(&sem1);
	}
	return NULL;
}

void* def_cal(void* arg){
	int i;
	int rst = 0;

	for(i = 0; i < SIZE; i++){
		sem_wait(&sem1);
		rst+=num;
		sem_post(&sem2);
	}
	
	printf("\nresult = %d\n", rst);
	return NULL;
}
int main(){

	pthread_t id_1, id_2;

	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 1);

	pthread_create(&id_1, NULL, def_input, NULL);
	pthread_create(&id_2, NULL, def_cal, NULL); 
	
	pthread_join(id_1, NULL);
	pthread_join(id_2, NULL);

	sem_destroy(&sem1);
	sem_destroy(&sem2);
	return 0;


}
