#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
  pid_t id = fork();
  if(id == 0){
    // 子进程
    int count = 10;
    while(count--){
      printf("I am child, pid:%d, ppid:%d\n",getpid(),getppid());
      sleep(1);
    }
    exit(0);
  }

  // 父进程
  int status = 0;
  pid_t ret = waitpid(id, &status, 0);
  if(ret >= 0){ // 等待成功
    printf("wait successfully\n");
    if(WIFEXITED(status)){    // 正常返回
      printf("exit code: %d\n",WEXITSTATUS(status));
    }else{  // 异常返回 被信号杀死
      printf("killed by signal, %d\n",status & 0xF);
    }
  }
  sleep(3);

  return 0;

}

