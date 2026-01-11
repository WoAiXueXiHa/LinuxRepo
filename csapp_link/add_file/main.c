#include <stdio.h>

int g_val1 = 10;
int g_val2;

int add(int a, int b){
    return a + b;
}

int main(){
    static int s_val = 20;
    static int ss_val = 40;
    int num = 30;
    int ret = add(s_val,num);
    printf("ret is: %d\n",ret);

    return 0;

}