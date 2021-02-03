#pragma once
#include <linux/limits.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "log/__log_packet.h"

namespace LOG {
    enum log_emsgid {
        LOG_MSGID_REGINST,
        LOG_MSGID_UNREGINST,
        LOG_MSGID_PUSH,
        LOG_MSGID_REQEXIT,
        LOG_SIZEOF_MSGID
    };
    struct header : __log_packet_header {
        header(u_int16_t sz, u_int16_t id, const char* Owner);
    };
    struct RegInstance : header {
    };
    struct UnregInstance : header {
    };
    struct addMessage : header {
        struct timespec ts;
        char Msg[PIPE_BUF - sizeof(header) - sizeof(timespec)];
    };
    struct RequestLoggerExit : header {
    };
    union log_frames_un {
        RegInstance RegInst;
        UnregInstance UnregInst;
        addMessage addMsg;
        RequestLoggerExit ReqLoggerExit;
        u_int8_t bytes[PIPE_BUF];
    };
};