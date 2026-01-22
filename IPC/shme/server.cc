#include <iostream>
#include <unistd.h>
#include <cstring>

#include "ShareMemory.hpp"
#include "Time.hpp"
#include "Fifo.hpp"

int main(){
    std::cout << "server started at: " << GetCurTime() <<std::endl;

    // 1. 资源初始化
    // 创建共享内存
    shm.CreateShm();
    // 挂接
    shm.AttachShm();

    // 2. 建立同步机制
    // 打开管道读端
    // 如果客户端没有打开写端 open会阻塞
    std::cout<< "server is waiting for client to connet...\n";
    gpipe.OpenForRead();
    std::cout<< "client connected. startding communication.\n";

    // 3. 获取数据
    struct data* image = (struct data*)shm.GetAddr();

    // 4. 通信循环
    while(1){
        // a. 等待信号
        // 这里的 gpipe.Wait() 实际上是在读取管道。
        // 如果管道空了，Server 会在这里挂起（阻塞），不消耗CPU。
        gpipe.Wait();

        // b. 读取数据
        printf("--------------------------------\n");
        printf("Status: %s\n", image->status);
        printf("Time:   %s\n", image->lasttime);
        printf("Data:   %s\n", image->image);

        strcpy(image->status, "已处理(过期)");

    }

    // 5. 资源清理
    shm.DetachShm();
    shm.DeleteShm();

    return 0;
}