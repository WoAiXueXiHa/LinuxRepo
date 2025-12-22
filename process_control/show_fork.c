#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
  printf("before: pid: %d\n",getpid());

  pid_t id = fork();
  if(id < 0){
    perror("fork");
    return 1;
  }else if(id == 0){
    printf("after: child, pid:%d, return val: %d\n",getpid(),id);
  }else{
    printf("after: fatehr, pid:%d, return val: %d\n",getpid(),id);
  }

  return 0;
}
