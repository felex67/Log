#pragma once

#include "headers/parser.h"

struct config_var_base {
    char *Var;
    char *Grp;
    u_int64_t val;
    config_var_base(const char *Var, const char *Grp) : Var(Var), Grp(Grp), val(0) {}
    config_var_base(const config_var_base &V) : Var(V.Var), Grp(V.Grp), val(V.val) {}
    config_var_base() : Var(nullptr), Grp(nullptr), val(0) {}

    config_var_base& operator = (const config_var_base &O) {
        Var = O.Var;
        Grp = O.Grp;
        val = O.val;
    }
    int operator () (const ConfigParser *Parser);
    virtual int getvar(const ConfigParser *Parser);
};
struct cfg_int : config_var_base {
    cfg_int(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    int operator * ();
};
struct cfg_uint : config_var_base {
    cfg_uint(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    unsigned int operator * ();
};
struct cfg_long : config_var_base {
    cfg_long(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    long operator * ();
};
struct cfg_ulong : config_var_base {
    cfg_ulong(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    unsigned long operator * ();
};
struct cfg_float : config_var_base {
    cfg_float(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    float operator * ();
};
struct cfg_double : config_var_base {
    cfg_double(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    double operator * ();
};
struct cfg_string : config_var_base {
    cfg_double(const char *Var, const char *Grp);
    virtual int getvar(const ConfigParser *Parser);
    const char* operator * ();
};

class __log_config {
public:
    __log_config(const char*);
    ~__log_config();
    int init(const char*);
public:
    //
    struct _Mode {
        mutable cfg_string RunAs;
    } Mode;
    
    //
    struct _LifeTime {
        mutable cfg_long DayTime;
        mutable cfg_long FileTime;
        mutable cfg_ulong FileSize;
        mutable cfg_long KickAfter;
    } LifeTime;
    //
    struct _BuffSz {
        mutable cfg_int Date;
        mutable cfg_int Time;
        mutable cfg_int ActName;
    } BufferSize;
    //
    struct _Format {
        mutable cfg_string Format_PathDate;
        mutable cfg_string Format_PathTime;
        mutable cfg_string Format_MsgTime;
        mutable cfg_string Format_Message;
        mutable cfg_string Format_RelPath;
        mutable cfg_string Format_RelName;
        mutable cfg_string Format_RelMap;
    } Format;
    //
    struct _Pipe {
        mutable cfg_string Path;
    } Pipe;
    //
    struct _Server {
        mutable cfg_string Server_listAddr;
        mutable cfg_uint Server_listPort;
    } Server;
protected:
    char *Buff;
};
__log_config VrV = {
    .Mode = {
        config_entry<size_t>("RunType", "Mode")
    },
    .LifeTime = {

    }
    . LifeTime_DayTimeconfig_entry<time_t>("DayTime", "LifeTime"),
    config_entry<time_t>("FileTime", "LifeTime"),
    config_entry<size_t>("FileSize", "LifeTime"),
    config_entry<time_t>("KickAfter", "LifeTime"),
    config_entry<size_t>("Date", "BufferSize"),
    config_entry<size_t>("Time", "BufferSize"),
    config_entry<size_t>("ActualName", "BufferSize"),
    config_entry<char *>("PathDate", "Format"),
    config_entry<char *>("PathTime", "Format"),
    config_entry<char *>("MsgTime", "Format"),
    config_entry<char *>("Message", "Format"),
    config_entry<char *>("RelativePath", "Format"),
    config_entry<char *>("RelativeName", "Format"),
    config_entry<char *>("RelativeMap", "Format"),
    config_entry<char *>("PipePath", "Pipe"),
    config_entry<char *>("ListenAddr", "Srver"),
    config_entry<unsigned int>("ListenPort", "Srver"),
    config_var_base()
};