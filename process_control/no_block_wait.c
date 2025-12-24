#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
  pid_t id = fork();
  if(id < 0){
    perror("fork err\n");
    exit(1);
  }else if(id == 0){  // child
    int count = 5;
    while(count--){
      printf("child runing... pid: %d\n",getpid());
      sleep(3);
    }
    exit(0);
  }else{  // father
    while(1){
      int status = 0;
      pid_t ret = waitpid(id,&status,WNOHANG);
      if(ret > 0){
        printf("wait child successfully\n");
        printf("child exit code:%d\n",WEXITSTATUS(status));
        break;
      }else if(ret == 0){
        printf("father does other things...\n");
        sleep(1);
      }else{
        printf("wait err...\n");
        break;
      }
    }
  
  }

  return 0;
}
