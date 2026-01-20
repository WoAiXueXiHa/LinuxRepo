#pragma once 
#include <iostream>
#include "common.hpp"

class Client{
private:
    int _fd;
public:
    Client()
        :_fd(gdefultFd)
        {}

    bool OpenPipeForWrite(){
        _fd = OpenPipe(gForWrite);
        if(_fd < 0) return false;
        return true;
    }

    int SendPipe(const std::string& in){
        return ::write(_fd, in.c_str(), in.size());
    }
    void ClosePipe() { ClosePipeHelper(_fd); }
    ~Client(){}
};