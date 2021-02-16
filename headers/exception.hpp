#pragma once

#include <sys/types.h>
#include <cstdio>
#include <exception>

namespace __errorlog {
    extern const char __format_exception[];
    class __handle {
        static FILE* errorlog;
        static __handle* main_inst;
    public:
        __handle();
        ~__handle();
        void addlog(const char*) const noexcept;
    };
    extern const __handle handle;
};

class exception : public std::exception {
    static char buff[1024];
    static char tbuff[32];
    char *__buff;
public:
    exception(const char *file, const int line, const char *What, const char *Module = nullptr);
    virtual ~exception();
    virtual const char* what() const noexcept;
    virtual void perror() const noexcept;
    static bool logerrors;
};

