#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

// // 方式一 自然死亡
// void* thread_return(void* arg){
//     cout << "线程1 正在运行，准备通过return退出...\n";
//     sleep(1);
//     return (void*)10;
// }

// // 方式二 自杀 pthread_exit
// void* thread_exit_self(void* arg){
//     cout << "线程2 正在运行，准备通过phread_exit退出...\n";
//     sleep(1);
//     sleep(3);
//     // 立即终止当前线程 后续代码不会执行
//     pthread_exit((void*)20);

//     cout << "线程2 这行代码不会被执行！\n";
//     return nullptr;
// }

// // 方式三 他杀 pthread_cancel
// void* thread_cancel(void* arg){
//     cout << "线程3 正在死循环运行，等待被主线程取消...\n";
//     while(1){
//         // 这里的sleep是个取消点 只有遇到取消点 线程才会响应cancel信号
//         sleep(1);
//         cout << "线程3 还活着...\n";
//     }
//     return nullptr;
// }

// // 致命陷阱
// void* thread_trap(void* arg){
//     cout << "陷阱线程 如果我调用exit(1) 整个进程都会挂掉！\n";
//     // exit(1);     // 取消注释 会发现整个程序直接退出 后面的log全没了
//     return nullptr;
// }

// int main(){
//     pthread_t tid1, tid2, tid3, tid4;
//     void* ret_val;      

//     pthread_create(&tid1, nullptr, thread_return, nullptr);
//     pthread_create(&tid2, nullptr, thread_exit_self, nullptr);
//     pthread_create(&tid3, nullptr, thread_cancel, nullptr);
//     // pthread_create(&tid4, nullptr, thread_trap, nullptr);


//     cout << "==============================================" << endl;
//     pthread_join(tid1,&ret_val);
//     cout << "主线程 等待线程1退出，返回值：" << (long long)ret_val << endl;
//     pthread_join(tid2,&ret_val);
//     cout << "主线程 等待线程2退出，返回值：" << (long long)ret_val << endl;
    
//     cout << "主线程 请求取消线程3...\n";
//     pthread_cancel(tid3);
//     pthread_join(tid3,&ret_val);
//     // 如果线程是被 cancel 的
//     // join 得到的返回值是一个宏 PTHREAD_CANCELED (通常是 -1)
//     if(ret_val == PTHREAD_CANCELED)
//         cout << "主线程 线程3成功被取消...\n";
//     else
//         cout << "主线程 线程3异常退出...\n";

//     cout << "==============================================" << endl;
//     cout << "主线程 所有演示结束\n";

//     return 0;

// }

#include <iostream>
#include <pthread.h>
#include <unistd.h>

// 子线程函数：它要活得比主线程长
void* worker(void* arg) {
    for (int i = 1; i <= 5; i++) {
        sleep(1);
        cout << "[子线程] 我还在运行... 第 " << i << " 秒" << endl;
    }
    cout << "[子线程] 任务完成，我也要退出了。此时进程才会真正结束。" << endl;
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

    cout << "[主线程] 我只负责启动子线程，现在我要先走了！" << endl;

    // 重点来了！！！
    // 如果这里写 return 0; 或者 exit(0); 子线程会瞬间被杀掉。
    // 但是我们用 pthread_exit，只结束主线程自己。
    pthread_exit(nullptr);

    // 这行代码永远不会执行
    cout << "这行代码绝对看不见！" << endl;
    
    return 0;
}