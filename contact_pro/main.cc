#include "address_book.h"
#include <iostream>
using namespace std;

int main(){
  address_book addr_book;
  int select = 0;

  cout<<"==== 启动通讯录 ====\n";
  while(true){
    addr_book.menu();
    cout<<"请输入功能编号（1-7）：";  
    cin >> select;

    // 调度不同进程
    switch(select){
      case 1 : addr_book.fork_add_contacts();  break;
      case 2 : addr_book.fork_show_contacts();  break;
      case 3 : addr_book.fork_delete_contacts();  break;
      case 4 : addr_book.fork_find_contacts();  break;
      case 5 : addr_book.fork_modify_contacts();  break;
      case 6 : addr_book.fork_clean_contacts();  break;
      case 7 : addr_book.fork_exit_contacts();  
               cout << "退出通讯录系统！\n";
               exit(0);  
               break;
      default: 
          cout << "【输入错误】功能编号只能是1-7的数字，请重新输入！\n\n";
          break;
    }
  }
  return 0;
}
