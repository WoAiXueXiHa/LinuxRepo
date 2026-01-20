#include "server.hpp"
#include <iostream>

int main(){
    Server s;
    std::cout << "pos 1\n";
    s.OpenPipeForRead();
    std::cout << "pos2\n";

    std::string msg;
    while(1){
        if(s.RecvPipe(&msg) > 0) 
            std::cout << "client sat: " << msg <<std::endl;
        else break;
        std::cout << "pos 3\n";
    }
    std::cout << "client quit, me too\n";
    s.ClosePipe();

    return 0;
}