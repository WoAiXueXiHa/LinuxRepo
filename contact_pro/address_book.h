#include "contact.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_CONTACTS 1000   //最大联系人数

class address_book{
private:
  vector<contact> contacts;

  // 按姓名查找迭代器
  vector<contact>::iterator find_contact_iterator(const string& name);
  vector<contact>::const_iterator find_contact_const_iterator(const string& name) const;

public:
  void menu();  

  // 父进程：创建子进程执行对应功能
  void fork_add_contacts();
  void fork_show_contacts();
  void fork_delete_contacts();
  void fork_find_contacts();
  void fork_modify_contacts();
  void fork_clean_contacts();
  void fork_exit_contacts();

  // 子进程：实际功能实现
  void add_contacts();
  void show_contacts();
  void delete_contacts();
  void find_contacts();
  void modify_contacts();
  void clean_contacts();
  void exit_contacts();
};
