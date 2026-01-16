// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <iostream>
// #include <string.h>

// int main(){
//     // 1. 创建管道
//     int fds[2] = { 0 };
//     int ret = pipe(fds);
//     if(ret == -1){
//         std::cerr << "pipe err\n";
//         return 1;
//     }

//     // 2. 创建子进程
//     pid_t pid = fork();
//     if(pid == -1){
//         std::cerr << "fork err\n";
//         return 2;
//     }else if(pid == 0){
//         // 子进程 保留写端但不写入数据
//         ::close(fds[0]);
//         std::cout << "child working, atfter 10s begins to write...\n ";
//         sleep(10);

//         std::string msg = "hello(after 10s)\n";
//         ::write(fds[1], msg.c_str(), msg.size());
//         std::cout << "child had written msg\n";

//         ::close(fds[1]);
//         exit(0);
//     }else{
//         // 父进程读取空管道
//         ::close(fds[1]);

//         char buffer[1024] = { 0 };
//         std::cout << "father: reading pipe(pipe is empty)...\n";

//         // 管道为空&&写端未关闭 read阻塞
//         ssize_t n = ::read(fds[0], buffer, sizeof(buffer) - 1);

//         if(n > 0){
//             buffer[n] = '\0';
//             std::cout << "father: read msg :" << buffer;
//         }
//         ::close(fds[0]);
//         waitpid(pid, nullptr, 0);
//         exit(0);
//     }
//     return 0;
// }


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

int main() {
    int fds[2];
    pipe(fds);
    pid_t pid = fork();

    if (pid == 0) { // 子进程：持续写入（读端已被父进程关闭）
        close(fds[0]);
        char buf[10] = {0};
        std::cout << "子进程尝试写入...\n";
        while (1) {
            write(fds[1], buf, 10); // 核心：读端已关，写入触发SIGPIPE
        }
    } else { // 父进程：立即关闭读端，不读取
        close(fds[0]); // 核心：关闭读端，让管道异常
        close(fds[1]);
        int status;
        waitpid(pid, &status, 0); // 回收子进程，查看终止信号
        std::cout << "子进程终止信号：" << (status & 0x7F) << "\n"; // SIGPIPE=13
    }
    return 0;
}


// #include <unistd.h>
// #include <sys/types.h>
// #include <iostream>

// int main() {
//     int fds[2];
//     pipe(fds);
//     pid_t pid = fork();

//     if (pid == 0) { // 子进程：写入少量数据后，关闭写端
//         close(fds[0]);
//         write(fds[1], "hello", 5);
//         close(fds[1]); // 核心：关闭写端
//         exit(0);
//     } else { // 父进程：循环读取，直到返回0
//         close(fds[1]);
//         char buf[10];
//         ssize_t n;
//         while (1) {
//             n = read(fds[0], buf, sizeof(buf));
//             std::cout << "读取返回值：" << n << "\n"; // 先返回5，再返回0
//             if (n == 0) break; // 读到0（EOF），退出循环
//         }
//         close(fds[0]);
//     }
//     return 0;
// }

// #include <unistd.h>
// #include <sys/types.h>
// #include <iostream>

// int main() {
//     int fds[2];
//     pipe(fds);
//     pid_t pid = fork();

//     if (pid == 0) { // 子进程：持续写入，直到管道满（阻塞）
//         close(fds[0]);
//         char buf[1024] = {0};
//         long long total = 0;
//         std::cout << "开始写入，直到管道满（阻塞）...\n";
//         while (1) {
//             write(fds[1], buf, 1024); // 每次写1KB
//             total += 1024;
//             if (total % 16384 == 0) { // 每16KB打印一次，看进度
//                 std::cout << "已写：" << total/1024 << "KB\n";
//             }
//         }
//         close(fds[1]);
//     } else { // 父进程：不读取，让管道满，3秒后再读取
//         close(fds[1]);
//         sleep(10); // 10秒内不读取，子进程写满管道后阻塞
//         char tmp[65536] = {0};
//         read(fds[0], tmp, 65536); // 读取64KB，释放缓冲区
//         std::cout << "父进程读取数据，子进程可继续写入\n";
//         close(fds[0]);
//     }
//     return 0;
// }