#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "list.h"
#include "task.h"
#include "cpu.h"

struct node *head;
int num=0;
int task_id=0;

int curtime=0;
int pretime=0;
int turnaround=0;
int wait=0;
int response=0;

// add a task to the list 
void add(char *name, int priority, int burst){
	struct task* tmp=malloc(sizeof(struct task));
	tmp->name=name;
	tmp->priority=priority;
	tmp->burst=burst;
	tmp->tid=__sync_fetch_and_add(&task_id,1);
	insert(&head,tmp);
	num++;
}

// invoke the scheduler
void schedule(){
	int tsk_num=num;
	while(num){
		struct node *cur=head;
		while(cur->next!=NULL){
			cur=cur->next;
		}
		run(cur->task,cur->task->burst);
		
		curtime+=cur->task->burst;
		turnaround+=curtime;
		wait+=pretime;
		pretime+=cur->task->burst;

		delete(&head,cur->task);
		free(cur->task);
		num--;
	}
	double avg_turnaround = 1.0 * turnaround / tsk_num;
	double avg_wait = 1.0 * wait / tsk_num;
	double avg_response = avg_wait;
	
	printf("Average Turnaround Time: %.3lf\n", avg_turnaround);
	printf("Average Waiting Time: %.3lf\n", avg_wait);
	printf("Average Response Time: %.3lf\n", avg_response);
}
