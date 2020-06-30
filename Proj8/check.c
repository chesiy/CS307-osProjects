# include <stdio.h>
# include <string.h>
int main() {
    FILE *out = fopen("myoutput.txt", "r");
    FILE *ans = fopen("correct.txt", "r");
    int accept = 1;
    int ans_vir_addr,out_vir_addr;
    int ans_phy_addr,out_phy_addr;
    int ans_val, out_val, cnt = 0;

    while (~fscanf(ans, "Virtual address: %d Physical address: %d Value: %d\n",
                   &ans_vir_addr,&ans_phy_addr,&ans_val)) {
        if (fscanf(out, "Virtual address: %d Physical address: %d Value: %d\n",
                   &out_vir_addr,&out_phy_addr,&out_val) == EOF) {
            printf("Wrong length!\n");
            accept = 0;
            break;
        }
        if(ans_vir_addr!=out_vir_addr){
            printf("Error! line %d. virtual address\n", cnt);
            accept=0;
        }
        if(ans_phy_addr!=out_phy_addr){
            printf("Error! line %d. physical address\n", cnt);
            accept=0;
        }
        if (ans_val != out_val){
            printf("Error! line %d. value\n", cnt);
            accept = 0;
        }
        cnt++;
    }
    if (accept == 0) printf("Wrong answer.\n");
    else printf("Accept.\n");
    fclose(out);
    fclose(ans);
    return 0;
}
