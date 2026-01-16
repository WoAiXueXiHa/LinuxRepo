#include "ProcessPool.hpp"
#include "Task.hpp"

void Usage(std::string proc){
    std::cout << "Usage: " << proc << " process-num\n"; 
}

// main就是master
int main(int argc, char* argv[]){ 
    if(argc != 2){
        Usage(argv[0]);
        return UsageError;
    }

    int num = std::stoi(argv[1]);
    ProcessPool* pp = new ProcessPool(num,Worker);

    // 1. 初始化进程池
    pp->InitProcessPool();

    // 2. 派发任务
    pp->DispatchTask();

    // 3. 退出进程池
    pp->CleanProcessPool();

    delete pp;
    return 0;
}