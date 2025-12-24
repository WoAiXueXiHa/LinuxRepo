#include "address_book.h"
#include <iostream>
using namespace std;

int main() {
    address_book book;
    int select;

    while (true) {
        book.menu();
        cin >> select;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }

        switch (select) {
        case 1: book.add_contacts(); break;
        case 2: book.show_contacts(); break;
        case 3: book.delete_contacts(); break;
        case 4: book.find_contacts(); break;
        case 5: book.modify_contacts(); break;
        case 6: book.clean_contacts(); break;
        case 7: return 0;
        default: cout << "无效选项\n";
        }
    }
}


