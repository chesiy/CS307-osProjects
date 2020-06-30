/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

#define WorkNum 15

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // create some work to do
    struct data work[WorkNum];
    for(int i=0;i<WorkNum;i++){
	work[i].a=5*i+3;
	work[i].b=8*i-3;
    }

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for(int i=0;i<WorkNum;i++){
	int err=pool_submit(&add,&work[i]);
	if(err){
	    i--;
	    sleep(3);
	}
    }   

    pool_shutdown();

    return 0;
}
