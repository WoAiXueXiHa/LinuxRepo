#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
  pid_t id = fork();
  if(id == 0){
    // 子进程
    int count = 6;
    while(count--){
      printf("I am child, pid: %d, ppid: %d\n",getpid(),getppid());
      sleep(1);
    }
    exit(0);
  }

  // 父进程
  int status = 0;
  // pid_t wait(int* status);
  pid_t ret = wait(&status);  // 等待子进程 获取子进程返回码
  if(ret > 0){
    // 等待成功
    printf("successful wait\n");
    if(WIFEXITED(status)){ // 判断是否正常退出
      printf("exit code: %d\n",WEXITSTATUS(status));  // 提取退出码
    }
  }
  sleep(3);

  return 0;
}
