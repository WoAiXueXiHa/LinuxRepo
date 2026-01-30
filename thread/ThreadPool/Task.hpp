#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "Log.hpp"

using namespace LogMudle;

using task_t = std::function<void(std::string name)>;

void Push(std::string name){
    //  模拟业务实现
    LOG(DEBUG) << "我是一个任务，我正在被执行 " << "[" << name << "]";
}