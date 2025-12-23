#include "address_book.h"
#include <iostream>
#include <string>
using namespace std;  

vector<contact>::iterator address_book::find_contact_iterator(const string& name){
    for (auto it = contacts.begin(); it != contacts.end(); ++it) {
        if (it->get_name() == name) return it;
    }
    return contacts.end();
}

vector<contact>::const_iterator address_book::find_contact_const_iterator(const string& name) const {
    for (auto it = contacts.cbegin(); it != contacts.cend(); ++it) {
        if (it->get_name() == name) return it;
    }
    return contacts.cend();
}

// 父进程：菜单显示
void address_book::menu(){
 cout<<"*********************"<<endl; 
 cout<<"****1. 添加联系人****"<<endl; 
 cout<<"****2. 显式联系人****"<<endl; 
 cout<<"****3. 删除联系人****"<<endl; 
 cout<<"****4. 查找联系人****"<<endl; 
 cout<<"****5. 修改联系人****"<<endl; 
 cout<<"****6. 清空联系人****"<<endl; 
 cout<<"****7. 退出通讯录****"<<endl; 
}

// 添加联系人
void address_book::fork_add_contacts(){
  pid_t pid = fork();   // 创建子进程
  if(pid < 0){
    perror("add_contacts err!");
    exit(1);
  }else if(pid == 0){   // 子进程 执行添加功能
    cout<<"\n===添加联系人===\n";
    add_contacts();
    sleep(1);
    exit(0);
  }else{                // 父进程 回收子进程
    cout<<"等待添加联系人功能响应...\n";
    waitpid(pid,NULL,0);
    cout<<"联系人添加完成!\n\n";  
  }
}

void address_book::add_contacts(){
  //  容量检查
  if(contacts.size() >= MAX_CONTACTS){
    cout<<"通讯录已满1000人，正在扩容\n";
    contacts.resize(2*MAX_CONTACTS);
    cout<<"扩容成功，请继续操作...\n";
  }

  string name,phone,addr;
  int sex = 0;  // 初始化sex，避免未定义行为
  cout<<"请输入姓名："; cin >> name;
  
  // 性别输入：输错指引重新输入（无按任意键）
  while(true){
    cout<<"请输入性别（1代表男性，2代表女性）："; cin >> sex;  // 修复：赋值给sex而非name
    if(sex == 1 || sex == 2) break;
    cout<<"【输入错误】请按照要求输入：1-男性 2-女性（例如：2），请重新输入！\n";
  }
  
  cout<<"请输入电话："; cin >> phone;
  cout<<"请输入地址："; cin >> addr;

  contact new_contact(name, sex, phone, addr);
  contacts.push_back(new_contact);
  cout << "【提示】联系人" << name << "已成功添加到通讯录！\n";
}

// 显示联系人
void address_book::fork_show_contacts(){
  pid_t pid = fork();
  if(pid < 0){
    perror("show_contacts err");
    exit(1);
  }else if(pid == 0){
    cout<<"正在加载联系人...\n";
    sleep(1);
    show_contacts();
    exit(0);
  }else{
    waitpid(pid,NULL,0);
    cout<<"联系人已全部展示！\n\n";  
  }
}

void address_book::show_contacts(){
  // 考虑空表（无按任意键，直接提示后返回）
  if(contacts.empty()){
    cout<<"【提示】通讯录为空！\n";
    return;
  }
  
  cout<<"当前联系人总数："<<contacts.size()<<endl;
  cout<<"----------------------------"<<endl;
  for(const auto& cnt : contacts){
    cout<<"姓名："<<cnt.get_name()<<"\t";
    cout<<"性别："<<(cnt.get_sex() == 1 ? "男" : "女")<<"\t";
    cout<<"电话："<<cnt.get_phone()<<"\t";
    cout<<"地址："<<cnt.get_address()<<endl;  
  }
  cout<<"----------------------------"<<endl;
}

// 删除联系人
void address_book::fork_delete_contacts() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("delete_contact err");
        exit(1);
    } else if (pid == 0) {
        cout<<"正在执行删除操作...\n";
        sleep(1);
        delete_contacts();
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        cout << "删除操作执行完毕！\n\n";  
    }
}

void address_book::delete_contacts() {
    string name;
    cout << "请输入要删除的联系人姓名："; cin >> name;
    auto it = find_contact_iterator(name);
    
    if (it == contacts.end()) {
        cout << "【查找失败】未找到联系人【" << name << "】，删除操作取消！\n";
    } else {
        contacts.erase(it);  // 子进程副本中删除
        cout << "【删除成功】联系人【" << name << "】已删除（仅子进程可见）！\n";
    }
}

// 查找联系人
void address_book::fork_find_contacts() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("find_contact err");
        exit(1);
    } else if (pid == 0) {
        cout << "执行查找功能中...\n";
        sleep(1);
        find_contacts();
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        cout << "查找执行完成！\n\n";  
    }
}

void address_book::find_contacts() {
    string name;
    cout << "请输入要查找的联系人姓名："; cin >> name;
    auto it = find_contact_const_iterator(name);
    
    if (it == contacts.cend()) {
        cout << "【查找失败】未找到联系人【" << name << "】！\n";
    } else {
        cout << "---------------------------" << endl;
        cout << "姓名：" << it->get_name() << "\t";
        cout << "性别：" << (it->get_sex() == 1 ? "男" : "女") << "\t";
        cout << "电话：" << it->get_phone() << "\t";
        cout << "地址：" << it->get_address() << endl;
        cout << "---------------------------" << endl;
    }
}

// 修改联系人
void address_book::fork_modify_contacts() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("modify_contact err");
        exit(1);
    } else if (pid == 0) {
        modify_contacts();
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        cout << "修改完成！\n\n";  
    }
}

void address_book::modify_contacts() {
    string name;
    cout << "请输入要修改的联系人姓名："; cin >> name;
    auto it = find_contact_iterator(name);
    
    if (it == contacts.end()) {
        cout << "【修改失败】未找到联系人【" << name << "】，修改操作取消！\n";
        return;
    }

    string new_name, new_phone, new_addr;
    int new_sex;
    
    cout << "请输入新姓名："; cin >> new_name; it->set_name(new_name);
    
    // 性别输错指引重新输入
    while (true) {
        cout << "请输入新性别（1-男，2-女）："; cin >> new_sex;
        if (it->set_sex(new_sex)) break;
        cout << "【输入错误】性别只能输入1或2，请重新输入！\n";
    }
    
    cout << "请输入新电话："; cin >> new_phone; it->set_phone(new_phone);
    cout << "请输入新地址："; cin >> new_addr; it->set_address(new_addr);

    cout << "【修改成功】联系人【" << new_name << "】信息已更新！\n";
}

// 清空联系人（删除按任意键+输错指引）
void address_book::fork_clean_contacts() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("clean_contacts err");
        exit(1);
    } else if (pid == 0) {
        cout << "正在执行清空操作..." << endl;
        sleep(1);
        clean_contacts();
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        cout << "清空完成！\n\n";  
    }
}

void address_book::clean_contacts() {
    char confirm;
    cout << "确定清空所有联系人吗？（y/n）："; cin >> confirm;
    
    // 输错指引（非y/Y则取消）
    if (confirm != 'y' && confirm != 'Y') {
        cout << "【操作取消】已放弃清空通讯录！\n";
        return;
    }
    
    contacts.clear();  // 子进程副本中清空
    cout << "【清空成功】通讯录所有联系人已清空！\n";
}

// 退出系统
void address_book::fork_exit_contacts(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("exit_contacts err");
        return;
    } else if (pid == 0) {
        exit_contacts();
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }
}

void address_book::exit_contacts(){
    cout << "欢迎下次使用通讯录系统！\n";
    sleep(1);
}
