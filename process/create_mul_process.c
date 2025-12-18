#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
  for(int i = 0; i < 10; i++){
    pid_t id = fork();
    if(id == 0){
      printf("子进程PID: %d,PPID: %d, 序号：%d\n",getpid(),getppid(),i + 1);
      sleep(2);
      return 0; // 子进程退出
    }
  }
    sleep(5); // 让子进程跑完
    printf("父进程:%d,所有子进程创建完毕\n",getpid());

    return 0;
}
