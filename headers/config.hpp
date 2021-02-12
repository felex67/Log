#pragma once

#include <stdio.h>
#include <cstring>
#include "headers/parser.h"

namespace config {
    struct dyn_container {
        inline dyn_container(const char *array, const size_t size) : array(array), size(size) {}
        inline dyn_container(dyn_container &dc) : array(dc.array), size(dc.size) { *((char**)&dc.array) = nullptr; }
        inline ~dyn_container() { if (array != nullptr) delete[] array; }

        const char *array;
        const size_t size;
    };
    struct group {
        inline group(const char *Name);
    private:
        const char *GrpName;
        size_t VarCnt;
    };
    struct instance {
        inline instance(const char *CfgName);
    private:
        const char *CfgName;
        size_t GrpCnt;
    };
struct variable {
    protected:
        const char *Var;
        mutable u_int64_t data;
    public:
        inline variable(const char *Var) : Var(Var), data(0) {}
        inline variable(const variable &V) : Var(V.Var), data(V.data) {}
        inline variable() : Var(nullptr), data(0) {}
        inline ~variable() {}

        inline variable& operator = (const variable &V) { Var = V.Var; data = V.data; return *this; }
        inline virtual int operator () (const ConfigParser *Parser, const char *Grp) const final { return this->getvar(Parser, Grp); }
        inline dyn_container& __to_string() { return this->tostring(); }
    protected:
        inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const = 0;
        inline virtual dyn_container& tostring() = 0;
    };
};


struct cfint : config::variable {
    inline cfint(const char *Var) : config::variable(Var) {}
    inline int& operator * () const { return *((int*)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const { return Parser->parse_i((int*)&data, Parser, Var, Grp); }
    inline virtual config::dyn_container& tostring() {
        char *string = new char[256];
        sprintf(string, "%s=%i\n", Var, *((int*)&data));
        return config::dyn_container(string, strlen(string));
    }
};
struct cfuint : config::variable {
    inline cfuint(const char *Var) : config::variable(Var) {}
    inline unsigned int& operator * () const { return *((unsigned int*)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const { return Parser->parse_ui((unsigned int*)&data, Parser, Var, Grp);; }
    inline virtual config::dyn_container& tostring() {
        char *string = new char[256];
        sprintf(string, "%s=%u\n", Var, *((int*)&data));
        return config::dyn_container(string, strlen(string));
    }
};
struct cflong : config::variable {
    inline cflong(const char *Var) : config::variable(Var) {}
    inline long& operator * () const { return  *((long*)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const { return Parser->parse_l((long*)&data, Parser, Var, Grp);; }
    inline virtual config::dyn_container& tostring() {
        char *string = new char[256];
        sprintf(string, "%s=%li\n", Var, *((int*)&data));
        return config::dyn_container(string, strlen(string));
    }
};

struct cftime_t : cflong {
    inline cftime_t(const char *Name) : cflong(Name) {}
};

struct cfulong : config::variable {
    inline cfulong(const char *Var) : config::variable(Var) {}
    inline unsigned long& operator * () const { return  *((unsigned long*)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const { return Parser->parse_ul((unsigned long*)&data, Parser, Var, Grp);; }
    inline virtual config::dyn_container& tostring() {
        char *string = new char[256];
        sprintf(string, "%s=%lu\n", Var, *((int*)&data));
        return config::dyn_container(string, strlen(string));
    }
};

struct cfsize_t : cfulong {
    inline cfsize_t(const char *Name) : cfulong(Name) {}
};

struct cffloat : config::variable {
    inline cffloat(const char *Var) : config::variable(Var) {}
    inline float& operator * () const { return  *((float*)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const { return Parser->parse_f((float*)&data, Parser, Var, Grp);; }
    inline virtual config::dyn_container& tostring() {
        char *string = new char[256];
        sprintf(string, "%s=%f\n", Var, *((int*)&data));
        return config::dyn_container(string, strlen(string));
    }
};
struct cfdouble : config::variable {
    inline cfdouble(const char *Var) : config::variable(Var) {}
    inline double& operator * () const { return  *((double*)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const { return Parser->parse_d((double*)&data, Parser, Var, Grp);; }
    inline virtual config::dyn_container& tostring() {
        char *string = new char[256];
        sprintf(string, "%s=%lf", Var, *((int*)&data));
        return config::dyn_container(string, strlen(string));
    }
};
struct cfstring : config::variable {
    inline cfstring(const char *Var) : config::variable(Var) {}
    inline const char*& operator * () const { return  *((const char**)&data); }
protected:
    inline virtual int getvar(const ConfigParser *Parser, const char *Grp) const {
        return Parser->parse_str((const char**)&data, Parser, Var, Grp);
    }
    inline virtual config::dyn_container& tostring() {
        size_t size = strlen(Var) + strlen(Val) + 1;
        char *string = new char[size];
        sprintf(string, "%s=\"%s\"", Var, *((int*)&data));
        return config::dyn_container(string, size);
    }
};