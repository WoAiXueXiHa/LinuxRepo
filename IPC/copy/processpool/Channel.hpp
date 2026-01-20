#pragma once

#include <iostream>
#include <string>
#include <unistd.h>

class Channel{
public:
    // 构造 建立信道
    Channel(int wfd, pid_t who)
        :_wfd(wfd)
        ,_who(who)
        {
            // Channel-fdwho
            _name = "Channel-" + std::to_string(wfd) + "-" + std::to_string(who);
        }

        int getWfd() const { return _wfd; }
        pid_t getWho() const { return _who; }
        std::string getName() const { return _name; }

        // 发送指令->写入管道 父进程调用
        void Send(int cmd) { ::write(_wfd, &cmd, sizeof(cmd)); }
        // 关闭管道
        void Close() { ::close(_wfd); }
        
private:
    int _wfd;           // 写端文件描述符
    pid_t _who;          // 绑定的子进程
    std::string _name;  // 指令名称
};