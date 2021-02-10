#pragma once

#include "headers/parser.h"

struct __log_config {
    __log_config(const char*);
    ~__log_config();
    //
    mutable size_t Mode;
    //
    mutable time_t LifeTime_DayTime;
    mutable time_t LifeTime_FileTime;
    mutable size_t LifeTime_FileSize;
    mutable time_t LifeTime_KickAfter;
    //
    mutable size_t BuffSize_Date;
    mutable size_t BuffSize_Time;
    mutable size_t BuffSize_ActName;
    //
    mutable char *Format_PathDate;
    mutable char *Format_PathTime;
    mutable char *Format_MsgTime;
    mutable char *Format_Message;
    mutable char *Format_RelPath;
    mutable char *Format_RelName;
    mutable char *Format_RelMap;
    //
    mutable char *Pipe_Path;
    //
    mutable char *Server_listAddr;
    mutable size_t Server_listPort;
protected:
    char *Buff;
};