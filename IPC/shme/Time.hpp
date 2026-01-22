#pragma once
#include <iostream>
#include <ctime>
#include <string>

std::string GetCurTime(){
    time_t t = time(nullptr);
    struct tm* cur = ::localtime(&t);

    char curtime[32];
    snprintf(curtime, sizeof(curtime), "%d-%d-%d %d:%d:%d",
             cur->tm_year + 1900,
             cur->tm_mon + 1,
             cur->tm_mday,
             cur->tm_hour,
             cur->tm_min,
             cur->tm_sec);
    return curtime;
}
