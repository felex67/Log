#pragma once

#include "headers/modules/Zipper.hpp"

namespace modules {
    class Config : private Zipper {
    public:
        struct group : Zipper::group {
            group(const char *CfgGroupName, const size_t __SizeofConfigGroup);
            ~group();
        };
        struct instance : public Zipper::instance {
            instance(const char *ConfigName, const char *ConfigPath, const size_t SizeofConfig);
            ~instance();
        };
        Config();
        ~Config();
        virtual const u_int8_t* pack(Zipper::instance *Inst);
        virtual int unpack(Zipper::instance *Inst);
    };
};

typedef modules::Config::instance cfg_instance_t;
typedef modules::Config::group cfg_group_t;

typedef modules::Zipper::zipp_int32 cfg_int32_t;
typedef modules::Zipper::zipp_uint32 cfg_uint32_t;
typedef modules::Zipper::zipp_int64 cfg_int64_t;
typedef modules::Zipper::zipp_uint64 cfg_uint64_t;
typedef modules::Zipper::zipp_float cfg_float_t;
typedef modules::Zipper::zipp_double cfg_double_t;
typedef modules::Zipper::zipp_cstring cfg_string_t;
