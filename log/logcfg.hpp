#pragma once

#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

    const mode_t S_IRWXA = S_IRWXU | S_IRWXG | S_IRWXO;
    
/* types config */
    typedef u_int16_t logmsgsz_t;
    typedef u_int8_t logmsgbyte_t;
    typedef u_int8_t logmsgid_t;

    class __Logger {
    public:
        __Logger();
        ~__Logger();
    private:
        struct __Config {
            const char *const LogDir = { nullptr };
            const char *const Extension = { nullptr };
            const char *const Pipe = { nullptr };       /* "/tmp/.pipe-flx-logger" */
            const char *const NasterAddr = { nullptr };
            const u_int16_t MasterPort = { 0 };
        } Config;
        struct __Lifetime {
            /* Absolut/LogDir/LogName/Date/Timestamp/Timestamp.Ext */
            mutable time_t PathTime_m = { 6 * 60 };  //max path time
            mutable time_t ScopeTime_m = { 60 }; //max scope time
            mutable time_t FileTime_m = { 10 };  //max file time
            mutable size_t FileSize_b = { 5 * 1024 * 1024 };  //max filesize: 5 * 1024 * 1024 = 5 MBytes
        } LifeTime;
        struct __Timers {
            time_t Path = { 0 };
            time_t Scope = { 0 };
            time_t File = { 0 };
        } Timers;
        struct __Buffers {
            char Date[24] = {};
            char Time[24] = {};
            char ActualPath[1024] = {};
            char ActualName[32] = {};
            char Temp[1024] = {};
        } Buffers;
        struct __Format {
            const char *Date = "%Y-%m-%d";
            const char *Blok = "%H-%M-%S";
            const char *Msg = "%s.%li %s\n";
            const char *MsgTime = "%Y-%m-%d %X";
            // " %Date / %Time "
            const char *RelativePath = "%s/%s";
            // " %RelativePath / %Time . %LogFileExt "
            const char *RelativeName = "%s/%s.%s";
            const char *RelativeMap = "%s/%s/%s/%s";
        } Format;
    } Logger;

    namespace LOGMSG {
        struct header {
            header(const u_int16_t Sz, const u_int16_t Id, const char* Sender)
                : sz(Sz), id(Id), padding({4})
                , sender({})
            { strncpy(sender, Sender, sizeof(sender) - 1); }
            ~header() {}
            u_int16_t sz;
            u_int16_t id;
        private:
            const u_int8_t padding[4];
        public:
            char sender[32];
        };

        union __u_log_msg {
            header Header;
            char ch[PIPE_BUF];
        };
    };
    namespace LOG {
        
        struct {
            /* buffers config */
            struct {
                const int MsgTotal = PIPE_BUF;
                const int Header_Padding = sizeof(void*) - sizeof(logmsgsz_t) - sizeof(logmsgid_t);
            } Bytes;
            
            const int InstInBlock = 5;
            
            #define LOG_MSG_HEADER_BYTES (32)
            #define LOG_MSG_SENDER_BYTES (24)
            #define LOG_MSG_SENDER_LEN (LOG_MSG_SENDER_BYTES - 1)
            const int LOG_MSG_FRAME_BYTES = LOG_MSG_TOTAL_BYTES - LOG_MSG_HEADER_BYTES;
        } Buffer;
        namespace FORMAT {
            /* formatting */
            const char Date[] = "%Y-%m-%d";
            const char Blok[] = "%H-%M-%S";
            const char Msg[] = "%s.%li %s\n";
            const char MsgTime[] = "%Y-%m-%d %X";
            // " %Date / %Time "
            const char RelativePath[] = "%s/%s";
            // " %RelativePath / %Time . %LogFileExt "
            const char RelativeName[] = "%s/%s.%s";
            const char RelativeMap[] = "%s/%s/%s/%s";
        };
    };
    

    //
    
    //msg zerro
    typedef u_int8_t logmsgpad_t[LOG_MSG_HEADER_PADDING];
    //sender name
    typedef char logsender_chv[LOG_MSG_SENDER_BYTES];
    //identicate process killRequest
    const logmsgid_t  LOG_CONFIG_REQ_KILL_ID = 0xff;
    const logsender_chv LOG_CONFIG_REQ_KILL_NAME = "harakiri";
/* logheader_t */
    typedef struct __log_header {
        logmsgsz_t sz;
        logmsgid_t id;
        logmsgpad_t pad;
        logsender_chv sender;
    } logheader_t;

