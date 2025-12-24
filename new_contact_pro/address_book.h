#pragma once
#include "contact.h"
#include <vector>
#include <string>
using namespace std;

#define DATA_FILE "contacts.db"

class address_book {
private:
    vector<contact> contacts;

    vector<contact>::iterator find_contact_iterator(const string& name);
    vector<contact>::const_iterator find_contact_const_iterator(const string& name) const;

public:
    address_book();   // 启动自动读取
    ~address_book();  // 退出自动保存

    void menu();

    void add_contacts();     // 多进程 + pipe
    void show_contacts();    // 单进程
    void delete_contacts();  // 多进程判断
    void find_contacts();
    void modify_contacts();
    void clean_contacts();
};
