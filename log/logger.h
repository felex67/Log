#ifndef __logger_h__
#define __logger_h__ 1

    #include <linux/limits.h>
    #include <sys/types.h>

    #include "log/__log_packet.h"

    struct Logger_UserOptions {
        char *RootDir;
        char *PathFormat;
        time_t BlockTime;
        time_t FileTime;
        size_t FileSize;
    };
    struct Logger_UserOptions Log_Initial_Options = {
        .RootDir = 0,
        .PathFormat = 0,
        .BlockTime = 0,
        .FileTime = 0,
        .FileSize = 0,
    };
    struct Logger_User {
        time_t ActiveTimer;
        size_t FSize;
        char Name[LOG_CONFIG_OWNER_BYTES];
        struct UserSettings Opt;
        int fd;
        int stat;
    };
    struct Logger_User Log_Initial_User {
        .ActiveTimer = 0,
        .FSize = 0,
        .Name = "",
        .Opt = Log_Initial_Options,
        .fd = -1,
        .stat = 0
    };

    struct Logger_Buffer {
        char *Buff;
        size_t size;
    };
    struct Logger_Buffer Log_Initial_Buffer = { .Buff = 0, .size = 0 };
    
    struct Logger_State {
        struct Logger_User *Clients;
        struct Logger_Buffer Date;
        struct Logger_Buffer Time;
        struct Logger_Buffer Root;
        struct Logger_Buffer FName;
    };
    struct Logger_Formats {
        char *Date;
        char *Time;
        char *LogMsg;
        char *MsgTime;
    };
    struct Logger_State Logger_Initial_State = {
        .Clients = 0,
        .Date = Log_Initial_Buffer,
        .Time = Log_Initial_Buffer,
        .Root = Log_Initial_Buffer,
        .FName = Log_Initial_Buffer
    };
    
    int logger_init_Users(struct Logger_User *Clients, const size_t Count);
    int logger_init_Buffer(struct Logger_Buffer *Buffer, const size_t Size);
    int logger_init_State(struct struct Logger_State * State, const size_t Count);

    void logger_delete_UserOpt(struct UserSet *);
    void logger_delete_Logger_Buffer(struct Logger_Buffer *);
    void logger_delete_LogUser(struct LogUser *);
    void logger_delete_LoggerState(struct LoggerState *);

#endif //__logger_h__