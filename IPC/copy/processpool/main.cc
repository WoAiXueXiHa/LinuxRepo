#include "Task.hpp"
#include "ProcessPool.hpp"

void Usager(std::string proc){
    std::cout << "Usage: " << proc << " process-num\n";
}

int main(int argc, char* argv[]){
    if(2 != argc){
        Usager(argv[0]);
        return USAGEERR;
    }

    int num = std::atoi(argv[1]);
    ProcessPool* pp = new ProcessPool(num, Worker);

    pp->ProcessInit();
    pp->DispatchTask();
    pp->CleanProcessPool();

    delete pp;
    pp = nullptr;
    return 0;

}