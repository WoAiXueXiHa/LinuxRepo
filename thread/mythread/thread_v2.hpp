#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <functional>

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
