#pragma once

#include <linux/limits.h>
#include <sys/types.h>
#include <cstring>
#include <time.h>

    constexpr size_t LOG_OWNER_NAME_BYTES = 32;
    constexpr size_t LOG_FRAME_MAX_SIZE = 1456;
    
    enum log_enum_packets {
        LOG_PACKET_REGINST,
        LOG_PACKET_UNREGINST,
        LOG_PACKET_PUSH,
        LOG_PACKET_REQEXIT,
        LOG_SIZEOF_ENUM_PACKET
    };

    struct __log_packet_header {
        inline __log_packet_header() : sz(0), id(-1), pad(sizeof(__log_packet_header::pad)), Owner() {
            memset(Owner, 0, sizeof(Owner));
        }
        inline __log_packet_header(const u_int16_t Sz, u_int16_t id, const char *Owner)
            : sz(Sz), id(id), pad(sizeof(__log_packet_header::pad))
        {
            strncpy(this->Owner, Owner, sizeof(this->Owner) - 1);
            this->Owner[sizeof(this->Owner) - 1] = 0;
        }
        inline ~__log_packet_header();
        u_int16_t sz;
        u_int16_t id;
        u_int32_t pad;
        char Owner[LOG_OWNER_NAME_BYTES];
    };

    /* Frames */
    struct log_packet_RegInstance : __log_packet_header {
        inline log_packet_RegInstance(const char *Owner)
            : __log_packet_header(sizeof(*this), LOG_PACKET_REGINST, Owner)
        {}
        inline ~log_packet_RegInstance() {}
    };
    struct log_packet_UnregInstance : __log_packet_header {
        inline log_packet_UnregInstance(const char *Owner)
            : __log_packet_header(sizeof(*this), LOG_PACKET_UNREGINST, Owner)
        {}
        inline ~log_packet_UnregInstance() {}
    };
    struct log_packet_addMessage : __log_packet_header {
        inline log_packet_addMessage(const char *Owner, const char *LogMsg)
            : __log_packet_header(0, LOG_PACKET_PUSH, Owner)
        {
            size_t sz = strlen(LogMsg);
            if (sz > sizeof(Msg) - 1) { sz = sizeof(Msg) - 1; }
            memcpy(Msg, LogMsg, sz);
            Msg[sz] = 0;
            this->sz = sz + sizeof(__log_packet_header) + sizeof(ts);
            clock_gettime(CLOCK_REALTIME, &ts);
        }
        inline ~log_packet_addMessage() {}
        struct timespec ts;
        char Msg[LOG_FRAME_MAX_SIZE - sizeof(__log_packet_header) - sizeof(ts)];
    };

    struct log_packet_RequestLoggerExit : __log_packet_header {
        inline log_packet_RequestLoggerExit(const char *Owner)
            : __log_packet_header(sizeof(*this), LOG_PACKET_REQEXIT, Owner)
        {}
        inline ~log_packet_RequestLoggerExit() {}
    };

    union __log_packet_frames {
        __log_packet_header Header;
        log_packet_RegInstance RegInst;
        log_packet_UnregInstance UnregInst;
        log_packet_addMessage addMsg;
        log_packet_RequestLoggerExit ReqLoggerExit;
        u_int8_t bytes[LOG_FRAME_MAX_SIZE];
    };