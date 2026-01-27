#include "Cond.hpp"
#include "Mutex.hpp"
#include "BlockQueue.hpp"
#include <unistd.h>

void* consumer(void* arg){
    BlockQueue_v2<int>* bq = static_cast<BlockQueue_v2<int>*>(arg);
    while(1){
        int data;
        bq->Dequeue(&data);
        printf("consumer 消费了一个数据: %d\n", data);
    }
}

void* productor(void* arg){
    BlockQueue_v2<int>* bq = static_cast<BlockQueue_v2<int>*>(arg);
    int data = 50;
    while(1){
        sleep(2);
        bq->Equeue(data);
        printf("producter 生产了一个数据: %d\n", data);
        data++;
    }
}
int main(){
    BlockQueue_v2<int>* bq = new BlockQueue_v2<int>(50);

    pthread_t c, p;
    pthread_create(&c, nullptr, consumer, bq);
    pthread_create(&p, nullptr, productor, bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    delete bq;
    return 0;
}