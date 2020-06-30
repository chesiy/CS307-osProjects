#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long init_memory;
typedef struct Block{
    int begin;
    int end;
    int process;
}block;

block blocks[100];
int block_num;

void allocate_mem(int block_id,int request,int proc){
    if((blocks[block_id].end-blocks[block_id].begin+1)>request){
        int end=blocks[block_id].end;
        blocks[block_id].end=blocks[block_id].begin+request-1;
        blocks[block_id].process=proc;
        for(int i=block_num;i>block_id+1;i--){
            blocks[i]=blocks[i-1];
        }
        blocks[block_id+1].process=-1;
        blocks[block_id+1].begin=blocks[block_id].end+1;
        blocks[block_id+1].end=end;
        block_num++;
    }else{
        blocks[block_id].process=proc;
    }
}

int request_mem(int proc,int request,char type){
    if(type=='F'){
        for(int i=0;i<block_num;i++){
            if(blocks[i].process==-1 && (blocks[i].end-blocks[i].begin)>=request){
                allocate_mem(i,request,proc);
                return 0;
            }
        }
        return -1;
    }else{
        if(type=='B'){
            int minhole=init_memory;
            int min_id=-1;
            for(int i=0;i<block_num;i++){
                int size=blocks[i].end-blocks[i].begin+1;
                if(blocks[i].process==-1 && size<minhole && size>=request){
                    minhole=blocks[i].end-blocks[i].begin;
                    min_id=i;
                }
            }
            if(min_id==-1){
                return -1;
            }else{
                allocate_mem(min_id,request,proc);
                return 0;
            }
        }else{
            if(type=='W'){
                int maxhole=0;
                int max_id=-1;
                for(int i=0;i<block_num;i++){
                    int size=blocks[i].end-blocks[i].begin+1;
                    if(blocks[i].process==-1 && size>=request && size>maxhole){
                        maxhole=size;
                        max_id=i;
                    }
                }
                if(max_id==-1){
                    return -1;
                }else {
                    allocate_mem(max_id, request, proc);
                    return 0;
                }
            }else{
                printf("ERROR! invalid type\n");
                return -1;
            }
        }
    }
}
void merge_blocks(int block1,int block2){
    if(block1<block2){
        blocks[block1].end=blocks[block2].end;
        for(int i=block1+1;i<block_num-1;i++){
            blocks[i]=blocks[i+1];
        }
        block_num--;
    }
}

void release_mem(int proc){
    for(int i=0;i<block_num;i++){
        if(blocks[i].process==proc){
            blocks[i].process=-1;
            if(i>0 && blocks[i-1].process==-1){
                merge_blocks(i-1,i);
		if(blocks[i].process==-1){
                    merge_blocks(i-1,i);
                }
            }else{
                if(blocks[i+1].process==-1){
                    merge_blocks(i,i+1);
                }
            }
            break;
        }
    }
}

int compact_mem(){
    block tmp[100];
    int num=0;
    for(int i=0;i<block_num;i++){
        if(blocks[i].process!=-1){
            if(num==0){
                tmp[num].begin=0;
                tmp[num].end=blocks[i].end-blocks[i].begin;
                tmp[num].process=blocks[i].process;
                num++;
            }else{
                tmp[num].begin=tmp[num-1].end+1;
                tmp[num].end=tmp[num].begin+blocks[i].end-blocks[i].begin;
                tmp[num].process=blocks[i].process;
                num++;
            }
        }
    }
    if(tmp[num-1].end<init_memory){
        tmp[num].begin=tmp[num-1].end+1;
        tmp[num].end=init_memory-1;
        tmp[num].process=-1;
        num++;
    }
    block_num=num;
    for(int i=0;i<num;i++){
        blocks[i]=tmp[i];
    }
}

void report(){
    printf("-------------------\n");
    for(int i=0;i<block_num;i++){
        if(blocks[i].process==-1){
            printf("Address [%d:%d] Unused\n",blocks[i].begin,blocks[i].end);
        }else{
            printf("Address [%d:%d] Process P%d\n",blocks[i].begin,blocks[i].end,blocks[i].process);
        }
    }
    printf("-------------------\n");
}

int main(int argc,char* argv[]) {
    //get command line arguments
    if(argc!=2){
        printf("error: wrong argument!\n");
        return 1;
    }
    init_memory=atoi(argv[1]);
   
    //initialize
    //init_memory=1048576;
    blocks[0].begin=0;
    blocks[0].end=init_memory-1;
    blocks[0].process=-1;
    block_num=1;

    while(1){
        printf("allocator>");
        char command[5];
        char space;

        scanf("%s",command);
        scanf("%c",&space);
        if(strcmp(command,"RQ")==0){
            int process;
            int req_mem;
            char type;
            scanf("P%d %d %c",&process,&req_mem,&type);
            scanf("%c",&space);
            request_mem(process,req_mem,type);
        }else{
            if(strcmp(command,"RL")==0){
                int process;
                scanf("P%d",&process);
                scanf("%c",&space);
                release_mem(process);
            }else{
                if(strcmp(command,"C")==0){
                    compact_mem();
                }else{
                    if(strcmp(command,"STAT")==0){
                        report();
                    }else{
                        if(strcmp(command,"X")==0){
                            break;
                        }else {
                            printf("ERROR! invalid command!\n");
                        }
                    }
                }
            }
        }
    }
    return 0;
}
