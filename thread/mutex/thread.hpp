#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>

namespace thread_v1
{
    static int gnumber = 1;
    using func_t = std::function<void()>;
    enum STATUS
    {
        NEW = 0, // 刚创建 还没运行
        RUNNING, // 正在运行
        STOP     // 停止
    };

    class thread
    {
    private:
        // 为什么要static？
        // pthread_create要求传一个普通的函数指针
        // 但是类内成员函数天然有一个this指针 定义在类外不优雅
        // 而静态成员函数没有this指针！
        static void *routine(void *arg)
        {
            // 参数传完了 再强转回对象原来的类型
            thread *t = static_cast<thread *>(arg);
            t->_status = STATUS::RUNNING;
            // 回调真正的业务逻辑
            t->_func();
            return nullptr;
        }
        void EnableDetach() { _joinable = false; }

    public:
        thread(func_t func)
            : _func(func), _status(STATUS::NEW), _joinable(true) // 默认是可连接
        {
            _name = "thread- " + std::to_string(gnumber++);
            _pid = getpid();
        }

        bool Start()
        {
            // 只有NEW状态下才能启动
            if (_status != STATUS::RUNNING)
            {
                int n = ::pthread_create(&_tid, nullptr, routine, this);
                if (n != 0)
                    return false;

                return true;
            }
            return false;
        }

        bool Stop()
        {
            if (_status == STATUS::RUNNING)
            {
                int n = ::pthread_cancel(_tid);
                if (n != 0)
                    return false;

                _status = STATUS::STOP;
            }
            return false;
        }

        bool Join()
        {
            // 只有处于joinable状态才能join
            if (_joinable)
            {
                int n = ::pthread_join(_tid, nullptr);
                if (n != 0)
                    return false;

                _status = STATUS::STOP;
                return true;
            }
            return false;
        }

        void Detach()
        {
            EnableDetach();
            pthread_detach(_tid);
        }

        bool IsJoinable() const { return _joinable; }
        std::string getName() const { return _name; }
        ~thread() {}

    private:
        pthread_t _tid;
        std::string _name;
        pid_t _pid;
        bool _joinable;
        func_t _func;
        STATUS _status;
    };
}


namespace thread_v2
{
    static int gnumber = 1;
    enum STATUS
    {
        NEW,
        RUNNING,
        STOP
    };

    template <class T>
    class thread
    {
        using func_t = std::function<void(T)>;
    private:
        static void *routinue(void *arg)
        {
            thread<T> *t = static_cast<thread<T> *>(arg);
            t->_status = STATUS::RUNNING;
            t->_func(t->_data);
            return nullptr;
        }
        void EnableJoinable() { _joinable = false; }

    public:
        thread(func_t func, T data)
            : _func(func), _status(STATUS::NEW), _joinable(true), _pid(getpid()),_data(data)
        {
            _name = "thread_v2-" + std::to_string(gnumber++);
        }

        bool Start()
        {
            if (_status != STATUS::RUNNING)
            {
                int n = ::pthread_create(&_tid, nullptr, routinue, this);
                if (n != 0)
                    return false;

                _status = STATUS::RUNNING;
                return true;
            }
            return false;
        }

        bool Stop()
        {
            if (_joinable)
            {
                int n = ::pthread_join(_tid, nullptr);
                if (n != 0)
                    return false;

                _status = STATUS::STOP;
                _joinable = false;
                return true;
            }
            return false;
        }

        bool Join() { return Stop(); }

        void Detach()
        {
            EnableJoinable();
            pthread_detach(_tid);
        }

        std::string getName() const { return _name; }
        bool IsJoinable() const { return _joinable; }
        ~thread() {}

    private:
        pthread_t _tid;
        std::string _name;
        STATUS _status;
        func_t _func;
        pid_t _pid;
        bool _joinable;
        T _data;
    };
}
