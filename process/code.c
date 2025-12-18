#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  int cnt = 1;
  while(cnt <= 1000){
    printf("The process is running! My pid is: %d,  The sceond is : %d\n",getpid(), cnt++);
    sleep(1);
  }

  return 0;
}
