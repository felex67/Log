#include <cstdio>
#include <time.h>

#include "headers/exception.hpp"

namespace __errorlog {
    const char __format_exception[] = "[%s]\t%s.%lu \"%s\"\tmodule:'%s' file:%s:%i\n";
    FILE *__handle::errorlog = fopen("log/error.log", "ab");
    __handle* __handle::main_inst = nullptr;
    __handle::__handle() {
        if (main_inst == nullptr) main_inst = this;
    }
    __handle::~__handle() { if (main_inst == this) fclose(errorlog); }
    void __handle::addlog(const char* str)  const noexcept {
        if (errorlog != nullptr) fputs(str, errorlog);
    }
    const __handle handle;
};

char exception::buff[1024];
char exception::tbuff[32];
bool exception::logerrors = 0;

exception::exception(const char *file, const int line, const char *What, const char *Module)
    : __buff(new char[1024])
{
    if (nullptr != __buff) {
        timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        strftime(tbuff, 32, "%Y-%m-%d %X", localtime(&ts.tv_sec));
        sprintf(__buff, __errorlog::__format_exception, "Exception", tbuff, ts.tv_nsec, What, Module, file, line);
    }
}
exception::~exception() {
    if (__buff) {
        if (logerrors) {
            __errorlog::handle.addlog(__buff);
        }
        delete[] __buff;
        __buff = nullptr;
    }
}
const char* exception::what() const noexcept {
    return __buff;
}
void exception::perror() const noexcept {
    if (nullptr != __buff) ::perror(__buff);
}
