#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
  pid_t ret = fork(); // 创建子进程 
  if(ret < 0){ // 错误分支
    perror("fork");
    return 1;
  }else if(ret == 0){ // 子进程分支
    printf("I am child process! My pid is : %d, ppid is: %d, ret is: %d\n",getpid(),getppid(),ret);
  }else{
    printf("I am father process! My pid is: %d, ppid is: %d, ret is: %d\n",getpid(),getppid(),ret);
  }
  sleep(1);

  return 0;
}
