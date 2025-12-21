#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int global = 10;
int main(){
  pid_t ret = fork();
  if(ret < 0){
    perror("fork err\n");
    return 1;
  }else if(ret == 0){
    // 子进程
    while(1){
      printf("I am child process,pid:%d,ppid:%d,global:%d,global pos:%p\n",getpid(),getppid(),global, &global);
      ++global;
      sleep(1);
    }
  }else{
    // 父进程
    while(1){
       printf("I am father process,pid:%d,ppid:%d,global:%d,global pos:%p\n",getpid(),getppid(),global, &global);
       sleep(5);
    } 
  }
  return 0;
}
