#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "headers/modules/config.hpp"

namespace modules {
    namespace __config {
        const char *scan_masks[] = {
            "", "=%i", "=%u", "=%li", "=%lu", "=%f", "=%lf", "=\"%[^\"]s", "", ""
        };
        const char *print_masks[] = {
            "", "=%i", "=%u", "=%li", "=%lu", "=%f", "=%lf", "=\"%s", "", ""
        };
        const char *errsprintf[] = {
            "Невозможно открыть или создать файл: '%s'",
            "Невозможно открыть или создать каталог: '%s'",
            "Нет такого файла:'%s'",
            "Нет такого каталога:'%s'"
        };
        enum __cfg_errormsg {
            ERR_BADFNAME,
            ERR_BADDIR,
            ERR_NOFILE,
            ERR_NODIR
        };
    };
    void handle(const char*, const char*);
    Config::group::group(const char *Name, const size_t __Size)
        : Zipper::group(Name, __Size)
    {}
    Config::group::~group() {}
    Config::instance::instance(const char *Name, const char *Path, const size_t Size)
        : Zipper::instance(Name, Path, Size)
    {}
    Config::instance::~instance() {}

    Config::Config() {}
    Config::~Config() {}
    
    int Config::save(__zipper::entries &cfg) {
        std::string sbuff = cfg.inst.path;
        char *cbuff = new char[2048];
        struct stat cfgstat;
        int errcnt = 0;
        int result = open(sbuff.c_str(), O_DIRECTORY, S_IRWXU | S_IRWXG | S_IRWXO);
        if (-1 != result);
        else {
            if (errno == ENOTDIR) {
                unlink(sbuff.c_str());
            }
            if (-1 != (result = mkdir(sbuff.c_str(), S_IRWXU | S_IRWXG |  S_IRWXO)));
            else {
                sprintf(cbuff, __config::errsprintf[__config::ERR_BADDIR], sbuff.c_str());
                throw exception(__FILE__, __LINE__, const_cast<const char*&>(cbuff), "Config");
            }
        }
        if (-1 != result) close(result);
        sbuff += cfg.inst.name;
        if (-1 != (result = open(sbuff.c_str(), O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)));
        else {
            sprintf(cbuff, __config::errsprintf[__config::ERR_BADFNAME], sbuff.c_str());
            throw exception(__FILE__, __LINE__, const_cast<const char*&>(cbuff), "Config");
        }
        sbuff = "# Конфигурационный файл: '";
        sbuff += cfg.inst.name;
        sbuff += "' (создан автоматически)\n";
        
        for (size_t i = 0; i < cfg.inst.data; i++) {
            try {
                cfg.v[i].__to_string(sbuff, __config::print_masks[cfg.v[i].type]);
            }
            catch (std::exception &e){
                ++errcnt;
                perror(e.what());
            }
            sbuff += "\n";
        }
        write (result, sbuff.c_str(), sbuff.length());
        close (result);
        return errcnt;
    }
    int unpack(__zipper::entries *Inst) { return -1; }
};
