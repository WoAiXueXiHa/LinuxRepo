#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <functional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using task_t = std::function<void()>;

class TaskManager{
public:
    TaskManager(){
        srand(time(nullptr));
        // [](){}
        tasks.push_back([](){
            std::cout << "process " << getpid() << " task1\n";
        });
        tasks.push_back([](){
            std::cout << "process " << getpid() << " task2\n";
        });
        tasks.push_back([](){
            std::cout << "process " << getpid() << " task3\n";
        });
    }

    // 选择任务
    int SelectTask() { return rand() % tasks.size(); }
    void Excute(unsigned int number) { tasks[number](); }
private:
    std::vector<task_t> tasks;      // 任务列表
};

TaskManager tm;

void Worker(){
    int cmd = 0;
    while(1){
        ssize_t n = ::read(0, &cmd, sizeof(cmd));
        if(n == sizeof(cmd)) tm.Excute(cmd);
        else if(n == 0){
            std::cout << "child: " << getpid() << " quit...\n";
            break;
        }else{
            std::cerr << "read err" << std::endl;
            break;
        }
    }
}