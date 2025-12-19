#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  pid_t child_pid = fork();
  if(child_pid == 0){
    // 子进程：立刻退出，成为僵尸进程
    printf("子进程PID:%d，即将退出\n",getpid());
    return 1;
  }else{
    // 父进程 无限循环 不回收子进程
    printf("父进程PID:%d, 子进程PID:%d\n",getpid(),child_pid);
    printf("子进程成为Z态，请查看状态\n");
    while(1){  }
  }

  return 0;
}
