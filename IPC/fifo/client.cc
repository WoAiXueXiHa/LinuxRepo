#include "client.hpp"
#include <iostream>

int main(){
    Client c;
    c.OpenPipeForWrite();

    std::string msg;
    while(1){
        std::cout << "please enter# ";
        std::getline(std::cin, msg);
        c.SendPipe(msg);
    }
    c.ClosePipe();
    return 0;
}