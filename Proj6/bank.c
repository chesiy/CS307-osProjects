#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUSTOMER_NUM 5
#define RESOURCE_NUM 4

/* the available amount of each resource */
int available[RESOURCE_NUM];

/*the maximum demand of each customer */
int maximum[CUSTOMER_NUM][RESOURCE_NUM];

/* the amount currently allocated to each customer */
int allocation[CUSTOMER_NUM][RESOURCE_NUM]={0};

/* the remaining need of each customer */
int need[CUSTOMER_NUM][RESOURCE_NUM];

int request_resources(int cust_num,int request[]){
    int work[RESOURCE_NUM];
    int finish[CUSTOMER_NUM]={0};

    for(int i=0;i<RESOURCE_NUM;i++){
        if(request[i]>need[cust_num][i]){
            printf("error! request more than need!\n");
            return -1;
        }
    }
    for(int i=0;i<RESOURCE_NUM;i++){
        if(request[i]>available[i]){
            printf("error! available resource is not enough!\n");
            return -1;
        }
    }
    //check whether the state is safe
    for(int i=0;i<RESOURCE_NUM;i++){
        work[i]=available[i]-request[i];
        allocation[cust_num][i]+=request[i];
        need[cust_num][i]-=request[i];
    }

    while(1){
	//printf("loop\n");
	/*printf("work\n");
    	for(int k=0;k<RESOURCE_NUM;k++){
            printf("%d ",work[k]);
    	}
    	printf("\n");

	printf("need\n");
	for(int i=0;i<CUSTOMER_NUM;i++){
    	    for(int k=0;k<RESOURCE_NUM;k++){
            	printf("%d ",need[i][k]);
    	    }
    	    printf("\n");
	}*/
        int flag=0;
        for(int i=0;i<CUSTOMER_NUM;i++){
            if(finish[i]==0){
                int j;
                for(j=0;j<RESOURCE_NUM;j++){
                    if(need[i][j]>work[j])
                        break;
                }
                if(j==RESOURCE_NUM){
		    printf("->%d",i);
                    flag=1;
                    for(int k=0;k<RESOURCE_NUM;k++){
                        work[k]+=allocation[i][k];
                    }
                    finish[i]=1;
                    break;
                }
            }
        }
        if(flag==0){
            for(int i=0;i<CUSTOMER_NUM;i++){
                if(finish[i]==0){//unsafe!
                    printf("\nthe state is unsafe!\n");
                    for(int k=0;k<RESOURCE_NUM;k++){
                        allocation[cust_num][k]-=request[k];
                        need[cust_num][k]+=request[k];
                    }
                    return -1;
                }
            }
            //safe!
	    printf("\n");
            for(int i=0;i<RESOURCE_NUM;i++){
                available[i]-=request[i];
            }
            return 0;
        }
    }
}

void release_resources(int cust_num,int release[]){
    for(int i=0;i<RESOURCE_NUM;i++){
        if(release[i]>allocation[cust_num][i]){
            printf("error! release more than allocated!");
            return;
        }
    }
    for(int i=0;i<RESOURCE_NUM;i++){
        allocation[cust_num][i]-=release[i];
        available[i]+=release[i];
        need[cust_num][i]+=release[i];
    }
    printf("the resource has been released!\n");
}

int main(int argc,char*argv[]){

    //get command line arguments
    if(argc!=RESOURCE_NUM+1){
        printf("error: wrong argument!\n");
        return 1;
    }
    for(int i=1;i<argc;i++){
        available[i-1]=atoi(argv[i]);
       // printf("%d\n",available[i-1]);
    }
  
    FILE *in;
    in = fopen("max_request.txt","r");
    char space;
    for(int i=0;i<CUSTOMER_NUM;i++){
        fscanf(in,"%d,%d,%d,%d",&maximum[i][0],&maximum[i][1],&maximum[i][2],&maximum[i][3]);
        fscanf(in,"%c",&space);
    }
    for(int i=0;i<CUSTOMER_NUM;i++){
        for(int j=0;j<RESOURCE_NUM;j++){
            need[i][j]=maximum[i][j];
            //printf("%d ",need[i][j]);
        }
      //  printf("\n");
    }

    while(1){
	printf(">");
        char command[5];
        int cust_num;
        int request[RESOURCE_NUM];
        int release[RESOURCE_NUM];

        scanf("%s",command);

        if(strcmp(command,"RQ")==0){
            scanf("%c",&space);
            scanf("%d ",&cust_num);
          //  printf("cust_num: %d\n",cust_num);
            for(int i=0;i<RESOURCE_NUM;i++){
                scanf("%d",&request[i]);
                scanf("%c",&space);
            }
           /* for(int i=0;i<RESOURCE_NUM;i++){
                printf("%d ",request[i]);
            }
            printf("\n");*/

            int res=request_resources(cust_num,request);
            if(res==0){
                printf("SUCCESS! the request can be satisfied.\n");
            }else{
                printf("FAIL! the request is denyed.\n");
            }
        }else{
            if(strcmp(command,"RL")==0){
                scanf("%c",&space);
                scanf("%d ",&cust_num);
                for(int i=0;i<RESOURCE_NUM;i++){
                    scanf("%d",&release[i]);
                    scanf("%c",&space);
                }
                release_resources(cust_num,release);
            }else{
                if(strcmp(command,"*")==0){
                    printf("available:\n");
                    for(int i=0;i<RESOURCE_NUM;i++){
                        printf("%d ",available[i]);
                    }
                    printf("\n");
                    printf("maximum:\n");
                    for(int i=0;i<CUSTOMER_NUM;i++){
                        for(int j=0;j<RESOURCE_NUM;j++){
                            printf("%d ",maximum[i][j]);
                        }
                        printf("\n");
                    }
                    printf("allocation:\n");
                    for(int i=0;i<CUSTOMER_NUM;i++){
                        for(int j=0;j<RESOURCE_NUM;j++){
                            printf("%d ",allocation[i][j]);
                        }
                        printf("\n");
                    }
                    printf("need:\n");
                    for(int i=0;i<CUSTOMER_NUM;i++){
                        for(int j=0;j<RESOURCE_NUM;j++){
                            printf("%d ",need[i][j]);
                        }
                        printf("\n");
                    }
                }
                else{
                    if(strcmp(command,"exit")==0){
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
