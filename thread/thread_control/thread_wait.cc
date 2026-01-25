#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

// 场景1：子线程计算数据 返回一个结构体指针
struct Result{
    int code;
    string msg;
};

void* thread_work(void* arg){
    long long id = (long long)arg;
    cout << "子线程 " << id << " 开始工作...\n";
    
    sleep(2);

    // 返回值必须在堆区开辟！！！或者使用静态/全局变量
    // 绝对不能返回局部变量地址！！！函数结束栈帧就销毁了
    Result* res = new Result();
    res->code = 200;
    res->msg = "success";

    cout << "子线程 " << id << " 工作完成，返回结果地址：" << res << endl;

    return (void*)res;
}

// 场景2：子线程被异常取消
void* thread_bad(void* arg){
    cout << "子线程bad 我来打酱油，马上被cancel...\n";
    while(1) sleep(1);
    return nullptr;
}

int main(){
    pthread_t tid_work, tid_bad;

    pthread_create(&tid_work, nullptr, thread_work, nullptr);

    cout << "主线程 正在阻塞等待子线程完成...\n";

    void* ret_ptr = nullptr;
    int n = pthread_join(tid_work, &ret_ptr);
    if(n == 0){
        Result* final_res = (Result*)ret_ptr;
        cout << "主线程 成功回收工作线程！\n";
        cout << "   |--返回地址： " << ret_ptr << endl;
        cout << "   |--状态码： " << final_res->code << endl;
        cout << "   |--消息： " << final_res->msg << endl;

        delete final_res;
    }

    cout << "------------------------------------------" << endl;
    pthread_cancel(tid_bad);
    pthread_join(tid_bad, &ret_ptr);
    if(ret_ptr == PTHREAD_CANCELED)
        cout << "主线程 检测到子线程时被强制取消的！\n";

    return 0;
}