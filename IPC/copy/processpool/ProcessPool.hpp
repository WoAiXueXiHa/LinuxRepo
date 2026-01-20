#pragma once

#include "Task.hpp"
#include "Channel.hpp"

#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

enum STATUS{
    OK = 0,
    USAGEERR,
    PIPEERR,
    FORKERR
};

using work_t = std::function<void()>;

class ProcessPool{
private:
    int _processNum;
    std::vector<Channel> _channel; 
    work_t _work;
public:

    ProcessPool(int processNum, work_t work)
        :_processNum(processNum)
        ,_work(work)
        {}

    // 1. 初始化进程池
    int ProcessInit(){
        // 创建指定个数子进程
        for(int i = 0; i < _processNum; i++){
            // 创建管道
            int fds[2] = { 0 };
            int pipeRet = pipe(fds);
            if(pipeRet < 0){
                std::cerr << "pipe err \n";
                return PIPEERR;
            } 

            // 创建子进程
            pid_t pid = fork();
            if(pid < 0){
                std::cerr << " fork err \n";
                return FORKERR;
            }
            // 建立通信信道
            else if(pid == 0){
                // 子进程 执行任务
                // 关闭历史wfd
                std::cout << getpid() << ", child close history fd: ";
                for(auto& c : _channel){
                    std::cout << c.getWfd() << " ";
                    c.Close();
                }
                std::cout << "over\n";

                // 子进程读取指令 关闭写端
                ::close(fds[1]);

                std::cout << "debug: " << fds[0] << std::endl;
                dup2(fds[0], 0);            // 命令行重定向到管道读端
                _work();                    // 执行命令
                ::exit(0);
            }else{
                //  父进程
                ::close(fds[0]);
                _channel.emplace_back(fds[1], pid);
            }
        }
        return OK;
    }

    // 2. 派发任务
    void DispatchTask(){
        int who = 0;
        int num = 10;
        while(num--){
            // 选择一个任务 
            int task = tm.SelectTask();
            // 选择一个子进程
            Channel& cur = _channel[who++];
            who %= _channel.size();

            std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
            std::cout << "send " << task << " to " << cur.getName()
                      << " , 剩余任务数： " << num << std::endl;
            
            // 派发任务
            cur.Send(task);
            sleep(1);
        }
    }

    // 3. 清理进程池
    void CleanProcessPool(){
        for(auto& c : _channel){
            c.Close();
            pid_t waitRet = waitpid(c.getWho(), nullptr, 0);
            if(waitRet > 0){
                std::cout << "child " << waitRet << " wait success...\n";
            }
        }
    }

};