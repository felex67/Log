#pragma once

#include "headers/parser.h"

struct config_var_base {
    const char *Var;
    const char *Grp;
    u_int64_t data;
    config_var_base(const char *Var, const char *Grp) : Var(Var), Grp(Grp), data(0) {}
    config_var_base(const config_var_base &V) : Var(V.Var), Grp(V.Grp), data(V.data) {}
    config_var_base() : Var(nullptr), Grp(nullptr), data(0) {}

    config_var_base& operator = (const config_var_base &O) {
        Var = O.Var;
        Grp = O.Grp;
        data = O.data;
        return *this;
    }
    
    int operator () (const ConfigParser *Parser) const { return this->getvar(Parser); }
    virtual int getvar(const ConfigParser *Parser) const = 0;
};

struct cfint : config_var_base {
    cfint(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const { return Parser->parse_i((int*)&data, Parser, Var, Grp); }
    int& operator * () const { return *((int*)&0); }
};
struct cfuint : config_var_base {
    cfuint(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const { return Parser->parse_ui((unsigned int*)&data, Parser, Var, Grp);; }
    unsigned int& operator * () const { return *((unsigned int*)&data); }
};
struct cflong : config_var_base {
    cflong(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const { return Parser->parse_l((long*)&data, Parser, Var, Grp);; }
    long& operator * () const { return  *((long*)&data); }
};

struct cftime_t : cflong {};

struct cfulong : config_var_base {
    cfulong(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const { return Parser->parse_ul((unsigned long*)&data, Parser, Var, Grp);; }
    unsigned long operator * () const { return  *((unsigned long*)&data); }
};

struct cfg_size_t : cfulong {};

struct cffloat : config_var_base {
    cffloat(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const { return Parser->parse_f((float*)&data, Parser, Var, Grp);; }
    float operator * () const { return  *((float*)&data); }
};
struct cfdouble : config_var_base {
    cfdouble(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const { return Parser->parse_d((double*)&data, Parser, Var, Grp);; }
    double operator * () const { return  *((double*)&data); }
};
struct cfstring : config_var_base {
    cfstring(const char *Var, const char *Grp) : config_var_base(Var, Grp) {}
    virtual int getvar(const ConfigParser *Parser) const {
        data = Parser->parse_str((const char*)&data, Parser, Var, Grp);
        return (*((char**)&data) != 0 ? 0 : -1);
    }
    const char* operator * () const { return  *((const char**)&data); }
};

struct __log_config {
    int init(const char*) { return -1; }
    //
    struct _Mode {
        const cfstring RunAs;
    } Mode;
    
    //
    struct _LifeTime {
        const cflong DayTime;
        const cflong FileTime;
        const cfulong FileSize;
        const cflong KickAfter;
    } LifeTime;
    //
    struct _BuffSz {
        const cfint Date;
        const cfint Time;
        const cfint ActualName;
    } BufferSize;
    //
    struct _Format {
        const cfstring PathDate;
        const cfstring PathTime;
        const cfstring MsgTime;
        const cfstring Message;
        const cfstring RelPath;
        const cfstring RelName;
        const cfstring RelMap;
    } Format;
    //
    struct _Pipe {
        const cfstring Path;
    } Pipe;
    //
    struct _Server {
        const cfstring listAddr;
        const cfuint listPort;
    } Server;
};

__log_config LogConfig = {
    .Mode = {
        .RunAs = {"RunType", "Mode"}
    },
    .LifeTime = {
        .DayTime = { "DayTime", "LifeTime" },
        .FileTime = { "FileTime", "LifeTime"  },
        .FileSize = { "FileSize", "LifeTime" },
        .KickAfter = { "KickAfter", "LifeTime" }
    },
    . BufferSize = {
        .Date = { "Date", "BufferSize" },
        .Time = { "Time", "BufferSize" },
        .ActualName = { "ActualName", "BufferSize" },

    },
    .Format = { 
        .PathDate = { "PathDate", "Format" },
        .PathTime = { "PathTime", "Format" },
        .MsgTime = { "MsgTime", "Format" },
        .Message = { "Message", "Format" },
        .RelPath = { "RealPath", "Format" },
        .RelName = { "RelativeName", "Format" },
        .RelMap = { "RelativeMap", "Format" }
    },
    .Pipe = { .Path = { "PipePath", "Pipe" } },
    .Server = { 
        .listAddr = { "ListenAddr", "Server" },
        .listPort = { "ListenPort", "Server" }
    }
};
