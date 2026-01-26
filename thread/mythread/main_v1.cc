#include "thread_v1.hpp"
using namespace std;
using namespace thread_v1;

// 模拟抢票
using namespace std;
using namespace thread_v1;

// 全局共享资源 火车票
static int tickets = 1000;

void GetTicket()
{
    while (1)
    {
        if (tickets > 0)
        {
            // 模拟抢票耗时
            usleep(1000);

            cout << "当前线程： " << pthread_self() << "抢到了第 " << tickets << " 张票\n";
            tickets--;
        }else{
            break;
        }
    }
}

int main(){
    thread t1(GetTicket);
    thread t2(GetTicket);
    thread t3(GetTicket);
    thread t4(GetTicket);

    t1.Start();
    t2.Start();
    t3.Start();
    t4.Start();

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();

    cout << "最终剩余票数: " << tickets << endl;

    return 0;
}