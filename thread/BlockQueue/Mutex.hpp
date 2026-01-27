#pragma once
#include <iostream>
#include <pthread.h>

class Mutex{
public:
    Mutex() { pthread_mutex_init(&_lock, nullptr); }
    void Lock() { pthread_mutex_lock(&_lock); }
    void Unlock() { pthread_mutex_unlock(&_lock); }
    ~Mutex() { pthread_mutex_destroy(&_lock); }
    // 关键：条件变量需要锁，要传指针
    pthread_mutex_t* GetLock() { return &_lock; }
private:
    pthread_mutex_t _lock;
};

class GuardLock{
public:
    GuardLock(Mutex& m) :_mtx(m) { _mtx.Lock(); }
    ~GuardLock() { _mtx.Unlock(); }
private:
    Mutex& _mtx;
};