/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task {
    char *name;
    int priority;
    int burst;
    int tid;
    int first;	//whether the task is executed the first time;
    int response_time;
} Task;

#endif
