#pragma once

#include <iostream>
#include <string>
#include <unistd.h>

// 信道类 封装管道写端fd 子进程pid
// 作用： 解耦管道fd和子进程的绑定关系
class Channel{
public:
    // 初始化管道写端 子进程pid 
    // _wfd：管道写端fd  _who：绑定的子进程pid
    Channel(int wfd, pid_t who)
        :_wfd(wfd)
        ,_who(who)
        {
            // Channel-3-1234
            _name = "Channel-" + std::to_string(wfd) + "-" + std::to_string(who);
        }

    std::string getName() { return _name; }
    int getWfd() { return _wfd; }
    pid_t getId() { return _who; }
    // 向子进程发送任务指令 父进程调用
    void Send(int cmd){ ::write(_wfd, &cmd, sizeof(cmd)); }
    void Close() { ::close(_wfd); }

    ~Channel(){}
private:
    int _wfd;
    std::string _name;
    pid_t _who;
};