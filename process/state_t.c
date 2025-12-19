#include <stdio.h>
#include <unistd.h>

int main(){
  pid_t pid = getpid();
  printf("进程PID:%d\n",pid);
  printf("按ctrl+z暂停进程，变为T态，用fg恢复变为R态\n");

  while(1){
  }

  return 0;
}
