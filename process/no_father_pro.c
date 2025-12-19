#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  pid_t ret = fork();

  if(ret == 0){
    printf("子进程PID:%d,PPID:%d\n",getpid(),getppid());
    printf("子进程不退出\n");
    sleep(7);
    printf("子进程PID:%d,新的PPID:%d\n",getpid(),getppid());
    while(1){ sleep(15);  }
  }else{
    printf("父进程:%d,父进程即将退出\n",getpid());
    sleep(5);
    printf("父进程退出\n");
    return 1;
  }

  return 0;
}


