#pragma once

#include <iostream>
#include <string>
#include <unistd.h>

class Channel{
public:
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

    void Send(int cmd){ ::write(_wfd, &cmd, sizeof(cmd)); }
    void Close() { ::close(_wfd); }

    ~Channel(){}
private:
    int _wfd;
    std::string _name;
    pid_t _who;
};