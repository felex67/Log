#include "headers/modules/Zipper.hpp"

#include <cstdio>
#include <errno.h>

namespace modules {
    namespace __zipper {
        const char *const print_scan_mask[] = {
            "", "%i", "%u", "%li", "%lu", "%f", "%lf", "%s", "%s", "%s/%s", 0
        };

/** __zipper::entry_base */
        entry_base::entry_base(zv_type_t type, const char* name, const zv_data_t data)
            : type(type), name(nullptr), data(data)
        {
            if (name != nullptr) {
                const_cast<char*&>(this->name) = new char[strlen(name) + 1];
                if (this->name != nullptr) {
                    strcpy(const_cast<char*&>(this->name), name);
                }
            }
        }
        entry_base::~entry_base() {
            if (name) {
                delete[] name;
                const_cast<char*&>(this->name) = nullptr;
            }
        }
        ssize_t entry_base::__from_string(const char* Source, const char *mask = nullptr) {
            throw exception(__FILE__, __LINE__, "Попытка вызова чисто виртуального метода: __from_string()", "Zipper");
        };
        ssize_t entry_base::__to_string(void(const char*,const char*), const char *mask = nullptr) const {
            throw exception(__FILE__, __LINE__, "Попытка вызова чисто виртуального метода: __to_string()", "Zipper");
        };
        entry_base::operator bool () { return data != 0; }

/** __zipper::instance */
        instance::instance(const char *Name, const char *Path, const size_t ByteCnt)
            : __base(INSTANCE, Name, (ByteCnt - sizeof(*this)) / sizeof(__zipper::entry_base))
            , path(nullptr)
        {
            const_cast<char*&>(path) = new char[strlen(Path) + 1];
            if (nullptr != path) {
                strcpy(const_cast<char*&>(path), Path);
            }
        }
        instance::~instance() {
            if (nullptr != path) {
                delete[] path;
                const_cast<char*&>(path) = 0;
            }
        }
        ssize_t instance::__from_string(const char* Source, const char *mask = nullptr) {
            throw exception(__FILE__, __LINE__, "Попытка вызова чисто виртуального метода: __from_string()", "Zipper");
        };
        ssize_t instance::__to_string(void(const char*,const char*), const char *mask = nullptr) const {
            throw exception(__FILE__, __LINE__, "Попытка вызова чисто виртуального метода: __to_string()", "Zipper");
        };


/** __zipper::group */
        group::group(const char *Name, const size_t ByteCnt)
            : __base(GROUP, Name, (ByteCnt / sizeof(*this)))
        {}
        group::~group() {}
        ssize_t group::__from_string(const char* Source, const char *mask = nullptr) {
            throw exception(__FILE__, __LINE__, "Попытка вызова чисто виртуального метода: __from_string()", "Zipper");
        };
        ssize_t group::__to_string(void(const char*,const char*), const char *mask = nullptr) const {
            throw exception(__FILE__, __LINE__, "Попытка вызова чисто виртуального метода: __to_string()", "Zipper");
        };
    };

    Zipper::instance::instance(const char *FileName, const char *FilePath, const size_t __SizeofInst)
        : __zipper::instance(FileName, FilePath, __SizeofInst)
    {}
    Zipper::instance::~instance() {}

    Zipper::group::group(const char *GroupName, const size_t __SizeofGroup)
        : __zipper::group(GroupName, __SizeofGroup)
    {}

    Zipper::Zipper() {}
    Zipper::~Zipper() {}

    Zipper::zipp_int32::zipp_int32(const char *VarName, const int32_t Dflt)
        : __zipper::__base<int32_t>(__zipper::INT, VarName, Dflt)
    {}
    Zipper::zipp_uint32::zipp_uint32(const char *VarName, const u_int32_t Dflt)
        : __zipper::__base<u_int32_t>(__zipper::UINT, VarName, Dflt)
    {}
    Zipper::zipp_int64::zipp_int64(const char *VarName, const int64_t Dflt)
        : __zipper::__base<int64_t>(__zipper::LONG, VarName, Dflt)
    {}
    Zipper::zipp_uint64::zipp_uint64(const char *VarName, const u_int64_t Dflt)
        : __zipper::__base<u_int64_t>(__zipper::ULONG, VarName, Dflt)
    {}
    Zipper::zipp_float::zipp_float(const char *VarName, const float Dflt)
        : __zipper::__base<float>(__zipper::FLOAT, VarName, Dflt)
    {}
    Zipper::zipp_double::zipp_double(const char *VarName, const double Dflt)
        : __zipper::__base<double>(__zipper::DOUBLE, VarName, Dflt)
    {}
    Zipper::zipp_cstring::zipp_cstring(const char *VarName, const char *Dflt)
        : __zipper::__base<const char*>(__zipper::CSTRING, VarName, Dflt)
    {}
};