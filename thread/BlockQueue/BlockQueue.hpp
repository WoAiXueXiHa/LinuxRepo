#pragma once
#include <pthread.h>
#include <iostream>
#include <queue>
#include "Mutex.hpp"
#include "Cond.hpp"

// version1
template<typename T>
class BlockQueue_v1{
private:
    std::queue<T>              _q;
    int                     _capacity;
    pthread_mutex_t         _mutex; 
    pthread_cond_t          _productor_cond;        // 队列满，生产者在这里等
    pthread_cond_t          _consumer_cond;         // 队列空，消费者在这里等
    int                     _cwait_num;
    int                     _pwait_num;

    bool IsFull() { return _q.size() == _capacity; }
    bool IsEmpty() { return _q.empty(); }
public:
    BlockQueue_v1(int cap = 10)
        :_capacity(cap)
        ,_cwait_num(0)
        ,_pwait_num(0)
        {
            // 初始化锁和条件变量
            // nullptr表示默认属性
            pthread_mutex_init(&_mutex, nullptr);
            // 条件变量是等待队列 不是锁
            pthread_cond_init(&_productor_cond, nullptr);
            pthread_cond_init(&_consumer_cond, nullptr);
        }

    // 生产者，写数据入队列
    // 输入型参数
    void Equeue(const T& in){
        // 1. 加锁
        pthread_mutex_lock(&_mutex);

        // 2. 检查条件IsFull
        // 为什么用while? 
        // 防止虚假唤醒，if线程被唤醒可能还没收到信号，会直接往下执行push
        // 如果此时队列满，会溢出或覆盖
        // 用while醒来之后会再次检查IsFull，不满足继续睡
        while(IsFull()){
            std::cout << "队列满了，生产者开始等待...\n";
            _pwait_num++;

            // 3. 等待
            // 函数底层做三件事：
            // 1） 解锁unlock，允许别人进来，否则会死锁
            // 2） 线程挂起->进入_productor_cond的等待队列
            // 3） 被唤醒后->重新抢锁lock，抢不到就阻塞在这里
            pthread_cond_wait(&_productor_cond, &_mutex);
            _pwait_num--;
            std::cout << "生产者被唤醒，准备生产...\n";
        }

        // 4. 生产
        // 到这里，说明IsFull为0，且我持有锁
        _q.push(in);

        // 5. 唤醒消费者
        // 只有当_cwait_num>0时，再去唤醒，否则浪费资源
        if(_cwait_num > 0){
            std::cout << "通知消费者来取货\n";
            // siganl只唤醒一个
            // broadcast全部唤醒
            pthread_cond_signal(&_consumer_cond);
        }

        // 6. 解锁
        pthread_mutex_unlock(&_mutex);
    }

    // 消费者，读数据出队列
    // 输出型参数
    void Dequeue(T* out){
        pthread_mutex_lock(&_mutex);

        while(IsEmpty()){
            std::cout << "队列空了，消费者开始等待...\n";
            _cwait_num++;

            pthread_cond_wait(&_consumer_cond, &_mutex);
            _cwait_num--;
            std::cout << "消费者被唤醒...\n";
        }
        
        *out = _q.front();
        _q.pop();

        if(_pwait_num > 0){
            std::cout << "通知生产者补货...\n";
            pthread_cond_signal(&_productor_cond);
        }

        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue_v1(){
        pthread_mutex_destroy(&_mutex); 
        pthread_cond_destroy(&_productor_cond);
        pthread_cond_destroy(&_consumer_cond);
    }
};

template <typename T>
class BlockQueue_v2{
private:
    int              _capacity;
    std::queue<T>       _q;
    Mutex            _mutex;
    Cond             _productor_cond;
    Cond             _consumer_cond;
    int              _pwait_num;
    int              _cwait_num;

    bool IsFull() { return _q.size() == _capacity; }
    bool IsEmpty() { return _q.empty(); }
public:
    // Mutex和Cond会自动调用构造初始化
    BlockQueue_v2(int cap = 10)
        :_capacity(cap)
        ,_pwait_num(0)
        ,_cwait_num(0)
        {}

    // 生产者
    void Equeue(const T& in){
        GuardLock guardLock(_mutex);

        while(IsFull()){
            std::cout << "生产者进入等待...\n";
            _pwait_num++;
            _productor_cond.Wait(_mutex);
            _pwait_num--;
            std::cout << "生产者被唤醒...\n";
        }
    
        _q.push(in);
        if(_cwait_num > 0){
            std::cout << "唤醒消费者\n";
            _consumer_cond.Notify();
        }
    }   // guardLock离开作用域自动析构，解锁

    void Dequeue(T* out){
        GuardLock guardLock(_mutex);
        while(IsEmpty()){
            std::cout << "消费者进入等待...\n";
            _cwait_num++;
            _consumer_cond.Wait(_mutex);
            _cwait_num--;
            std::cout << "消费者被唤醒...\n";
        }

        *out = _q.front();
        _q.pop();

        if(_pwait_num > 0){
            std::cout << "唤醒生产者\n";
            _productor_cond.Notify();
        }
    }

    // 成员变量会自用调用析构销毁
    ~BlockQueue_v2() { }
};