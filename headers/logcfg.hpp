#pragma once

#include "headers/modules/config.hpp"

namespace log {
    struct __config {
        const cfg_instance_t header = { "log", "config", sizeof(__config) };

        struct _Mode {
            const cfg_group_t header = { "Mode", sizeof(Mode) };
            mutable cfg_int32_t RunAs = { "RunAs", 0 };
            
        } Mode;
        

        struct _LifeTime {
            const cfg_group_t header = { "LifeTime", sizeof(_LifeTime) };

            mutable cfg_int64_t DayTime = { "DayTime", 6 };
            mutable cfg_int64_t FileTime = { "FileTime", 10 };
            mutable cfg_uint64_t FileSize = { "FileSize", 5 };
            mutable cfg_int64_t KickAfter = { "KickAfter", 60 };
        } LifeTime;

        struct _BuffSz {
            const cfg_group_t header = { "BufferSize", sizeof(_BuffSz) };

            mutable cfg_int32_t Date = {"Date", 24 };
            mutable cfg_int32_t Time = { "Time", 24 };
            mutable cfg_int32_t ActualName = { "ActualName", 64 };
        } BuffSize;

        struct _Format {
            const cfg_group_t header = { "Format", sizeof(_Format) };

            mutable cfg_string_t PathDate = { "PathDate", "%Y-%m-%d" };
            mutable cfg_string_t PathTime = { "PathTime", "%H-%M-%S" };
            mutable cfg_string_t MsgTime = { "MsgTime", "%Y-%m-%d %X.%lu" };
            mutable cfg_string_t Message = { "Message", "%s %s\n" };
            mutable cfg_string_t RelPath = { "RelativePath", "%s/%s" };
            mutable cfg_string_t RelName = { "RelativeName", "%s/%s.%s" };
            mutable cfg_string_t RelMap = { "RelativeMap", "NDT" };
        } Format;

        struct _Pipe {
            cfg_group_t header = { "Pipe", sizeof(_Pipe) };

            mutable cfg_string_t Path = { "Path", ".temp/flxwd-logger-pipe123" };
        } Pipe;

        struct _Server {
            cfg_group_t header = { "Server", sizeof(_Server) };

            mutable cfg_string_t listAddr = { "ListenAddr", "localhost" };
            mutable cfg_uint32_t listPort = { "ListenPort", 39990 };
        } Server;
    } Config;
};
