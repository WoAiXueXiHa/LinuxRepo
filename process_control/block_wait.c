#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
  pid_t pid = fork();
  if(pid < 0){
    printf("fork err");
    exit(1);
  }
  else if(pid == 0){  // 子进程
    printf("I am child, pid: %d\n",getpid());
    sleep(5);
    exit(257);
  }else{
    int status = 0;
    pid_t ret = waitpid(-1,&status,0);  // 阻塞等待，等待5s
    printf("waiting...\n");
    if(WIFEXITED(status) && ret == pid){
      printf("wait child 5s successfully, child return code: %d\n",WEXITSTATUS(status)); 
    }else{
      printf("wait child failed\n");
      exit(1);
    }
  }
  return 1;
}



