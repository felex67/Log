#include <string>
#include "headers/modules/Config.hpp"

namespace modules {
    Config::group::group(const char *Name, const size_t __Size)
        : Zipper::group(Name, __Size)
    {}
    Config::group::~group() {}
    Config::instance::instance(const char *Name, const char *Path, const size_t Size)
        : Zipper::instance(Name, Path, Size)
    {}
    Config::instance::~instance() {}
        
        const u_int8_t* Config::pack(Zipper::instance *Inst) {
            char *tbuff = new char[4096];
            __zipper::Container &cfg = reinterpret_cast<__zipper::Container&>(*Inst);
            size_t cnt = (cfg.inst.bcount - sizeof(Zipper::instance)) / sizeof(__zipper::__base);
            for (size_t i = 0; i < cnt; i++) {
                cfg.ent[i].__to_string()
            }
        }
        int unpack(Zipper::instance *Inst) {}
};
