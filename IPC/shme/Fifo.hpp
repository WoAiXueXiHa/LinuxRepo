#include <iostream>
#include "common.hpp"

class Init{
public:
    // 创建管道
    Init(){
        umask(0);
        int n = ::mkfifo(gpipeFile.c_str(), gmode);
        if(n < 0){
            std::cerr << "mkfifo err\n";
            return;
        }
        std::cout << "mkfifo success\n";
    }
    // 释放管道
    ~Init(){
        int n = :: unlink(gpipeFile.c_str());
        if(n < 0){
            std::cerr << "unlink err\n";
            return;
        }
        std::cout << "unlink success\n";
    }
};

Init init;

class Server{
private:
    int _fd;
public:
    Server()
        :_fd(gdefultFd)
        {}
    // 
    bool OpenPipeForRead(){
        _fd = OpenPipe(gForRead);
        if(_fd < 0) return false;
        return true;
    }
    // 输出型参数string* out
    int RecvPipe(std::string* out){
        char buf[gsize];
        ssize_t n = ::read(_fd, buf, sizeof(buf));
        if(n > 0){
            buf[n] = '\0';
            *out = buf;
        }
        return n;
    }

    void ClosePipe(){ ClosePipeHelper(_fd); }

    ~Server(){}
};