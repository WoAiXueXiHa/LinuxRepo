#pragma once
#include <iostream>
#include <pthread.h>

// 1. 封装底层的互斥锁
// 出生初始化锁 销毁扔掉锁
class Mutex{
public:
    // 禁止拷贝
    // 锁是一种独占的系统资源，拷贝会导致所有权不明，可能会二次析构或二次解锁
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    Mutex(){ ::pthread_mutex_init(&_lock, nullptr); }
    void Lock() { ::pthread_mutex_lock(&_lock); }
    void Unlock() { ::pthread_mutex_unlock(&_lock); }
    ~Mutex() { ::pthread_mutex_destroy(&_lock); }

private:
    pthread_mutex_t _lock;
};

// 2. 封装锁的守卫 RAII
// 创建自动上锁 销毁自动解锁
class LockGuard{
private:
    Mutex& _mutex;  // 要操作锁 而不是操作拷贝的锁
public:
    LockGuard(Mutex& mutex) :_mutex(mutex) { _mutex.Lock(); }
    ~LockGuard() { _mutex.Unlock(); }
};