#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  pid_t id = fork();

  if(id == 0){
    printf("我是子进程,pid: %d, ppid: %d\n",getpid(),getppid());
  }else{
    printf("我是父进程,pid: %d, ppid: %d\n",getpid(),getppid());
  }

  sleep(1000000);
  return 0;
}
