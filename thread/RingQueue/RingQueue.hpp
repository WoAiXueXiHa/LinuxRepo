#pragma once
#include <iostream>
#include <vector>
#include <pthread.h>
#include "Sem.hpp"
#include "Mutex.hpp"

namespace RingBuffer{
    using namespace SemModule;

    template<typename T>
    class RingBuff{
    private:
        std::vector<T> _ring;
        int            _capacity;
        // 生产者和消费者的索引，临界资源，需要保护
        int            _p_idx;          
        int            _c_idx;
        // 同步机制：信号量
        Sem            _dataSem;
        Sem            _spaceSem;
        // 互斥机制：锁
        Mutex          _p_lock;
        Mutex          _c_lock;
    public:
        RingBuff(int cap)
            :_capacity(cap)
            ,_ring(cap)
            ,_p_idx(0)
            ,_c_idx(0)
            ,_dataSem(0)
            ,_spaceSem(cap)
            {}

        // 生产者
        void Enqueue(const T& in){
            // 必须先P再Lock
            // 先申请资源。如果先锁再P，且资源不足，生产者抱着锁挂起
            // 消费者向消费腾出空间，却拿不到锁-`>死锁

            // 1. 申请空间资源（-1）
            _spaceSem.P();

            {
                // 2. 只有申请到资源后，才竞争锁保护临界区
                GuardLock guardLock(_p_lock);

                _ring[_p_idx++] = in;
                _p_idx %= _capacity;
            }
            // 3. 生产完毕，增加数据资源，唤醒消费者（+1）
            _dataSem.V();
        }

        // 消费者
        void Dequeue(T* out){
            // 1. 申请资源（-1） 没数据就阻塞在这，不占锁
            _dataSem.P();

            {
                // 2. 加锁访问
                GuardLock guardLock(_c_lock);

                *out = _ring[_c_idx++];
                _c_idx %= _capacity;
            }
            // 3. 消费完毕，归还空间资源，唤醒生产者（+1）
            _spaceSem.V();
        }

        ~RingBuff(){}
    };
}