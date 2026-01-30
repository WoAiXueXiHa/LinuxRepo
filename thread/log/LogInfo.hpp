#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <memory>
#include <time.h>
#include <unistd.h>
#include "GuardLock.hpp"

namespace LogTxt{
    // ----------------------------------------------------
    // 工具函数：获取时间
    // ----------------------------------------------------
    std::string CurTime(){
        time_t time_stamp = ::time(nullptr);
        struct tm cur;
        // localtime返回的是一个指向静态内存的全局指针
        // 多线程环境下，线程A改写了这块内存，线程B读到的是脏数据
        // localtime_r是可重入版本
        // 每个线程用自己的栈空间，互不干扰
        localtime_r(&time_stamp, &cur);

        char buf[1024];
        snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
                    cur.tm_year + 1900,
                    cur.tm_mon  + 1,
                    cur.tm_mday,
                    cur.tm_hour,
                    cur.tm_min,
                    cur.tm_sec
                );

        return buf;
    }

    // ----------------------------------------------------
    // 配置和定义相关内容
    // ----------------------------------------------------
    const std::string defaultFilePath = "./log/";
    const std::string defaultFileName = "log.txt";

    // 定义日志标准
    enum LogLevel{
        DEBUG = 1,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    // 辅助函数：将枚举转为字符串
    std::string Level2String(LogLevel l){
        switch(l){
            case DEBUG : return "DEBUG";
            case INFO : return "INFO";
            case WARNING : return "WARNING";
            case ERROR : return "ERROR";
            case FATAL : return "FATAL";
            default : return "NONE";
        }
    }

    // ----------------------------------------------------
    // 策略模式
    // ----------------------------------------------------

    // 接口基类
    // 后续想添加网络日志、数据库日志
    // 只需要继承这个类，无需修改Logger代码
    class LogStrategy{
    public:
        // 为什么基类析构函数必须是virtual？
        // 若基类析构不是虚函数，当delete基类指针时，
        // 会调用基类的析构，而不会调用派生类的析构
        // 导致派生类中打开的文件流、申请的内存无法释放，造成内存泄漏
        virtual ~LogStrategy() = default;
        virtual void SyncLog(const std::string& msg) = 0;
    };

    // 控制台策略：输出到屏幕
    class ConsoleLogStrategy : public LogStrategy{
    public:
        ConsoleLogStrategy() {}
        ~ConsoleLogStrategy() {}

        void SyncLog(const std::string& msg) override{
            // 显示器是公共资源
            GuardLock guardLock(_lock);
            std::cout << msg << std::endl;
        }
    private:
        Mutex _lock;
    };

    // 文件策略：保存到文件中
    class FileLogStrategy : public LogStrategy{
    public:
        FileLogStrategy(const std::string fileName = defaultFileName,
                        const std::string filePath = defaultFilePath) 
                :_fileName(fileName)
                ,_filePath(filePath)
                {
                    GuardLock guardLock(_lock);
                    // 判断路径是否存在，不存在则创建
                    if(std::filesystem::exists(_filePath)) return;
                    try {
                        // 目录可能会创建失败
                        std::filesystem::create_directories(_filePath);
                    } catch(std::filesystem::filesystem_error& e) {
                        std::cerr << "create directory err: " << e.what() << std::endl;
                    }
                }
        ~FileLogStrategy() { }
        void SyncLog(const std::string& msg) override{
            GuardLock guardLock(_lock);
            std::string log = _filePath + _fileName;

            // 确保每次写入都是追加到文件结尾，而不是覆盖
            std::ofstream out(log, std::ios::app);
            if(!out.is_open()) return;
            out << msg << "\n";
            out.close();
        }
    private:
        Mutex _lock;
        const std::string _fileName;
        const std::string _filePath;
    };
        
    // ----------------------------------------------------
    // 日志类
    // ----------------------------------------------------
    class Logger{
    public:
        // 为了让LogMsg看到，还可以提供get方法
        std::shared_ptr<LogStrategy> _strategy;
        Logger(){
            // 默认初始化为控制台策略
            _strategy = std::make_shared<ConsoleLogStrategy>(); 
        }
        void EnableConsoleLog(){ _strategy = std::make_shared<ConsoleLogStrategy>(); }
        void EnableFileLog() { _strategy = std::make_shared<FileLogStrategy>(); }
        ~Logger(){}

        // 内部类：LogMsg
        // 一个LogMsg对象就代表一条有效日志信息
        class LogMsg{
        public:
            LogMsg(LogLevel l, const std::string& fileName, int line, Logger& logger)
                :_curTime(CurTime())
                ,_level(l)
                ,_pid(::getpid())
                ,_line(line)
                ,_logger(logger)
                {
                    // 拼装头部信息
                    // [time] [level] [pid] [fileName] [line] -
                    std::stringstream ssbuf;
                    ssbuf << "[" << _curTime << "] "
                          << "[" << _level << "] "
                          << "[" << _pid << "]"
                          << "[" << fileName << "]"
                          << "[" << line << "] - ";
                    _logInfo = ssbuf.str();
                }

            // 重载operator<<
            template <typename T>
            LogMsg& operator<<(const T& info){
                std::stringstream ss;
                ss << info;
                _logInfo += ss.str();
                // 支持链式调用
                return *this;
            }

            ~LogMsg(){
                if(_logger._strategy){
                    // 根据当前是什么策略，执行不同写入逻辑
                    _logger._strategy->SyncLog(_logInfo);
                }
            }
        private:
            std::string _curTime;
            LogLevel _level;
            pid_t _pid;
            std::string _fileName;
            int _line;
            Logger& _logger;        // 持有外部Logger引用，便于访问_strategy
            std::string _logInfo;   // 最终完整的日志字符串
        };

        //仿函数 构造并返回一个临时的LogMsg对象
        LogMsg operator()(LogLevel l, const std::string& fileName, int line){
            return LogMsg(l, fileName, line, *this);
        }
    
    };

    Logger logger;
    // ----------------------------------------------------
    // 宏定义
    // ----------------------------------------------------
    // __FILE__: 编译器内置宏，展开为当前源文件的文件名字符串
    // __LINE__: 编译器内置宏，展开为当前代码行号
    // 用法：LOG(LogLevel::INFO) << "msg";
    // 展开后：logger(LogLevel::INFO, "main.cc", 50) << "msg";
    //          创建LogMsg临时对象                   调用operator<< 追加消息
    //                                              语句结束，析构，刷盘
    #define LOG(Level) logger(Level, __FILE__, __LINE__)

    // 开关宏
    #define ENABLE_CONSOLE_LOG() logger.EnableConsoleLog()
    #define ENABLE_FILE_LOG() logger.EnableFileLog()
}