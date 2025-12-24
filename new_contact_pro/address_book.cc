#include "address_book.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
using namespace std;

/* ========= 进程安全的原始结构（严禁 STL） ========= */
struct contact_raw {
    char name[64];
    int sex;
    char phone[64];
    char address[128];
};

/* ========= 启动自动读取 ========= */
address_book::address_book() {
    ifstream fin(DATA_FILE);
    if (!fin.is_open()) return;

    string name, phone, addr;
    int sex;
    while (fin >> name >> sex >> phone >> addr) {
        contacts.emplace_back(name, sex, phone, addr);
    }
    fin.close();
}

/* ========= 退出自动保存 ========= */
address_book::~address_book() {
    ofstream fout(DATA_FILE, ios::trunc);
    for (const auto& c : contacts) {
        fout << c.get_name() << " "
             << c.get_sex() << " "
             << c.get_phone() << " "
             << c.get_address() << "\n";
    }
    fout.close();
}

/* ========= 查找 ========= */
vector<contact>::iterator address_book::find_contact_iterator(const string& name) {
    for (auto it = contacts.begin(); it != contacts.end(); ++it)
        if (it->get_name() == name) return it;
    return contacts.end();
}

vector<contact>::const_iterator address_book::find_contact_const_iterator(const string& name) const {
    for (auto it = contacts.cbegin(); it != contacts.cend(); ++it)
        if (it->get_name() == name) return it;
    return contacts.cend();
}

/* ========= 菜单 ========= */
void address_book::menu() {
    cout << "\n1 添加  2 显示  3 删除  4 查找  5 修改  6 清空  7 退出\n";
}

/* ========= 添加联系人（多进程 + pipe，已修正性别输入判断） ========= */
void address_book::add_contacts() {
    string name, phone, addr;
    int sex;

    cout << "姓名："; cin >> name;

    // 性别输入校验：循环直到输入合法
    while (true) {
        cout << "性别(1/2)："; 
        // 检查输入是否为有效整数
        if (!(cin >> sex)) {
            cin.clear(); // 清除输入错误标志
            cin.ignore(1024, '\n'); // 清空输入缓冲区
            cout << "输入错误！性别必须是数字，请输入 1（男）或 2（女）\n";
            continue;
        }
        // 检查数值是否在合法范围
        if (sex != 1 && sex != 2) {
            cout << "输入错误！性别只能是 1（男）或 2（女），请重新输入\n";
            continue;
        }
        // 输入合法，退出循环
        break;
    }

    cout << "电话："; cin >> phone;
    cout << "地址："; cin >> addr;

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);

        contact_raw raw{};
        strncpy(raw.name, name.c_str(), sizeof(raw.name) - 1);
        strncpy(raw.phone, phone.c_str(), sizeof(raw.phone) - 1);
        strncpy(raw.address, addr.c_str(), sizeof(raw.address) - 1);
        raw.sex = sex;

        write(fd[1], &raw, sizeof(raw));
        close(fd[1]);
        _exit(0);
    }

    close(fd[1]);
    contact_raw raw{};
    read(fd[0], &raw, sizeof(raw));
    close(fd[0]);
    waitpid(pid, nullptr, 0);

    contacts.emplace_back(raw.name, raw.sex, raw.phone, raw.address);
    cout << "添加成功\n";
}

/* ========= 显示 ========= */
void address_book::show_contacts() {
    if (contacts.empty()) {
        cout << "通讯录为空\n";
        return;
    }

    for (const auto& c : contacts) {
        cout << c.get_name() << " "
             << (c.get_sex() == 1 ? "男" : "女") << " "
             << c.get_phone() << " "
             << c.get_address() << endl;
    }
}

/* ========= 删除（子进程只判断是否存在） ========= */
void address_book::delete_contacts() {
    string name;
    cout << "删除姓名："; cin >> name;

    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        bool found = false;
        for (const auto& c : contacts)
            if (c.get_name() == name) found = true;
        write(fd[1], &found, sizeof(found));
        close(fd[1]);
        _exit(0);
    }

    close(fd[1]);
    bool found = false;
    read(fd[0], &found, sizeof(found));
    close(fd[0]);
    waitpid(pid, nullptr, 0);

    if (!found) {
        cout << "未找到联系人\n";
        return;
    }

    contacts.erase(find_contact_iterator(name));
    cout << "删除成功\n";
}

/* ========= 查找 ========= */
void address_book::find_contacts() {
    string name;
    cout << "查找姓名："; cin >> name;

    auto it = find_contact_const_iterator(name);
    if (it == contacts.cend()) {
        cout << "未找到\n";
        return;
    }

    cout << it->get_name() << " "
         << it->get_phone() << " "
         << it->get_address() << endl;
}

/* ========= 修改 ========= */
void address_book::modify_contacts() {
    string name;
    cout << "修改姓名："; cin >> name;

    auto it = find_contact_iterator(name);
    if (it == contacts.end()) {
        cout << "未找到\n";
        return;
    }

    int sex;
    string phone, addr;
    // 同样给修改功能的性别添加输入校验
    while (true) {
        cout << "新性别(1/2)："; 
        if (!(cin >> sex)) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "输入错误！性别必须是数字，请输入 1（男）或 2（女）\n";
            continue;
        }
        if (sex != 1 && sex != 2) {
            cout << "输入错误！性别只能是 1（男）或 2（女），请重新输入\n";
            continue;
        }
        break;
    }
    cout << "新电话："; cin >> phone;
    cout << "新地址："; cin >> addr;

    it->set_sex(sex);
    it->set_phone(phone);
    it->set_address(addr);
    cout << "修改成功\n";
}

/* ========= 清空 ========= */
void address_book::clean_contacts() {
    contacts.clear();
    cout << "已清空\n";
}
