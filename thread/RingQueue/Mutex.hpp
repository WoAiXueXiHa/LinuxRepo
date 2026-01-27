#pragma once 
#include <pthread.h>

class Mutex{
public:
    Mutex(){ ::pthread_mutex_init(&_lock, nullptr); }
    void Lock() { ::pthread_mutex_lock(&_lock); }
    void Unlock() { ::pthread_mutex_unlock(&_lock); }
    ~Mutex() { ::pthread_mutex_destroy(&_lock); }

    pthread_mutex_t* GetLock() { return &_lock; }
private:
    pthread_mutex_t _lock;
};

class GuardLock{
private:
    Mutex& _mutex;
public:
    GuardLock(Mutex& mutex) :_mutex(mutex) { _mutex.Lock(); }
    ~GuardLock() { _mutex.Unlock(); }
};