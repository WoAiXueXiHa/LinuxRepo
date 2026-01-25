#include "thread_v1.hpp"
using namespace std;
using namespace thread_v1;

void task()
{
    while(1)
    {
        cout << "v1 线程正在疯狂运行...\n";
        sleep(1);
    }
}
// int main(){
//     thread t(task);

//     t.Start();
//     cout << "主线程 " << t.getName() << " 已启动\n";
//     sleep(3);
//     cout << "主线程：准备强制停止v1线程...\n";
//     t.Stop();
//     t.Join();
//     cout << "主线程： v1线程已销毁\n";

//     return 0;
// }