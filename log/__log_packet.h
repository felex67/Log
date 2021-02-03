#ifndef __log_packet_h__
#define __log_packet_h__ 1

#include <linux/limits.h>
#include <sys/types.h>

#define LOG_CONFIG_OWNER_BYTES 32
#define LOG_CONFIG_FRAME_BYTES 1456

    enum log_enum_packets {
        LOG_PACKET_REGINST,
        LOG_PACKET_UNREGINST,
        LOG_PACKET_PUSH,
        LOG_PACKET_REQEXIT,
        LOG_SIZEOF_ENUM_PACKET
    };

    struct __log_packet_header {
        u_int16_t sz;
        u_int16_t id;
        u_int32_t pad;
        char Owner[LOG_CONFIG_OWNER_BYTES];
    };
    struct __log_packet_header Log_Default_Header = {
        .sz = sizeof(__log_packet_header),
        .id = 0x00,
        .pad = sizeof(__log_packet_header::sz),
        .Owner = ""
    };
    /* Frames */
    struct log_packet_RegInstance {};
    struct log_packet_UnregInstance {};
    struct log_packet_addMessage {
        struct timespec ts;
        char Msg[LOG_CONFIG_FRAME_BYTES - sizeof(__log_packet_header) - sizeof(timespec)];
    };
    struct log_packet_RequestLoggerExit {};

    union log_union_packets {
        log_packet_RegInstance RegInst;
        log_packet_UnregInstance UnregInst;
        log_packet_addMessage addMsg;
        log_packet_RequestLoggerExit ReqLoggerExit;
        u_int8_t bytes[PIPE_BUF];
    };
    int log_pack_RegInstance(void *Packet, char *Owner);
    int log_pack_UnregInstance(void *Packet, char *Owner);
    int log_pack_addMessage(void* Packet, char *Owner, char *Msg);
    int log_pack_RequestLoggerExit(void *Packet, char *Owner);
#endif // __log_packet_h__