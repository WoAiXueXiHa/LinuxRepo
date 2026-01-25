#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

// 场景1：子线程自己分离自己
void* thread_self_detach(void* arg){
    // 获取自己的ID并分离
    pthread_detach(pthread_self());

    cout << "[主动独立线程] 我已经自我分离，运行2秒后自动消失，不用管我...\n";
    sleep(2);
    cout << "[主动独立线程] 拜拜！\n";
    return nullptr;
}

// 场景2：被主线程分离
void* thread_passive(void* arg){
    cout << "[被动独立线程] 我被主线程踢开了，运行1秒...\n";
    sleep(1);
    return nullptr;
}

int main(){
    pthread_t tid1, tid2;
    // 方式一：子线程内部自我分离
    pthread_create(&tid1, nullptr, thread_self_detach, nullptr);
    // 尝试join一个分离线程
    sleep(1);
    int ret = pthread_join(tid1, nullptr);
    if(ret != 0){
        // 分离成功 join应该失败
        cout << "[主线程] join 线程1 失败 符合预期！ 错误码： " << ret << endl;
    }else{
        cout << "[主线程]成功join 说明还没分离！\n";
    }

    cout << "================================================\n";
    
    // 方式二：主线程主动分离子线程
    pthread_create(&tid2, nullptr, thread_passive, nullptr);

    cout << "[主线程] 主动分离线程2...\n";
    pthread_detach(tid2);
    // 再次尝试join
    ret = pthread_join(tid2, nullptr);
    if(ret != 0){
        cout << "[主线程] join 线程2 失败 符合预期！ 错误码： " << ret << endl;
    }

    cout << "==========================================\n";
    cout << "[主线程] 我任务结束了 为了杀死在运行的线程 使用pthread_exit退出\n";

    pthread_exit(nullptr);
}