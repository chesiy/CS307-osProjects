#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char args_copy[MAX_LINE/2+1][10];	
	char hist_args[MAX_LINE/2+1][10];	//record history
	bool should_pipe=0;
	bool should_wait=0;
	char *pipe_args[MAX_LINE/2+1];
	char space;
	int argindex;
	int pipe_argindex;
	int hist_argindex=0;
    	int should_run = 1;
	pid_t pid;
	
    	while (should_run){   
        	
		for(int i=0;i<MAX_LINE/2+1;i++){
			args[i]=NULL;
			pipe_args[i]=NULL;
		}	
		
		printf("osh>");
        	fflush(stdout);
		wait(NULL);

		argindex=0;
		pipe_argindex=0;
		should_pipe=false;
		should_wait=true;

		while(scanf("%s",args_copy[argindex])){	// read in command
			scanf("%c",&space);
			args[argindex]=args_copy[argindex];
			argindex++;
			if(space=='\n') break;
		}
	/*	for(int i=0;i<argindex;i++){
			printf("%s\n",args[i]);
		}*/
		if(strcmp(args[0],"exit")==0){
			return 0;
		}

		if(strcmp(args[0],"!!")==0){
			if(strlen(hist_args[0])==0){
				printf("No commands in history.\n");
				continue;
			}else{
			//	printf("history!!\n");
				for(int i=0;i<hist_argindex;i++){
					args[i]=hist_args[i];
				}
				argindex=hist_argindex;
			}
		}else{	
		//	printf("not history!!\n");
			for(int i=0;i<argindex;i++){
				strcpy(hist_args[i],args[i]);
			}
			hist_argindex=argindex;
		}

		if(strcmp(args[argindex-1],"&")==0){
			should_wait=false;
			args[argindex-1]=NULL;
			argindex--;
		}

		for (int i=0;i<argindex;i++){
			if(strcmp(args[i],"|")==0){	
				//printf("argindex:%d,i:%d\n",argindex,i);
				should_pipe=true;
				args[i]=NULL;
				for(int j=i+1;j<argindex;j++){
					pipe_args[pipe_argindex]=args[j];
					//printf("%s\t",pipe_args[pipe_argindex]);
					//printf("pipe_args\n");
					args[j]=NULL;
					//printf("%s\t",pipe_args[pipe_argindex]);
					pipe_argindex++;
				}
				argindex=i;
				break;
			}
		}
		
	
		pid=fork();
		if(pid==0){	//child process	

			if(argindex>=3 && strcmp(args[argindex-2],">")==0){
				int fd=open(args[argindex-1],O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR);
				if(fd<0){
					perror("open file failed!");
					exit(1);
				}
				if(dup2(fd, STDOUT_FILENO)<0){
					perror("dup2 failed!");
					exit(1);
				}
				args[argindex - 2] = NULL;
				args[argindex - 1] = NULL;
				argindex -= 2;
				close(fd);
			}else if(argindex>=3 && strcmp(args[argindex-2],"<")==0){
				int fd=open(args[argindex-1],O_RDONLY);
				if(fd<0){
					perror("open file failed!");
					exit(1);
				}
				if(dup2(fd, STDIN_FILENO)<0){
					perror("dup2 failed!");
					exit(1);
				}
				args[argindex - 2] = NULL;
				args[argindex - 1] = NULL;
				argindex -= 2;
				close(fd);
			}
			if(should_pipe){
				int pip_fd[2];				
				pid_t pid1;
				
				pipe(pip_fd);
	
				pid1=fork();
				if(pid1<0){
					perror("pid1 fork failed!");
					exit(1);
				}else{
					if(pid1==0){	
						close(pip_fd[0]);
						dup2(pip_fd[1],STDOUT_FILENO);
						execvp(args[0],args);
						close(pip_fd[1]);					
						//printf("pipe second!!!\n");		
						//printf("%d\n",tmp);
						//printf("%d\n",pipe_argindex);
						/*for(int i=0;i<pipe_argindex;i++){
							printf("%s\t",pipe_args[i]);
						}
						printf("\n");*/
						
						exit(0);
					}else{
						wait(NULL);
						close(pip_fd[1]);
						dup2(pip_fd[0],STDIN_FILENO);
						execvp(pipe_args[0],pipe_args);
						close(pip_fd[0]);
						//printf("pipe first!!!\n");
						
					}
				}
				
			}else{
				execvp(args[0],args);
			}
			exit(0);

		}else{	//father process
			//printf("wait:%d\n",should_wait);
			
			if(should_wait){
				wait(NULL);
			}
		}
		
    	}
    
	return 0;
}
