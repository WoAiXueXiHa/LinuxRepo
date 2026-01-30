#include "ThreadPool.hpp"
#include "Task.hpp"
#include <memory>

using namespace ThreadPoolModule;

int main()
{
    // 开启控制台日志
    ENABLE_CONSOLE_LOG();
    // 也可以开启文件日志: ENABLE_FILE_LOG();

    // 创建线程池对象，任务类型是 task_t
    std::unique_ptr<ThreadPool<task_t>> tp = std::make_unique<ThreadPool<task_t>>();
    tp->Start(); // 启动5个线程，它们会阻塞在 Wait 上等待任务

    int cnt = 10;
    char c;
    while (true)
    {
        // 主线程作为【生产者】
        // 输入一个字符，就产生一个任务
        std::cin >> c;
        tp->Equeue(Push); // 把 Push 函数作为任务扔进去
    }

    // 以下代码在死循环后不会执行，但展示了标准流程
    tp->Stop();
    sleep(3);
    tp->Wait();

    return 0;
}