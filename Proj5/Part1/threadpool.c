/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

task workqueue[QUEUE_SIZE]; // the work queue
int queue_num=0;

pthread_mutex_t mutex;
sem_t full;


pthread_t bee[NUMBER_OF_THREADS];// the worker bee

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
	pthread_mutex_lock(&mutex);
	if(queue_num==QUEUE_SIZE){
		pthread_mutex_unlock(&mutex);    
		return 1;
	}
	workqueue[queue_num]=t;
	queue_num++;
	pthread_mutex_unlock(&mutex);    
	return 0;
}

// remove a task from the queue
task dequeue() 
{
	task worktodo;
	pthread_mutex_lock(&mutex);
	worktodo=workqueue[0];
	queue_num--;
	for(int i=0;i<queue_num;i++){
		workqueue[i]=workqueue[i+1];
	}	
	pthread_mutex_unlock(&mutex);  		
	return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
	task worktodo;
	while(TRUE){
		sem_wait(&full);
		worktodo=dequeue();
		execute(worktodo.function, worktodo.data);
	}
   	pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
	task submit_task;
    	submit_task.function = somefunction;
   	submit_task.data = p;
   	int error=enqueue(submit_task);
	if(!error)
		sem_post(&full);
    	return error;
}

// initialize the thread pool
void pool_init(void)
{
	pthread_mutex_init(&mutex,NULL);
	sem_init(&full,0,0);
	for(int i=0;i<NUMBER_OF_THREADS;i++)
    		pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
	for(int i=0;i<NUMBER_OF_THREADS;i++){
		pthread_cancel(bee[i]);
		pthread_join(bee[i],NULL);
	}    
	sem_destroy(&full);
	pthread_mutex_destroy(&mutex);
}
