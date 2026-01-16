#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

// typedef 封装函数包装器
using task_t = std::function<void()>;
// 任务管理器 管理所有任务
class TaskManger{
public:
    TaskManger(){
        // 随机分配任务
        srand(time(nullptr));
        // lambda [捕获列表](参数列表)...{函数逻辑}
        // lambda本质是一个对象函数
        tasks.push_back([]()
                        { std::cout << "sub process[" <<getpid() << "]访问数据库]\n" 
                                    << std::endl; });
        tasks.push_back([]()
                        { std::cout << "sub process[" <<getpid() << "]url解析]\n" 
                                    << std::endl; });
        tasks.push_back([]()
                        { std::cout << "sub process[" <<getpid() << "]加密任务]\n" 
                                    << std::endl; });
        tasks.push_back([]()
                        { std::cout << "sub process[" <<getpid() << "]数据持久化]\n" 
                                    << std::endl; });
    }

    int SelectTask(){ return rand() % tasks.size(); }
    void Excute(unsigned long number) { tasks[number](); }

private:
    std::vector<task_t> tasks;
};

TaskManger tm;      // 全局任务管理器

void Worker(){
    // 子进程核心工作函数
    while(1){
        int cmd = 0;
        // 读取任务指令 从管道读端重定向到stdin
        int n = ::read(0, &cmd, sizeof(cmd));
        // 成功读取指令
        if(n == sizeof(cmd)){
            tm.Excute(cmd);
        }
        else if(n == 0){
            // 读端关闭->父进程写端关闭 子进程退出
            std::cout << "pid: " << getpid() <<"quit ...\n";
            break;  
        }
    }
}