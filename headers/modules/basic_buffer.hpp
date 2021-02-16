#pragma once

#include <sys/types.h>
#include <vector>

#include "headers/exception.hpp"


namespace modules {
    size_t memcopy(void *Dest, const void *Src, const size_t Bytes);

    template <typename T>
    /** Класс-шаблон буффера */
    struct basic_buffer {
        // Конструктор
        basic_buffer();
        // не копирует а перемещает буффер!!!
        basic_buffer(basic_buffer<T> &Buffer);
        // Деструктор
        ~basic_buffer();
        // Инициализатор
        bool init(const size_t Size);
        // Изменяет размер буффера
        bool trunc(const size_t Size);
        // отпускает массив
        void release(T* &Dest, size_t &DestSize);
        // Захватывает массив
        bool assign(T *Array, const size_t Size);
        // Сброс
        void reset();
        // Клонирует массив тогоже типа
        T& clone(const basic_buffer<T> &Buffer);
        // Меняет массивы местами
        T& swap(basic_buffer<T> &Buffer);
        // правда если массив иннициализирован!!!
        explicit operator const bool ();
        // Доступ к элементам массива по ссылке для работы с ними
        T& operator [] (const size_t N);
        // Константный доступ
        const T& operator [] (const size_t N) const;
        // Конкатенация двух однотипных массивов
        basic_buffer& operator += (const basic_buffer<T> &buff);
        // перемещение правого массива в левый
        T& operator = (basic_buffer<T>& Buffer);
    private:
        T *array;
        size_t size;
    };

    inline size_t memcopy(void *Dest, const void *Source, const size_t Bytes) {
        if ((nullptr != Dest) && (nullptr != Source)) {
            size_t rndszt = Bytes / sizeof(size_t);
            size_t rndb = Bytes % sizeof(size_t);
            size_t *d = reinterpret_cast<size_t*>(Dest);
            u_int8_t *td = reinterpret_cast<u_int8_t*>(reinterpret_cast<size_t*>(Dest) + rndszt);
            const size_t *s = reinterpret_cast<const size_t*>(Source);
            const u_int8_t *ts = reinterpret_cast<const u_int8_t*>(reinterpret_cast<const size_t*>(Source) + rndszt);
            for (size_t i = 0; i < rndszt; i++) {
                d[i] = s[i];
            }
            for (size_t i = 0; i < rndb; i++) {
                td[i] = ts[i];
            }
            return Bytes;
        }
        return 0;
    }

    template <typename T>
    basic_buffer<T>::basic_buffer() : array(nullptr), size(0) {}
    template <typename T>
    basic_buffer<T>::basic_buffer(basic_buffer<T> &b) : array(b.array), size(b.size) {
        b.array = nullptr;
        b.size = 0;
    }
    template <typename T>
    basic_buffer<T>::~basic_buffer() { reset(); }
    template <typename T>
    void basic_buffer<T>::reset() { if (nullptr != array) { delete[] array; array = nullptr; } size = 0; }
    template <typename T>
    bool basic_buffer<T>::init(const size_t Size) {
        T *narray = new T[Size];
        if (nullptr != narray) {
            if (nullptr != array) delete[] array;
            array = narray;
            size = Size;
            return true;
        }
        return false;
    }
    template <typename T>
    bool basic_buffer<T>::trunc(const size_t nsz) {
        T *narray = nullptr;
        size_t cpysize = (nsz > size ? nsz : size);
        bool result = false;
        narray = new T[nsz];
        if (nullptr != narray) {
            if (nullptr != array) {
                memcopy(narray, array, cpysize);
                delete[] array;
            }
            array = narray;
            size = nsz;
            return true;
        }
        return false;
    }
    template <typename T>
    bool basic_buffer<T>::assign(T *Src, const size_t SrcSz) {
        reset();
        array = Src;
        size = SrcSz;
        return true;
    }
    template <typename T>
    void basic_buffer<T>::release(T* &dest, size_t &sz) {
        dest = array;
        sz = size;
        array = nullptr;
        size = 0;
    }
    template <typename T>
    basic_buffer<T>::operator const bool () { return nullptr != array; }
    template <typename T>
    const T& basic_buffer<T>::operator [] (const size_t N) const {
        if (nullptr != array) {
            if (N < size) {
                return array[N];
            }
            else throw exception(__FILE__, __LINE__, "Out of range", "basic_buffer");
        }
        else {
            else throw exception(__FILE__, __LINE__, "Try access nullptr", "basic_buffer");
        }
    }
    template <typename T>
    T& basic_buffer<T>::operator[] (const size_t pos) {
        if (nullptr != array) {
            if (N < size) {
                return array[N];
            }
            else throw exception(__FILE__, __LINE__, "Out of range", "basic_buffer");
        }
        else {
            else throw exception(__FILE__, __LINE__, "Try access nullptr", "basic_buffer");
        }
    }
    template <typename T>
    basic_buffer<T>& basic_buffer<T>::operator += (const basic_buffer<T> &b) {
        if (size && b.size) {
            size_t nsize = size + b.size;
            T* narray = new T[nsize];
            if (nullptr != narray) {
                memcopy(narray, array, size);
                memcopy(narray + size, b.array, b.size);
                delete[] array;
                array = narray;
                size = nsize;
            }
            else throw exception(__FILE__, __LINE__, "Bad alloc", "basic_buffer");
        }
        else throw exception(__FILE__, __LINE__, "Bad args", "basic_buffer");
        return *this;
    }
    template <typename T>
    T& basic_buffer<T>::operator = (basic_buffer& b) {
        if (array) delete[] array;
        array = b.array;
        b.array = nullptr;
        size = b.size;
        b.size = 0;
    }
    template <typename T>
    T& basic_buffer<T>::clone(const basic_buffer<T> &b) {
        reset();
        if (nullptr != b.array) {
            array = new T[b.size];
            if (nullptr != na) {
                if (nulltpr != array) { delete[] array; }
                memcopy(na, b.array, b.size);
                array = na;
                size = b.size;
            }
            else {
                throw exception(__FILE__, __LINE__, "Bad alloc", "base_buffer");
            }
        }
    }
    template <typename T>
    T& basic_buffer<T>::swap(basic_buffer<T> &b) {
        // b >> temp
        T* ta = b.array;
        size_t ts = b.size;
        // this >> b
        b.array = array;
        b.size = size;
        // temp >> this
        array = ta;
        size = ts;
        return *this;
    }

/*
    /// streamed buffer 
    struct memory_block {
        u_int8_t bytes[4086];
        size_t busy;
    };
    class stremed_buffer {
        std::vector<memory_block> v;
        size_t vpos;
        size_t bpos;
    public:
        stremed_buffer();
        ~stremed_buffer();
        template <typename T>
        void assign(T *Array, size_t Size);
        template <typename T>
        bool push(const T &Data);
        template <typename T>
        size_t push_range(const T *Data, size_t Count);
        template <typename T>
        bool pop(T &Dest);
        template <typename T>
        size_t pop_range(T *Dest, const size_t Count);
        bool eof();
    };
    */
};