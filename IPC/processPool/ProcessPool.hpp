#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <functional>
#include "Task.hpp"
#include "Channel.hpp"

using work_t = std::function<void()>;

enum{
    OK = 0,
    UsageError,
    PipeError,
    ForkError
};

class ProcessPool{
public:
    ProcessPool(int n, work_t w)
        :_processNum(n)
        ,_work(w)
        {}

    // _channels 输出型参数
    // work_t work 回调
    int InitProcessPool(){
        // 2. 创建指定个数进程
        for(int i = 0; i < _processNum; i++){
            // 1. 先创建管道
            int pipefd[2] = { 0 };
            int n = pipe(pipefd);
            if(n < 0)   return PipeError;

            // 2. 创建进程
            pid_t pid = fork();
            if(pid == -1) return ForkError;

            // 3. 建立通信信道
            if(pid == 0){
                // 关闭历史wfd
                std::cout << getpid() <<", child close history fd: ";
                for(auto& c: _channels){
                    std::cout << c.getWfd() <<" ";
                    c.Close();
                }
                std::cout << " over" << std::endl;

                ::close(pipefd[1]);

                // 子进程
                std::cout << "debug: " << pipefd[0] << std::endl;
                dup2(pipefd[0], 0);
                _work();
                ::exit(0);
            }

            // 父进程执行
            ::close(pipefd[0]);
            _channels.emplace_back(pipefd[1], pid);

        }
        return OK;
    }

    void DispatchTask(){
        int who = 0;
        // 2. 派发任务
        int num = 20;
        while(num--){
            // a. 选择一个任务
            int task = tm.SelectTask();
            // b. 选择一个子进程channel
            Channel& cur = _channels[who++];
            who %= _channels.size();

            std::cout << "########################" << std::endl;
            std::cout << "send " << task << " to " << cur.getName() 
                      << ", 任务剩余数：" << num << std::endl;
            std::cout << "########################" << std::endl;

            // c. 派发任务
            cur.Send(task);

            sleep(1);
        }
    }

    void CleanProcessPool(){
        // version3
        for(auto& c : _channels){
            c.Close();
            pid_t rid = ::waitpid(c.getId(), nullptr, 0);
            if(rid > 0){
                std::cout << "child " << rid << " wait... success\n";
            }
        }

        // version 2
        // for (auto &c : channels)
        // for(int i = channels.size()-1; i >= 0; i--)
        // {
        //     channels[i].Close();
        //     pid_t rid = ::waitpid(channels[i].Id(), nullptr, 0); // 阻塞了！
        //     if (rid > 0)
        //     {
        //         std::cout << "child " << rid << " wait ... success" << std::endl;
        //     }
        // }

        // version 1
        // for (auto &c : channels)
        // {
        //     c.Close();
        // }
        //?
        // for (auto &c : channels)
        // {
        //     pid_t rid = ::waitpid(c.Id(), nullptr, 0);
        //     if (rid > 0)
        //     {
        //         std::cout << "child " << rid << " wait ... success" << std::endl;
        //     }
        // }
    }

    void DebugPrint(){
        for(auto& c: _channels){
            std::cout << c.getName() << std::endl;
        }
    }
private:
    std::vector<Channel> _channels;
    int _processNum;
    work_t _work;
};