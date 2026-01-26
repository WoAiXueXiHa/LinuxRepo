#include <iostream>
#include <unistd.h>
#include "thread_v2.hpp"

using namespace std;
using namespace thread_v2;

// 1. 定义一个上下文结构体 (包含退出标志位)
struct ThreadContext {
    string message;
    bool is_running; // 控制开关
};

// 2. 线程任务函数，接收 ThreadContext*
void worker_task(ThreadContext* ctx)
{
    while (ctx->is_running)
    {
        cout << "V2 线程处理中: " << ctx->message << endl;
        sleep(1);
    }
    cout << "V2 线程: 收到退出指令，清理资源，自然返回..." << endl;
    // 函数结束 -> 等同于 pthread_exit
}

// int main()
// {
//     // 准备数据 (必须保证生命周期长于线程)
//     ThreadContext* ctx = new ThreadContext();
//     ctx->message = "处理高并发订单...";
//     ctx->is_running = true;

//     // 创建线程，传入函数和数据指针
//     // T 被推导为 ThreadContext*
//     thread<ThreadContext*> t(worker_task, ctx);


//     t.Start();
//     cout << "主线程: " << t.getName() << " 已启动" << endl;


//     sleep(3);

//     cout << "--------------------------------" << endl;
//     cout << "主线程: 1. 修改标志位 (通知)" << endl;
//     ctx->is_running = false; 

//     cout << "主线程: 2. 调用 Stop (等待)" << endl;
//     t.Stop(); // 这里其实是在 Join，等待 worker_task 函数 return

//     cout << "主线程: 3. 清理内存" << endl;
//     delete ctx; // 安全删除，因为线程已经确实停止了

//     cout << "主线程: 全部结束" << endl;

//     return 0;
// }