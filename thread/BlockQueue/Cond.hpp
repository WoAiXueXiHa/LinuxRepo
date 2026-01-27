#pragma once
#include <pthread.h>
#include "Mutex.hpp"

class Cond{
public:
    Cond() { pthread_cond_init(&_cond, nullptr); }
    ~Cond() { pthread_cond_destroy(&_cond); }

    // wait需要一把锁，传入封装的Mutex对象
    void Wait(Mutex& m){
        // 调用底层的pthread_cond_Wait
        // 需要原生指针
        pthread_cond_wait(&_cond, m.GetLock());
    }
    void Notify() { pthread_cond_signal(&_cond); }
    void NotifyAll() { pthread_cond_broadcast(&_cond); }
private:
    pthread_cond_t _cond;
};