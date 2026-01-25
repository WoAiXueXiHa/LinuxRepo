#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

// 结构体传参
struct ThreadData{
    int thread_id;
    std::string msg;
};

std::string ToHex(pthread_t tid){
    char buf[64];
    snprintf(buf, sizeof(buf), "0x%lx", tid);
    return buf;
}

// 新线程入口函数
void* thread_routine(void* arg){
    // 参数是任意类型 按照我们需要传递的类型强转
    ThreadData* data = static_cast<ThreadData*>(arg);

    // 获取当前线程的ID
    pthread_t tid = pthread_self();

    std::cout << "新线程启动！库ID：" << ToHex(tid)
              <<" | 业务ID：" << data->thread_id
              <<" | 消息：" << data->msg << std::endl;

    sleep(3);

    std::cout << "新线程任务完成，准备退出...\n";

    std::string* result = new std::string("success");

    return (void*)result;

}

int main(){
    std::cout << "主线程开始运行, pid: " << getpid() << std::endl;

    pthread_t tid;  // 新线程ID
    // C++11列表初始化
    ThreadData* tdata = new ThreadData { 1, "hello from thread!"};
    // 参数1 线程ID指针->输出型参数
    // 参数2 线程属性->nullptr是默认属性
    // 参数3 线程函数->函数指针
    // 参数4 传给线程函数的参数->任意类型
    int ret = pthread_create(&tid, nullptr, thread_routine, (void*)tdata);
    if(ret != 0){
        std::cerr << "线程创建失败，错误码：" << ret <<std::endl;
    }

    std::cout << "主线程创建新线程成功，新线程ID： " << tid << std::endl;
    std::cout << "主线程做我的事情...\n";

    void* thread_result;
    std::cout << "主线程阻塞等待新线程结束...\n";
    pthread_join(tid, &thread_result);

    std::string* res_str = static_cast<std::string*>(thread_result);
    std::cout << "主线程等待新线程退出成功，返回值：" << *res_str << std::endl;

    delete tdata;
    delete res_str;

    return 0;
}
