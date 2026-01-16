#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    // 1. 创建管道
    int fds[2] = { 0 };
    int ret = pipe(fds);
    if(ret == -1){
        std::cerr << "pipe err\n";
        return 1;
    }

    // 2. 创建子进程
    pid_t pid = fork();
    if(pid == -1){
        std::cerr << "fork err\n";
        return 2;
    }else if(pid == 0){
        // 子进程写入

        // 先关闭读端
        ::close(fds[0]);

        std::string msg = "hello\n";
        int cnt = 5;
        int total = 0;
        
        while(cnt--){
            total += ::write(fds[1], msg.c_str(), msg.size());
            std::cout << "child: write size: " << total << std::endl;
            sleep(1);
        }
        std::cout << "child: write 10 times, quit now\n";


        // 写完了 关闭写端 正常退出
        ::close(fds[1]);
        exit(0);
    }else{
        // 父进程读取

        // 先关闭写端
        ::close(fds[1]);
        
        char buffer[1024] = { 0 };
        
        while(1){
            ssize_t n = ::read(fds[0], buffer, sizeof(buffer) - 1);     // 留一个字节给\0
            if(n > 0){
                buffer[n] = '\0';
                std::cout << "father: read from child msg: " << buffer << std::endl;
            }else if(n == 0){
                // 读取完成 子进程写端已经关闭
                std::cout << "n: " << n <<std::endl;
                std::cout << " father: read over, child quit, me too\n";
                break;
            }else{
                std::cout << "father: read err\n";
                break;
            }
        }
        // 读完了 关闭读端
        ::close(fds[0]);
        std::cout <<"\n";

        // 回收子进程
        int status = 0;
        pid_t rid = waitpid(pid, &status, 0);
        if(rid == -1){
            std::cerr << "father: wait err\n";
            return 3;
        }else{
            std::cout << "father wait succes, child pid: " << pid
                      << " exit code :" <<((status << 8) & 0xFF)
                      << " exit signal:" << (status & 0x7F) << std::endl;
        }
    }
    return 0;
}