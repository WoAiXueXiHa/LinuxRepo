#pragma once
#include <semaphore.h>

namespace SemModule{
    int defalutSemVal = 1;
    class Sem{
    public:
        Sem(int value = defalutSemVal)
            :_init_value(value)
            {
                // 参数2为0，表示线程间共享，非0表示进程间共享
                int n = ::sem_init(&_sem, 0, _init_value);
            }
        
        // P操作：proberen申请资源
        // 如果计数器 > 0，原子-1，返回
        // 如果计数器 == 0，阻塞等待，知道被唤醒
        void P(){ int n = ::sem_wait(&_sem); }

        // V操作：verhogen归还资源
        // 计数器原子+1，如果有线程在等待该资源，唤醒它
        void V(){ int n = ::sem_post(&_sem); }

        ~Sem(){ int n = ::sem_destroy(&_sem); }
    private:
        sem_t _sem;
        int _init_value;
    };
}