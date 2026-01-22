#pragma once
#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/shm.h>

// 约定 路径和ID
const std::string gpath = "/home/vect/LinuxRepo/IPC/shme";
int gprojId = 0x6666;        // ftok只取低8位

// 内存大小 通常4KB
// 若申请4097B 实际分配8KB两页 但是用户只能用4097
// 申请要申请4KB的整数倍
int gshmsize = 4096;

// 权限 只读
mode_t gmode = 0600;

// 把key转为16进制 ipcs -m 看到的key也是16进制
std::string ToHex(key_t k){
    char buf[64];
    snprintf(buf, sizeof(buf), "0x%x", k);
    return buf;
}

class ShareMemory{
private:
    int _shmid;                 // 共享内存ID   - fd
    key_t _key;                 // 共享内存key  - 路径
    void* _addr;                // 映射到进程虚拟地址的首地址

    // 核心辅助函数
    void CreateShmHelper(int shmflg){
        // 1. 生成唯一key
        // innode + dev +projId -> 算出唯一key
        _key = ::ftok(gpath.c_str(), gprojId);
        if(_key < 0){
            std::cerr << "ftok err\n";
            return;
        }
        
        // 2. 向内核申请共享内存
        _shmid = ::shmget(_key, gshmsize, shmflg);
        if(_shmid < 0){
            std::cerr << " shmget err\n";
            return;
        }
    }
public:
    ShareMemory()
        :_shmid(-1)
        ,_key(0)
        ,_addr(nullptr)
        {}
    ~ShareMemory() {}

    // 1. 创建or获取共享内存
    // 给server用 创建新的共享内存
    void CreateShm(){
        if(_shmid == -1){
            CreateShmHelper(IPC_CREAT | IPC_EXCL | gmode);
            std::cout << "key: " << ToHex(_key) << std::endl;
        }
    }

    // client用 获取已存在的
    void GetShm(){
        CreateShmHelper(IPC_CREAT);
    }

    // 2. 挂接 
    void AttachShm(){
        // 让OS自动选择合适的地址 0表示默认读写模式
        _addr = shmat(_shmid, nullptr, 0);
        // 兼容64位指针大小
        // (void*)-1
        if((long long)_addr == -1){
            std::cerr << "attach err\n";
        }
    }

    // 3. 去关联
    // 此时还不删除shm 只是修改页表
    void DetachShm(){
        if(_addr != nullptr){
            ::shmdt(_addr);
            std::cout << "detach done: \n";
        }
    }

    // 4. 删除 -> 标记删除
    void DeleteShm(){
        // 内存段被标记为dest 
        // 当所有挂接的进程都shmdt后 引用计数为0 释放内存
        shmctl(_shmid, IPC_RMID, nullptr);
    }

    void* GetAddr(){ return _addr; }
    void ShmInfo(){
        struct shmid_ds buffer;
        // IPC_STAT 从内核拷贝信息到用户态的buffer
        int n = ::shmctl(_shmid, IPC_STAT, &buffer);
        if(n < 0) return;
        std::cout << "###########################" << std::endl;
        std::cout << "Debug Info:" << std::endl;
        std::cout << "Last Attach Time: " << buffer.shm_atime << std::endl;
        std::cout << "Creator PID: " << buffer.shm_cpid << std::endl;
        std::cout << "Creation Time: " << buffer.shm_ctime << std::endl;
        std::cout << "Current Attach Count: " << buffer.shm_nattch << std::endl; // 当前有几个进程连着
        std::cout << "Permission Key: " << ToHex(buffer.shm_perm.__key) << std::endl;
        std::cout << "###########################" << std::endl;
    }
};

ShareMemory shm;

// 数据载体
struct data{
    char status[32];
    char lasttime[48];
    char image[4000];
};