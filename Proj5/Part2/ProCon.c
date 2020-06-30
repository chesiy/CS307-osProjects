#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

int sleep_time;
int producer_num;
int consumer_num;

buffer_item buffer[BUFFER_SIZE+1];
int buffer_head;
int buffer_tail;

pthread_t producers[1000];
pthread_t consumers[1000];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void init_buffer(){
	pthread_mutex_init(&mutex,NULL);
	sem_init(&empty,0,BUFFER_SIZE);
	sem_init(&full,0,0);
	buffer_head=0;
	buffer_tail=0;
}

void shutdown_buffer(){
	sem_destroy(&full);
	sem_destroy(&empty);
	pthread_mutex_destroy(&mutex);
}

int insert_item(buffer_item item){
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);
	buffer[buffer_tail]=item;
	buffer_tail=(buffer_tail+1)%(BUFFER_SIZE+1);
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	return 0;
}

int remove_item(buffer_item *item){
	sem_wait(&full);
	pthread_mutex_lock(&mutex);
	*item=buffer[buffer_head];
	buffer_head=(buffer_head+1)%(BUFFER_SIZE+1);
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	return 0;
}

void *producer(void *param){
	buffer_item item;
	while(1){
		int st=rand()%4+1;
		sleep(st);
		item=rand();
		if(insert_item(item))
			printf("report error condition\n");
		else
			printf("producer produced %d\n",item);
	}
}

void * consumer(void *param){
	buffer_item item;
	while(1){
		int st=rand()%4+1;
		sleep(st);
		if(remove_item(&item)){
			printf("report error condition\n");
		}else{
			printf("consumer consumed %d\n",item);
		}
	}

}
int main(int argc, char *argv[]){
	//get command line arguments
	if(argc!=4){
		printf("error: wrong argument!\n");
		return 1;
	}
	sleep_time=atoi(argv[1]);
	producer_num=atoi(argv[2]);
	consumer_num=atoi(argv[3]);

	//initialize buffer
	init_buffer();

	//Create producer threads
	for(int i=0;i<producer_num;i++){
		pthread_create(&producers[i],NULL,producer,NULL);
	}

	//Create consumer threads
	for(int i=0;i<consumer_num;i++){
		pthread_create(&consumers[i],NULL,consumer,NULL);
	}
	
	//Sleep
	printf("sleep %d seconds\n",sleep_time);
	sleep(sleep_time);

	//exit
	for(int i=0;i<producer_num;i++){
		pthread_cancel(producers[i]);
		pthread_join(producers[i],NULL);
	}
	for(int i=0;i<consumer_num;i++){
		pthread_cancel(consumers[i]);
		pthread_join(consumers[i],NULL);
	}
	shutdown_buffer();

	return 0;
}
