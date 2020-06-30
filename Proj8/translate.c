#include <stdio.h>
#include <stdlib.h>

#define TLBsize 16
#define PageTablesize 256
#define PageNum 256
#define FrameNum 256
#define FrameSize 256

int TLB[TLBsize][3]={0};
char phy_memory[FrameNum*FrameSize];
int pagetable[PageTablesize][2]={0};
//i:pagenumber pagetable[i][0]:valid/invalid pagetable[i][1]:frame number
FILE* store;
FILE* out;
int available_frame=0;
int available_TLB=0;
int TLBhit=0;
int pagefault=0;
int total=0;

void update_TLB(int page_num,int frame_num){
    int victim=available_TLB%TLBsize;
    available_TLB=(available_TLB+1)%TLBsize;
    TLB[victim][0]=1;
    TLB[victim][1]=page_num;
    TLB[victim][2]=frame_num;
}

int search_TLB(int page_num){
    for(int i=0;i<TLBsize;i++){
        if(TLB[i][0]==1 && page_num==TLB[i][1]){   //TLB hit
            TLBhit++;
            //printf("TLB:%d\n\n\n",TLB[i][2]);
            return TLB[i][2];//return frame number
        }
    }
    return -1;  //TLB miss
}
int find_empty_frame() {
    if(available_frame<FrameNum) {
        return available_frame++;
    }
    int victim=(available_frame++)%FrameNum;
    for(int i=0;i<PageNum;i++){
        if(pagetable[i][0]&&pagetable[i][1]==victim){
            pagetable[i][0]=0;
            break;
        }
    }
    return victim;
}
int load_new_page(int pagenumber){
    int off=pagenumber*FrameSize;
    fseek(store,off,SEEK_SET);
    char buffer[FrameSize];
    fread(buffer, sizeof(char),FrameSize,store);

    int framenum=find_empty_frame();
 //   printf("empty frame is found!\n");

    for(int i=0;i<FrameSize;i++){
        phy_memory[framenum*FrameSize+i]=buffer[i];
	//printf("copy is ok!%d\n",i);
    }
    
    return framenum;
}

int get_framenumber(int pagenumber){
    int framenumber=search_TLB(pagenumber);
    if(framenumber!=-1){
      //  printf("TLB hit!\n");
        return framenumber;
    }
    if(pagetable[pagenumber][0]==1){
     //      printf("page is valid!\n");
        update_TLB(pagenumber,pagetable[pagenumber][1]);
        return pagetable[pagenumber][1];
    }else{          //page fault
       //    printf("page fault!\n");
        pagefault++;
        pagetable[pagenumber][0]=1;
        pagetable[pagenumber][1]=load_new_page(pagenumber);
        update_TLB(pagenumber,pagetable[pagenumber][1]);
        return pagetable[pagenumber][1];
    }
}
char getValue(int phy_addr){
    return phy_memory[phy_addr];
}
void print_res(int vir,int phy,char val){
   // fprintf(out,"Virtual address: %d Physical address: %d Value: %d\n",vir,phy,val);
    printf("Virtual address: %d Physical address: %d Value: %d\n",vir,phy,val);
}

int main(int argc,char*argv[]) {
    FILE *in;
    in=fopen(argv[1],"r");
    out=fopen("myoutput.txt","w");
    store=fopen("BACKING_STORE.bin","r");
    char addr[10];
    while(fgets(addr,9,in)!=NULL){
        total++;
        int logi_addr=atoi(addr);
        int offset=0b11111111&logi_addr;
        int page_number=(logi_addr>>8)&0b11111111;
        int frame_number=get_framenumber(page_number);
	//printf("get frame number!\n");
        int phy_addr=frame_number*FrameSize+offset;
        char value=getValue(phy_addr);
        print_res(logi_addr,phy_addr,value);
    }
    printf("page fault rate=%.2f%%\n",(float)pagefault/total*100);
    printf("TLB hit rate=%.2f%%\n",(float)TLBhit/total*100);
    fclose(in);
    fclose(out);
    fclose(store);
    return 0;
}
