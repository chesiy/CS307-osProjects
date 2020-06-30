#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "list.h"
#include "task.h"
#include "cpu.h"

struct node *head[10];
int num[10]={0};
int task_id=0;
int tsk_num=0;

int curtime=0;
int turnaround=0;
int wait=0;
int response=0;
int burst_sum=0;

// add a task to the list 
void add(char *name, int priority, int burst){
	struct task* tmp=malloc(sizeof(struct task));
	tmp->name=name;
	tmp->priority=priority;
	tmp->burst=burst;
	tmp->tid=__sync_fetch_and_add(&task_id,1);
	tmp->response_time=0;
	tmp->first=1;
	insert(&head[priority-1],tmp);
	num[priority-1]++;
	burst_sum+=burst;
	tsk_num++;
}

// invoke the scheduler
void schedule(){
	

	for(int i=9;i>=0;i--){
	//	printf("round%d\n",i);
		struct node *cur=head[i];
		struct node *pre;
		if(cur==NULL){
			continue;
		}
		while(cur->next!=NULL){
			cur=cur->next;
		}
		pre=cur;
		while(num[i]){
			if(cur->task->burst-10>0){
				run(cur->task,10);

				if(cur->task->first==1){
					response+=curtime;
					cur->task->first=0;
				}
				curtime+=10;

				cur->task->burst-=10;
				cur=head[i];
				while(cur->next!=pre&&cur->next!=NULL){
					cur=cur->next;
				}
				pre=cur;
				if(cur==head[i]){
					pre=NULL;
				}
			}else{
				run(cur->task,cur->task->burst);

				if(cur->task->first==1){
					response+=curtime;
					cur->task->first=0;
				}
				curtime+=cur->task->burst;
				turnaround+=curtime;

				cur->task->burst=0;
				pre=head[i];
			//	printf("okkk\n");
				while(pre->next!=cur&&pre->next!=NULL){
					pre=pre->next;
				}
				delete(&head[i],cur->task);
				free(cur->task);
				num[i]--;
				if(num[i]){
					cur=pre;
				}	
			}

		}
	}
	wait=turnaround-burst_sum;
	double avg_turnaround = 1.0 * turnaround / tsk_num;
	double avg_wait = 1.0 * wait / tsk_num;
	double avg_response = 1.0 * response / tsk_num;
	
	printf("Average Turnaround Time: %.3lf\n", avg_turnaround);
	printf("Average Waiting Time: %.3lf\n", avg_wait);
	printf("Average Response Time: %.3lf\n", avg_response);	

}
