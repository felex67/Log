#pragma once

#include "headers/config.hpp"

namespace log {
    struct _Mode : config::group {
        _Mode(const char *Name) : config::group(Name) {}
        mutable cfstring RunAs = { "RunAs" };
    };
    struct _LifeTime : config::group {
        _LifeTime(const char *Name) : config::group(Name) {}
        mutable cflong DayTime = { "DayTime" };
        mutable cflong FileTime = { "FileTime" };
        mutable cfulong FileSize = { "FileSize" };
        mutable cflong KickAfter = { "KickAfter" };
    };
    struct _BuffSz : config::group {
        _BuffSz(const char *Name) : config::group(Name) {}
        mutable cfint Date = {"Date" };
        mutable cfint Time = { "Time" };
        mutable cfint ActualName = { "ActualName" };
    };
    struct _Format : config::group {
        _Format(const char *Name) : config::group(Name) {}
        mutable cfstring PathDate = { "PathDate" };
        mutable cfstring PathTime = { "PathTime" };
        mutable cfstring MsgTime = { "MsgTime" };
        mutable cfstring Message = { "Message" };
        mutable cfstring RelPath = { "RelativePath" };
        mutable cfstring RelName = { "RelativeName" };
        mutable cfstring RelMap = { "RelativeMap" };
    };
    struct _Pipe : config::group {
        _Pipe(const char *Name) : config::group(Name) {}
        mutable cfstring Path = { "Path" };
    };
    struct _Server : config::group {
        _Server(const char *Name) : config::group(Name) {}
        mutable cfstring listAddr = { "ListenAddr" };
        mutable cfuint listPort = { "ListenPort" };
    };
};

struct __log_config : config::instance {
    log::_Mode Mode = { "Mode" };
    log::_BuffSz BuffSize = { "BufferSize" };
    log::_Format Format = { "Format" };
    log::_Pipe Pipe = { "Pipe" };
    log::_Server Server = { "Server" };
} LogCfg;
