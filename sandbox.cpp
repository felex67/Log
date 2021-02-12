#include <sys/types.h>
#include <stdio.h>

struct ConfigParser;


int main() {
    size_t rounds = sizeof(__log_config) / sizeof(config_var_base);
    config_var_base *v = (config_var_base*)&LogConfig;
    for (size_t i = 0; i < rounds; i++) {
        v[i].say();
    }
    return 0;
}