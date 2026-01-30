#include "LogInfo.hpp" // 引入你写的日志头文件
#include <thread>
#include <vector>
#include <atomic>

using namespace LogTxt;

// ==========================================
// 场景 1: 基础功能测试
// ==========================================
void TestBasic()
{
    std::cout << "=== 正在进行基础功能测试 ===" << std::endl;
    // 默认应该是控制台输出
    LOG(LogLevel::INFO) << "这是基础测试信息: " << 3.14159 << " Hello World";
    LOG(LogLevel::WARNING) << "这是一个警告信息";
    LOG(LogLevel::ERROR) << "这是一个错误信息, ID: " << 1001;
    std::cout << "=== 基础测试完成 ===\n" << std::endl;
}

// ==========================================
// 场景 2: 多线程并发压力测试
// 目的：验证是否会出现 "日志交错" (乱序)
// ==========================================
void ThreadTask(int id)
{
    for (int i = 0; i < 5; i++)
    {
        // 疯狂打印
        LOG(LogLevel::INFO) << "线程ID: [" << id << "] 正在执行第 " << i << " 次任务 " 
                            << "Payload: aaaaaaaaaabbbbbbbbbb";
        // 模拟一点点计算耗时
        usleep(1000); 
    }
}

void TestMultiThreadConsole()
{
    std::cout << "=== 正在进行多线程控制台并发测试 (观察是否有乱序) ===" << std::endl;
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++)
    {
        threads.emplace_back(ThreadTask, i);
    }

    for (auto &t : threads)
    {
        t.join();
    }
    std::cout << "=== 多线程测试完成 ===\n" << std::endl;
}

// ==========================================
// 场景 3: 文件策略与目录自动创建测试
// ==========================================
void TestFileLogStrategy()
{
    std::cout << "=== 正在进行文件日志测试 ===" << std::endl;
    
    // 1. 切换到文件策略
    // 预期：在当前目录下生成 ./log/log.txt
    ENABLE_FILE_LOG(); 

    LOG(LogLevel::INFO) << "这条消息应该出现在文件中，而不是屏幕上";
    LOG(LogLevel::FATAL) << "系统即将崩溃(模拟)";

    // 再次并发写入文件，验证文件写的原子性
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++)
    {
        threads.emplace_back([](int id){
            LOG(LogLevel::INFO) << "文件写入并发测试 线程:" << id;
        }, i);
    }
    for (auto &t : threads) t.join();

    std::cout << "=== 文件测试完成，请检查 ./log/log.txt 内容 ===" << std::endl;
    
    // 切回控制台，告诉用户结束了
    ENABLE_CONSOLE_LOG();
    LOG(LogLevel::INFO) << "测试全部结束，ByeBye!";
}

