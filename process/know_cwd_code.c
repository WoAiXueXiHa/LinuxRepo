#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  FILE* fptr = fopen("new.txt","w");
  if(fptr == NULL){
    perror("fopen err");
  }

  pid_t id = getpid();
  while(1){
    printf("我是一个进程，我的pid: %d\n",id);
    sleep(1);
  }

  return 0;
}
