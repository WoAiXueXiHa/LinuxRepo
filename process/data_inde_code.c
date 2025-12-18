#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

// 全局变量（数据段）
int global_val = 100;

int main(){
  // 局部变量（栈段）
  int local_val = 10;

  // fork前：打印初始值，验证代码执行流程
  printf("fork前,全局变量: %d, 局部变量：%d\n",global_val, local_val);

  // 创建子进程
  pid_t ret = fork();
  if(ret < 0){
    perror("fork err");
  }else if(ret == 0){
    // 子进程分支，代码共享，执行相同的代码逻辑
    printf("\n子进程pid:%d, ppid:%d\n",getpid(),getppid());
    // 子进程修改前
    printf("子进程修改前：全局变量：%d, 局部变量：%d\n",global_val,local_val);

    // 子进程修改变量
    global_val = 500;
    local_val = 200;

    // 子进程修改后
    printf("子进程修改后：全局变量：%d, 局部变量：%d\n",global_val,local_val);
    sleep(10);
  }else{
    // 父进程分支
    // 父进程sleep休眠 保证子进程完成变量修改
    sleep(2);
    
    printf("\n父进程pid:%d, ppid: %d\n",getpid(),getppid());

    printf("父进程-子进程修改后：全局变量：%d, 局部变量：%d\n",global_val,local_val);
    sleep(10);
  }
  return 0;
}
