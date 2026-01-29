#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include "LogInfo.hpp"

namespace ThreadPoolModule{
    using namespace LogTxt;

    class Task{
    public:
        Task(){ }
        Task(int id) :_id(id) { }
        void operator()(const std::string& thread_name){
            LOG(LogLevel::DEBUG) << "我是一个推送数据到服务器的任务，我正在执行[" << thread_name << "] ";
            sleep(2);
        }
    private:
        int _id;
    }
}