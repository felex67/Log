#include <sys/types.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <time.h>
#include <string>

#include "headers/modules/Config.hpp"

//using namespace modules;

struct TestConfig {
    cfg_instance_t __header = {"test.cfg", "config/", sizeof(TestConfig)};
    struct _Format {
        cfg_group_t __header = {"Fomat", sizeof(_Format)};
        cfg_string_t PathDate = {"PathDate", "%Y-%m-%d"};
        cfg_string_t PathTime = {"PathTime", "%X"};
    };
    struct _Server {
        cfg_group_t __header = {"Server", sizeof(_Server)};
        cfg_string_t SrvAddr = {"SrvAddr", "localhost"};
        cfg_uint32_t SrvPort = {"SrvPort", 39990};
    };
} test;

int main (const int argc, const char *argv[]) {
    modules::Config cfghandle;
    cfghandle.save(test.__header);
    return 0;
}