#include "RingQueue.hpp"
#include <pthread.h>
#include <unistd.h>

using namespace RingBuffer;

void* consumer(void* arg){
    RingBuff<int>* ring_buff = static_cast<RingBuff<int>*>(arg);
    while(1){
        sleep(1);
        int data;
        ring_buff->Dequeue(&data);
        printf("consumer 消费了一个数据: %d\n",data);
    }
}

void* productor(void* arg){
    RingBuff<int>* ring_buff = static_cast<RingBuff<int>*>(arg);
    int data = 0;
    while(1){
        sleep(1);
        ring_buff->Enqueue(data);
        printf("productor 生产了一个数据: %d\n",data);
        data += 10;
    }
}

int main(){
    RingBuff<int>* ring_buff = new RingBuff<int>(5);

    pthread_t c1, c2, c3, p1, p2;
    pthread_create(&c1, nullptr, consumer, ring_buff);
    pthread_create(&c2, nullptr, consumer, ring_buff);
    pthread_create(&c3, nullptr, consumer, ring_buff);
    pthread_create(&p1, nullptr, productor, ring_buff);
    pthread_create(&p2, nullptr, productor, ring_buff);

    pthread_join(c1, nullptr);
    pthread_join(c2, nullptr);
    pthread_join(c3, nullptr);
    pthread_join(p1, nullptr);
    pthread_join(p2, nullptr);

    delete ring_buff;

    return 0;
}