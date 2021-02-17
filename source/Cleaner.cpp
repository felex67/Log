#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fstream>

#include "headers/modules/Cleaner.hpp"

namespace modules {
    const u_int8_t *Cleaner::DfltLComm = reinterpret_cast<const u_int8_t*>(";#");
    const u_int8_t *Cleaner::DfltQts = reinterpret_cast<const u_int8_t*>("'\"");
    const u_int8_t *Cleaner::DfltNS = reinterpret_cast<const u_int8_t*>("\n");

    Cleaner::Cleaner()
        : In(nullptr), Wrk(nullptr)
        , LComm(DfltLComm), Quotes(DfltQts), NonSpc(DfltNS)
        , SzI(0)
        , SzW(0)
    {}
    Cleaner::~Cleaner() {
        clear();
    }
    void Cleaner::clear() {
        if (nullptr != In) delete[] In;
        if (nullptr != Wrk) delete[] Wrk;
        const_cast<u_int8_t*&>(In) = const_cast<u_int8_t*&>(Wrk) = nullptr;
        SzI = SzW = 0;
    }
    void Cleaner::reset() {
        clear();
        const_cast<const u_int8_t*&>(LComm) = DfltLComm;
        const_cast<const u_int8_t*&>(Quotes) = DfltQts;
        const_cast<const u_int8_t*&>(NonSpc) = DfltNS;
        SzI = SzW = 0;
    }
    char* Cleaner::release() {
        char *t = (reinterpret_cast<char*const>(In));
        const_cast<u_int8_t*&>(In) = nullptr;
        SzI = 0;
        return t;
    }
    int Cleaner::init(const char *FileName) {
        u_int8_t *tbytes;
        std::ifstream file;
        struct stat flstat;
        int result;
        if (-1 != (result = stat(FileName, &flstat))) {
            file.open(FileName, std::ios::binary);
            if (file.is_open()) {
                if (nullptr != (tbytes = new u_int8_t[flstat.st_size * 2 + 2])) {
                    reset();
                    file.read(reinterpret_cast<char*>(tbytes), SzI);
                    if ( flstat.st_size == static_cast<size_t>(file.tellg())) {
                        const_cast<u_int8_t*&>(In) = tbytes;
                        SzW = SzI = flstat.st_size;
                        const_cast<u_int8_t*&>(Wrk) = In + SzI + 1;
                        In[SzI] = Wrk[SzI] = 0;
                    }
                    else { delete[] tbytes; }
                }
                file.close();
            }
            else { errno = EINVAL; }
        }
        else { errno = EINVAL; }
        return (0 == result ? process() : result);
    }
    int Cleaner::init(const char* Bytes,const size_t Size) {
        size_t bSize = Size * 2;
        u_int8_t *_bytes;
        int result = -1;
        if (bSize && Bytes) {
            _bytes = new u_int8_t[bSize];
            if (_bytes) {
                reset();
                const_cast<u_int8_t*&>(In) = _bytes;
                const_cast<u_int8_t*&>(Wrk) = _bytes + Size;
                strncpy(reinterpret_cast<char*>(In), Bytes, Size);
                In[Size - 1] = Wrk[Size - 1] = 0;
                result = 0;
            }
        }
        else { errno = EINVAL; }
        return (0 == result ? process() : result);
    }
    int Cleaner::process() {
        char *pNew;
        int result = -1;
        if (-1 != (result = erase_comments())) {
            swap();             // allocated == Wrk (because swap())
            if (-1 != (result = erase_lines())) {
                if (SzW) {
                    pNew = new char[SzW + 1];
                    if (nullptr != pNew) {
                        try {
                            memcpy(pNew, Wrk, SzW + 1);
                            delete[] Wrk;
                            const_cast<u_int8_t*&>(Wrk) = nullptr;
                            SzI = SzW;
                            SzW = 0;
                            const_cast<u_int8_t*&>(In) = reinterpret_cast<u_int8_t*>(pNew);
                        }
                        catch (std::exception e) {}
                    }
                    else { swap(); }
                    result = 0;
                }
            }
        }            
        return result;
    }
    int Cleaner::set_lcomm(const char *Chars) {
        if (nullptr != Chars) {
            const_cast<const u_int8_t*&>(LComm) = reinterpret_cast<const u_int8_t*>(Chars);
            sort(const_cast<u_int8_t*>(LComm));
        }
        else { errno = EINVAL; }
        return 0;
    }
    int Cleaner::set_nonspace(const char *Chars) {
        if (nullptr != Chars) {
            const_cast<const u_int8_t*&>(NonSpc) = reinterpret_cast<const u_int8_t*>(Chars);
            sort(const_cast<u_int8_t*>(NonSpc));
        }
        else { errno = EINVAL; }
        return 0;
    }
    int Cleaner::set_quotes(const char *Chars) {
        if (nullptr != Chars) {
            const_cast<const u_int8_t*&>(Quotes) = reinterpret_cast<const u_int8_t*>(Chars);
            sort(const_cast<u_int8_t*>(Quotes));
        }
        else { errno = EINVAL; }
        return 0;
    }
    void Cleaner::swap() {
        size_t tSz = SzI;
        u_int8_t *t = In;
        const_cast<u_int8_t*&>(In) = Wrk;
        SzI = SzW;
        const_cast<u_int8_t*&>(Wrk) = t;
        SzW = tSz;
    }
    int Cleaner::erase_comments() {
        // pos & size
        size_t pi = 0, pw = 0;
        char *tchar = nullptr;
        u_int8_t Q = 0;
        const u_int8_t mchar = (u_int8_t)(*Quotes < *LComm ? *LComm : *Quotes);
        
        for (pi = 0, pw = 0; pi < SzI; pi++) {
            // помимо тех что больше максимального искомого символа исключаем алфавит и цифры
            if ((In[pi] > mchar) || ('A' <= In[pi] && 'Z' >= In[pi]) || (('0' <= In[pi]) && ('9' >= In[pi]))) {
                Wrk[(pw)++] = In[pi];
            }
            // это ковычка
            else if (nullptr != (*(const char**)&tchar = strchr(*(char**)&Quotes, (int)In[pi]))) {
                Q = static_cast<u_int8_t>(*tchar);
                do {
                    Wrk[(pw)++] = In[(pi)++];
                } while ((0 != In[pi]) && ((Q != In[pi]) || ('\\' == In[(pi) - 1])));
                Wrk[(pw)++] = In[pi];
            }
            // это строковый коммент
            else if (nullptr != (*(const char**)&tchar = strchr(*(const char**)&LComm, (int)In[pi]))) {
                while ('\n' != In[pi]) { ++(pi); }
                Wrk[(pw)++] = In[pi];
            }
            // возможно Си коммент 
            else if (In[pi] == '/') {
                if ('/' == In[(pi) + 1]) {
                    while ('\n' != In[pi]) { ++(pi); }
                    Wrk[(pw)++] = In[pi];
                }
                else if ('*' == In[(pi) + 1]) {
                    do { (pi++); } while (('*' != In[pi]) && ('/' != In[(pi) + 1]));
                    ++(pi);
                }
                else {
                    Wrk[(pw)++] = In[pi];
                }
            }
            // необходимый служебный символ: '\t' || '\n'
            else if ((' ' < In[pi]) || (nullptr != (*((const char**)&tchar) = strchr(*((const char**)&NonSpc), static_cast<int>(In[pi]))))) { Wrk[(pw)++] = In[pi]; }
        }
        Wrk[pw] = 0;
        if (Wrk[pw - 1] == '\n') Wrk[--pw] = 0;
        SzW = pw + 1;
        return 0;
    }
    int Cleaner::erase_lines() {
        size_t pi = 0, pw = 0, isz = SzI, len = 0;
        for ( ; pi < SzI; pi++) {
            if('\n' != In[pi]) { Wrk[pw++] = In[pi]; ++len; }
            else if (len > 0){ Wrk[pw++] = In[pi]; len = 0; }
        }
        Wrk[pw] = 0;
        if ('\n' == Wrk[pw - 1]) { Wrk[--(pw)] = 0; }
        SzW = pw + 1;
        return 0;
    }
    int Cleaner::sort(u_int8_t* bytes) {
        int i = 0, fwd = 1, swapped = 0;
        u_int8_t t;
        if (0 != bytes) {
            for ( i = 0, fwd = 1; (0 != bytes[i]); ) {
                if ((0 != bytes[i + 1]) && swapped) { fwd = 0; }
                if (bytes[i] < bytes[i + 1]) {
                    t = bytes[i];
                    bytes[i] = bytes[i + 1];
                    bytes[i + 1] = t;
                    swapped = 1;
                }
                if (i);
                else { swapped = 0; fwd = 1; }
                (fwd ? ++i : --i);
            }
            return 0;
        }
        return -1;
    }
    const char* Cleaner::c_str() const {
        return const_cast<const char*>(reinterpret_cast<char*>(In));
    }
};