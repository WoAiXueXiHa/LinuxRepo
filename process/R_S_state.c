#include <stdio.h>
#include <unistd.h>

int main(){
  pid_t pid = getpid();
  printf("进程ID: %d\n",pid);
  printf("阶段: R态（死循环，占用CPU）\n");
  /*while(1){
    // 死循环 持续占用CPU
  }
  */
  // 以下代码需要注释掉while循环,演示S态
  
   printf("阶段：S态（sleep 10000秒，可中断等待）\n");
   sleep(10000);
   printf("sleep结束，回到R态\n");

  return 0;

}
