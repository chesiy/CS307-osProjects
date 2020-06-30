#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
typedef struct{
    int num;
    int *arr;
}parameters;
void *sortit(void *param){
    parameters *tmp;
    tmp=(parameters*)param;
    int i,j,t;
    for(i=0;i<tmp->num-1;i++){
        for(j=0;j<tmp->num-i-1;j++){
            if(tmp->arr[j]>tmp->arr[j+1]){
                t=tmp->arr[j+1];
                tmp->arr[j+1]=tmp->arr[j];
                tmp->arr[j]=t;
            }
        }
    }
}
int sort_arr[100];
int total_num;

typedef struct{
    parameters* d1; 
    parameters* d2;
}merge_para;

void *mergeit(void *param){
    merge_para* tmp;
    tmp=(merge_para*)param;
    int i=0,j=0,k=0;
    while(i<tmp->d1->num && j<tmp->d2->num){
        if(tmp->d1->arr[i]<=tmp->d2->arr[j]){
            sort_arr[k++]=tmp->d1->arr[i++];
        }else{
            sort_arr[k++]=tmp->d2->arr[j++];
        }
    }
    while(i<tmp->d1->num){
        sort_arr[k++]=tmp->d1->arr[i++];
    }
    while(j<tmp->d2->num){
        sort_arr[k++]=tmp->d2->arr[j++];
    }
}


int main() {
    char space;
    scanf("%d",&total_num);
    for(int i=0;i<total_num;i++){
        scanf("%d",&sort_arr[i]);
        scanf("%c",&space);
    }

    printf("\noriginal array:\n");
    for(int i=0;i<total_num;i++){
	printf("%d ",sort_arr[i]);
    }
    printf("\n");
 
    pthread_t tid1,tid2;
    parameters *data1=(parameters *)malloc(sizeof(parameters));
    parameters *data2=(parameters *)malloc(sizeof(parameters));

    data1->num=total_num/2;
    data2->num=total_num-data1->num;
    data1->arr=(int *)malloc(sizeof(int)*data1->num);
    data2->arr=(int *)malloc(sizeof(int)*data2->num);
   
    for(int i=0;i<data1->num;i++){
        data1->arr[i]=sort_arr[i];
    }
    for(int i=0;i<data2->num;i++){
        data2->arr[i]=sort_arr[i+total_num/2];
    }
   
    pthread_create(&tid1,NULL,sortit,(void*)data1);
    pthread_create(&tid2,NULL,sortit,(void*)data2);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    printf("sorted left half:\n");
    for(int i=0;i<data1->num;i++){
	printf("%d ",data1->arr[i]);
    }
    printf("\n");

    printf("sorted right half:\n");
    for(int i=0;i<data2->num;i++){
	printf("%d ",data2->arr[i]);
    }
    printf("\n");

    pthread_t tid3;
    merge_para *mp=(merge_para *)malloc(sizeof(merge_para));
    mp->d1=data1;
    mp->d2=data2;
    pthread_create(&tid3,NULL,mergeit,(void*)mp);
    pthread_join(tid3,NULL);

    printf("sorted array:\n");
    for(int i=0;i<total_num;i++){
        printf("%d ",sort_arr[i]);
    }
    printf("\n");
    free(data1);
    free(data2);
    free(mp);

    return 0;
}

