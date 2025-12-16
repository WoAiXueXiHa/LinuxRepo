#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>

void subProcess(){
  while(1){
    std::cout<<"我是子进程 pid: "<<getpid()<<" 我的父进程id: "<<getppid()<<std::endl;
  }
}

int main(){
  pid_t id = getpid();

  vector<pid_t> vecSubProcess;
  for(int i = 0; i < 10; i++){
    subProcess();
    sleep(2);
    vecSubProcess.push_back(id);
  }

  for(auto e : vecSubProcess){

  }

}
