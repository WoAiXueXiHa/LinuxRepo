#pragma once

#include <pthread.h>
#include "Mutex.hpp"

namespace CondModule{
    using namespace LockModule;
    class Cond{
    public:
        Cond() { ::pthread_cond_init(&_cond, nullptr); }
        ~Cond() { ::pthread_cond_destroy(&_cond); }
        // 传入Mutex& 是为了调用GetLock()拿到原生锁
        // 只有拿到锁，pthread_cond_wait才能在进入等待前把所打开
        void Wait(Mutex& lock){ ::pthread_cond_wait(&_cond, lock.GetLock()); }
        void NotifyOne() { ::pthread_cond_signal(&_cond); }
        // 线程池销毁时，告诉所有线程下班
        // 一般不要用，会引起惊群效应
        void NotifyAll() { ::pthread_cond_broadcast(&_cond); }
    private:
        pthread_cond_t _cond;
    };
}