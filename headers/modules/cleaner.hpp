#pragma once

#include <sys/types.h>

namespace modules {
    class Cleaner {
        // static
        static const u_int8_t *DfltLComm;
        static const u_int8_t *DfltQts;
        static const u_int8_t *DfltNS;
        // Instance
        const u_int8_t *const LComm;
        const u_int8_t *const Quotes;
        const u_int8_t *const NonSpc;

        u_int8_t *const In;
        u_int8_t *const Wrk;
        size_t SzI;
        size_t SzW;
    public:
        Cleaner();
        ~Cleaner();
    private:
        Cleaner(const Cleaner&);
        Cleaner& operator = (const Cleaner&);
    public:
        int init(const char *SourceFile);
        int init(const char *CString, const size_t SizeWithZero);
        int set_lcomm(const char *LineCommChars);
        int set_nonspace(const char *NonSpaceChars);
        int set_quotes(const char *QuotesChars);
        char* release();
        const char* c_str() const;
        void reset();
    private:
        int process();
        int erase_comments();
        int erase_lines();
        void clear();
        void swap();
        int sort(u_int8_t*);
    };

};