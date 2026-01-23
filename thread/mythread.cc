#include <iostream>
#include <unistd.h>
#include <pthread.h>


void* run(void* args){
    while(1){
        std::cout << "我是新线程 pid为" << getpid() << std::endl;
        sleep(1);
    }
    return nullptr;
}
int main(){
    std::cout << "我是一个进程 我的pid为 " << getpid() << std::endl;

    pthread_t thread_id;
    pthread_create(&thread_id, nullptr, run, (void*)"thread-1");

    while(1){
        std::cout << "我是主线程 pid为" << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}