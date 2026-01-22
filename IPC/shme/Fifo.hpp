#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

const std::string gpipeFile = "./fifo";
const mode_t gfifoMode = 0600;
const int gdefultFd = -1;
const int gsize = 1024;
const int gForRead = O_RDONLY;
const int gForWrite = O_WRONLY;

class Fifo{
private:
    int _fd;

    void OpenFifo(int flag){
        // 读端打开文件时，写端还没打开，读端对应的open会阻塞
        _fd = ::open(gpipeFile.c_str(), flag);
        if(_fd < 0){
            std::cerr << "open err\n";
            return;
        }
    }
public:
    Fifo():_fd(gdefultFd){
        umask(0);
        int n = ::mkfifo(gpipeFile.c_str(), gfifoMode);
        if(n < 0){
            std::cerr << "mkfifo err\n";
            return;
        }
        std::cout << "mkfifo success\n";
    }

    bool OpenForWrite(){
        OpenFifo(gForWrite);
        if(_fd < 0) return false;
        return true;
    }
    bool OpenForRead(){
        OpenFifo(gForRead);
        if(_fd < 0) return false;
        return true;
    }

    int Wait(){
        int code = 0;
        // read是阻塞的
        // 管道没有数据 read会让进程进入sleep状态
        // CPU会把当前进程踢出运行队列 执行别的进程 直到管道里有数据为止
        ssize_t n = ::read(_fd, &code, sizeof(code));
        if(n == sizeof(code)) return 0;     // 成功读到 执行
        else if(n == 0) return 1;           // 读到文件结尾 对方下线
        else return 2;                      // 出错
    }

    void Signal(){
        int code = 1;
        ::write(_fd, &code, sizeof(code));
    }

};

Fifo gpipe;