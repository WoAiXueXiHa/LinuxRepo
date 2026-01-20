#include <iostream>
#include <string>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    // 1. 创建管道
    int fds[2] = { 0 };
    int pipeRet = pipe(fds);
    if(pipeRet == -1){
        std::cerr << " pipe err\n";
        return 1;
    }

    // 2. 创建子进程
    // 子进程写入 父进程读取
    pid_t pid = fork();
    if(pid == -1){
        std::cerr << " fork err\n";
        return 2;
    }else if(pid == 0){
        // 子进程
        ::close(fds[0]);

        int total = 0;
        int cnt = 5;
        std::string msg = "hello";
        while(cnt--){
            total += ::write(fds[1], msg.c_str(), msg.size());
            std::cout << "child: " << getpid() << ",write: " << msg << std::endl;
            std::cout << "write size: " << total << std::endl;
        }
        ::close(fds[1]);    // 写完了 关闭写端正常退出
        std::cout << "child write over, quit...\n";
        exit(0);
    }else{
        // 父进程 读取
        ::close(fds[1]);
        char buf[1024] = { 0 };
        while(1){
            ssize_t n = ::read(fds[0], buf, sizeof(buf) - 1);
            if(n > 0){
                buf[n] = '\0';
                std::cout << "father: " << getpid() << "read: " << buf << std::endl;
            }else if(n == 0){
            // 读到文件结尾
                std::cout << "father: " << getpid() << "read until the end of file... quit...\n";
                break;
            }else{
                std::cerr << "read err\n";
                break;
            }
        }
        ::close(fds[0]);

        // 进程等待
        int status = 0;
        pid_t waitRet = waitpid(pid, &status, 0);
        if(waitRet > 0){
            std::cout << "father wait success, child pid: " << pid 
                      << " exit code: " << ((status << 8) & 0xFF)
                      << " exit singal: " << (status & 0x7F) << std::endl;
        }

        exit(0);
    }

    return 0;
}
