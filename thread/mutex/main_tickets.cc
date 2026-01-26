#include "thread.hpp"


// // 模拟抢票 演示bug
// using namespace std;
// using namespace thread_v1;

// // 全局共享资源 火车票
// static int tickets = 1000;

// void GetTicket()
// {
//     while (1)
//     {
//         if (tickets > 0)
//         {
//             // 模拟抢票耗时
//             usleep(1000);

//             cout << "当前线程： " << pthread_self() << "抢到了第 " << tickets << " 张票\n";
//             tickets--;
//         }else{
//             break;
//         }
//     }
// }

// int main(){
//     thread t1(GetTicket);
//     thread t2(GetTicket);
//     thread t3(GetTicket);
//     thread t4(GetTicket);

//     t1.Start();
//     t2.Start();
//     t3.Start();
//     t4.Start();

//     t1.Join();
//     t2.Join();
//     t3.Join();
//     t4.Join();

//     cout << "最终剩余票数: " << tickets << endl;

//     return 0;
// }


/*=========================== 加锁修复 ==================================*/
// using namespace std;

// static int tickets = 1000;
// // 定义一把全局锁
// pthread_mutex_t mtx;

// void GetTicket(){
//     while(1){
//         // 1. 加锁
//         // 排队领钥匙，如果这时候被人拿着锁，当前线程阻塞
//         pthread_mutex_lock(&mtx);

//         // =========== 临界区开始 只有一个人能进入 ==========
//         if(tickets > 0){
//             usleep(1000);
//             cout << "当前线程: " << pthread_self() << " 抢到了第 " << tickets << " 张票\n";
//             tickets--;

//             // 2. 解锁
//             // 办完事了，赶紧还钥匙，让门口排队的人抢
//             pthread_mutex_unlock(&mtx);
//         } else {
//             // 解锁完再走！！！！
//             // 如果没票 准备break离开循环
//             // 但是手里还拿着锁呢！！！直接break 这把锁永远不会释放！！！
//             // 结果：门口的人等到天荒地老！死锁！
//             pthread_mutex_unlock(&mtx);
//             break;
//         }
//         // =========== 临界区结束 ===================
//         // 抢完一张票后面排队去，给别人机会，不然一个线程可能解锁之后又抢到了，导致线程饥饿
//         usleep(1000);
//     }
// }

// int main(){
//     // 0. 初始化锁
//     // nullptr表示默认属性
//     pthread_mutex_init(&mtx, nullptr);

//     thread_v1::thread t1(GetTicket);
//     thread_v1::thread t2(GetTicket);
//     thread_v1::thread t3(GetTicket);
//     thread_v1::thread t4(GetTicket);

//     t1.Start();
//     t2.Start();
//     t3.Start();
//     t4.Start();

//     t1.Join();
//     t2.Join();
//     t3.Join();
//     t4.Join();

//     // 3. 销毁锁
//     // 所有人都走了之后再销毁
//     pthread_mutex_destroy(&mtx);

//     cout << "最终剩余票数: " << tickets << endl;

//     return 0;
// }


// RAII加锁
#include "LockGuard.hpp"

using namespace std;

// 1. 定义共享数据结构
// 锁和资源绑定
struct TicketData{
    int tickets;
    Mutex mtx;
    string name;
    
    TicketData(int t, string n)
        :tickets(t)
        ,name(n)
        {}
};

// 2. 抢票逻辑
void GetTicket(TicketData* data){
    while(1){
        // 作用域块：一个局部遍历区域，用{}把临界区包起来
        {
            // 1. 构造guard对象 自动调用mtx.Lock()
            LockGuard guard(data->mtx);

            if(data->tickets > 0){
                usleep(1000);
                cout << "线程 " << pthread_self() << " (" << data->name
                     << ") 抢到了第 " << data->tickets << " 张票\n";
                data->tickets--;
            }else{
                // guard对象离开作用域，析构自动调用mtx.Unlock()
                break;
            }
        }
        usleep(1000);
    }
    cout << "线程 " << pthread_self() << " 退出抢票...\n";
}

int main(){
    // 准备共享数据
    // 数据必须在堆上或者主线程栈上，保证声明周期比子线程更长
    TicketData* global_data = new TicketData(1000,"抢票系统");

    // T被推到为TicketData*
    thread_v2::thread<TicketData*> t1(GetTicket, global_data);
    thread_v2::thread<TicketData*> t2(GetTicket, global_data);
    thread_v2::thread<TicketData*> t3(GetTicket, global_data);
    thread_v2::thread<TicketData*> t4(GetTicket, global_data);

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();

    t1.Stop();
    t2.Stop();
    t3.Stop();
    t4.Stop();

    cout << "最终剩余票数: " << global_data->tickets << endl;

    delete global_data;
    return 0;

}