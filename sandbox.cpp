#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

const char *_gdb = "-g";
const char *_obj = "-c";
const char *_incl = "-I/home/felex67/work/Log/";
const char *_src = "/home/felex67/work/Log/hello.c";
const char *_name = "/home/felex67/work/Log/hello";
const char *__args[] = { _src, _incl, "-o", _name, 0};

enum __POS {
    GDB,
    OBJECTIVE,
    SOURCE,
    INCLUDES,
    OUTNAME,
    SIZEOF_POS
};
extern "C" {
    
};
int main (const int argc, const char *argv[]) {
        char buff[2048];
        int returned;
    //    if (argc < 2) exit(-1);
        pid_t pid = fork();
    // compile
        if (0 == pid) {
            return execve("/bin/gcc", *(char*const**)&__args, environ);
        }
        else if (-1 != pid) {
            waitpid(pid, &returned, 0);
        }
        else { exit(-1); }
    // move
        return 0;
    }