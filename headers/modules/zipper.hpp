#pragma once

#include <sys/types.h>
#include <cstring>
#include <string>
#include <vector>

#include "headers/modules/cleaner.hpp"
#include "headers/exception.hpp"

namespace modules {
    namespace __zipper {
/** Типы переиенных */
        enum _e_vartype {
            INVALID,
            INT32,
            UINT32,
            INT64,
            UINT64,
            FLOAT,
            DOUBLE,
            CSTRING,
            GROUP,
            INSTANCE
        };
/** Маски для функций ряда 'printf' и 'scanf' */
        static const char *const __scan_mask[] = {
            "", "%i", "%u", "%li", "%lu", "%f", "%lf", "%s", "%[0-9_A-Za-z]s", "%[./0-9_A-Za-z-]s"
        };
        static const char *const __print_mask[] = {
            "", "%i", "%u", "%li", "%lu", "%f", "%lf", "%s", "%s", "%s%s"
        };
        /** после инициализации изменить будет невозможно()
         * Хотя, изменить то можно, но - не стоит, приведёт к ошибкам */
        typedef const ssize_t zv_type_t;
        typedef const char* zv_name_t;
/** базовый тип данных (instance, group и __data<T>) */
        typedef u_int64_t zv_data_t;
/** Базовый абстрактный класс переменной(в том числе и объектов-заголовков - 'instance' и 'group')
 * '__to_string()' и '__from_string()' в случае ошибки генерируют исключения:
 * 'class exception : public std::exception {};'
 * Следует заметить что прямой вызов из этого класса также сгенерирует исключение */
        struct entry_base {
        protected:
            // Конструктор: тип имя и данные(беззнаковое целое 64 бита)
            inline entry_base(zv_type_t type, const char* name, const zv_data_t data)
                : type(type)
                , name(nullptr)
                , data(data)
            {
                size_t len;
                if (nullptr != name) {
                    len = strlen(name);
                    this->name = new char[len + 1];
                    if (nullptr != this->name) {
                        strcpy(const_cast<char*&>(this->name), name);
                    }
                }
            }

            // Деструктор
            inline ~entry_base() { if (nullptr != name) { delete[] name; name = nullptr; } }
            /** Абстрактный метод сканирующий значение из строкового представления */
            inline virtual ssize_t __from_string(const char* Source, const char *mask = nullptr) {
                throw exception(__FILE__, __LINE__, "Обращение к чисто-виртуальному методу", "zipper");
            }
            // Абстрактный метод приводящий переменную в строковое представление
            inline virtual ssize_t __to_string(std::string &Dest, const char *mask = nullptr) const {
                throw exception(__FILE__, __LINE__, "Обращение к чисто-виртуальному методу", "zipper");
            }
            // Логическое представление переменной ('true' если 'data != 0')
            inline operator bool () { return (0 != data); }
            inline bool operator == (const char *Src) const {
                if ((nullptr != name) && (nullptr != Src)) {
                    return (0 == strncmp(name, Src, strlen(name)));
                }
                return false;
            }
        public:
            //
            zv_type_t type;
            zv_name_t name;
            zv_data_t data;
        };

/** Абстрактный класс объекта-заголовка: файл
 * Наследники должны реализовать методы (__from/__to)_string(),
 * в противном случае будет сгенерировано исключение!!! */
        struct instance : public entry_base {
            inline instance(const char *Name, const char *Path, const size_t ByteCnt)
                : entry_base(INSTANCE, Name, (ByteCnt - sizeof(*this)) / sizeof(__zipper::entry_base))
                , path(nullptr)
            {
                const_cast<char*&>(path) = new char[strlen(Path) + 1];
                if (nullptr != path) {
                    strcpy(const_cast<char*&>(path), Path);
                }
            }
            inline ~instance() {
                if (nullptr != path) {
                    delete[] path;
                    const_cast<char*&>(path) = 0;
                }
            }
            /** Абстрактный метод сканирующий значение из строкового представления */
            inline virtual ssize_t __from_string(const char* Source, const char *mask = nullptr) {
                throw exception(__FILE__, __LINE__, "Обращение к чисто-виртуальному методу", "zipper");
            }
            // Абстрактный метод приводящий переменную в строковое представление
            inline virtual ssize_t __to_string(std::string &Dest, const char *mask = nullptr) const {
                throw exception(__FILE__, __LINE__, "Обращение к чисто-виртуальному методу", "zipper");
            }
            zv_name_t path;
        };
/** Шаблонный класс переменной(в том числе и Объектов-заголовков: instance, group) */
        template <typename T>
        class __base : private entry_base {
        public:
            __base(const _e_vartype type, const char *name, const T data)
                : entry_base(type, name, reinterpret_cast<const u_int64_t&>(data))
            {}
            ~__base() {}
            const T& operator * () const { return reinterpret_cast<const T&>(const_cast<const u_int64_t&>(data)); }
        private:
            virtual ssize_t __from_string(const char* src, const char *mask = nullptr) {
                const char *msk = nullptr;
                if ((nullptr != src) && (INVALID < type) && (CSTRING > type)) {
                    msk = (nullptr != mask ? mask : __scan_mask[type]);
                    return sscanf(src, msk, reinterpret_cast<T*>(&data));
                }
                else if (nullptr != src) {
                    throw exception(__FILE__, __LINE__, "__base<T>::__from_string() Неизвестный тип", "Zipper");
                }
                else {
                    throw exception(__FILE__, __LINE__, "__base<T>::__from_string(Src) Src == nullptr", "Zipper");
                }
            }
            virtual ssize_t __to_string(std::string &Dest, const char* mask = nullptr) const {
                char *buff = nullptr;
                if ((INVALID < type) && (CSTRING > type)) {
                    buff = new char[1024];
                    if (buff != nullptr) {
                        sprintf(buff, ((nullptr == mask) ? __print_mask[type] : mask), reinterpret_cast<const T&>(data));
                        Dest += buff;
                        delete[] buff;
                    }
                    else { throw exception(__FILE__, __LINE__, "Недостаточно памяти...", "Zipper"); }
                }
                else { throw exception(__FILE__, __LINE__, "__base<T>::__to_string() Неизвестный тип", "Zipper"); }
                return 0;
            }
        protected:
            T& operator * () { return reinterpret_cast<T&>(data); }
            void setval(const T ndata) { data = reinterpret_cast<const zv_data_t&>(ndata); }
        };
        // специализация для [T=char*]
        template <>
        class __base<char*> : public entry_base {
        protected:
            __base(const _e_vartype type, const char *name, const char* data)
                : entry_base(type, name, 0)
            {
                char* cd = reinterpret_cast<char*&>(this->data);
                cd = new char[strlen(data) + 1];
                if (nullptr != cd) {
                    strcpy(cd, data);
                }
            }
        public:
            ~__base() {
                if (0 != data) {
                    delete[] reinterpret_cast<char*&>(data);
                    data = 0;
                }
            }
            const char*& operator * () const { return *((const char**)&data); }
        protected:
            char*& operator * () { return reinterpret_cast<char*&>(data); }
            void setval(const char *ndata) { data = reinterpret_cast<const zv_data_t&>(ndata); }
        private:
            virtual ssize_t __from_string(const char* src, const char *mask = nullptr) {
                char *temp = nullptr, *&cd = reinterpret_cast<char*&>(data);
                ssize_t len = 0;
                char Q = 0;
                if ((nullptr != src) && (CSTRING == type)) {
                    switch (*src) {
                        case '`':
                        case '\'':
                        case '"':
                            Q = *src;
                            ++src;
                            while ((0 != src[len]) && ((Q != src[len]) || (src[len - 1] == '\\'))) ++len;
                            break;
                        default:
                            len = 0;
                            while ((0 != src[len]) && (' ' < src[len])) ++len;
                    }
                    if (!(strncmp(cd, src, len)));
                    else {
                        if (nullptr != (temp = new char[len + 1]));
                        else {
                            throw exception(__FILE__, __LINE__, "Недостаточно памяти...", "Zipper");
                        }
                        delete[] cd;
                        cd = temp;
                        strncpy(cd, src, len);
                        cd[len] = 0;
                    }
                    return len;
                }
                else if (nullptr != src) {
                    throw exception(__FILE__, __LINE__, "__base<char*>::__from_string() Неизвестный тип", "Zipper");
                }
                else {
                    throw exception(__FILE__, __LINE__, "__base<char*>::__from_string(Src) Src == nullptr", "Zipper");
                }
            }
            virtual ssize_t __to_string(std::string &Dest, const char *mask = nullptr) const {
                char *buff = new char[1024];
                ssize_t result = -1;
                if ((CSTRING == type) && (nullptr != buff)) {
                    if (0 != (result = sprintf(buff, __print_mask[type], name, reinterpret_cast<const char*>(data)))) {
                        Dest += buff;
                    }
                    delete[] buff;
                }
                else if (nullptr == buff) {
                    Dest += name;
                    Dest += '=';
                    Dest += reinterpret_cast<const char*>(data);
                    result = 0;
                }
                else { throw exception(__FILE__, __LINE__, "__base<char*>::__to_string() Неизвестный тип", "Zipper"); }
                return result;
            }
        };

/** Гибкий массив для работы
 * Использование:
 * в методах упаковки/распаковки делаем каст на эту структуру и работаем как с
 * массивом
 * Zipper.unpack(Config) {
 *      __zipper::entries &list = reinterpret_cast<__zipper::entries&>(Config);
 *      ...
 * }*/
        struct entries {
            instance inst;
            entry_base v[];
        };

/** Абстрактный класс объекта-заголовка: группа
 * Наследники должны реализовать методы (__from/__to)_string(),
 * в противном случае будет сгенерировано исключение!!! */
        struct group : public entry_base {
            group(const char *Name, const size_t ByteCnt);
            ~group();
            /** Абстрактный метод сканирующий значение из строкового представления */
            virtual ssize_t __from_string(const char *&Source, const char *mask = nullptr);
            // Абстрактный метод приводящий переменную в строковое представление
            virtual ssize_t __to_string(std::string &Dest, const char *mask = nullptr) const;
        };

    };
/** Абстрактный класс сериализатора */
    class Zipper {
    protected:
        u_int8_t *__src;
        size_t __srcsz;
        std::vector<u_int8_t*> vLines;
        int error;
        inline Zipper()
            : __src(nullptr)
            , __srcsz(0)
            , vLines()
            , error(0)
        {}
    public:
        inline virtual ~Zipper() {
            if (nullptr != __src) {
                delete[] __src;
                __src = nullptr;
            }
        }
        // Загрузить
        inline virtual int load(__zipper::entries &Inst) final {
            std::string file = Inst.inst.path;
            u_int8_t *LineStart = nullptr;
            u_int8_t *end = nullptr;
            u_int8_t *i = nullptr;
            {
                Cleaner cleaner;
                file += Inst.inst.name;
                if (-1 != cleaner.init(file.c_str()));
                else {
                    /* handle error */
                    return -1;
                }
                cleaner.release(__src, __srcsz);
            }
            LineStart = __src;
            end = __src + __srcsz;
            i = LineStart;

            for ( ; (0 != *i) && (i < end); i++) {
                if ('\n' == *i) {
                    vLines.push_back(LineStart);
                    *i = 0;
                    LineStart = i + 1;
                }
            }
            if (LineStart < i) vLines.push_back(LineStart);
            return unpack(Inst);
        }
        // Сохранить
        inline virtual int save(__zipper::entries &Inst) final {
            return pack(Inst);
        }
    private:
        // Должны быть реализованы
        virtual ssize_t pack(__zipper::entries &Inst) = 0;
        virtual ssize_t unpack(__zipper::entries &Inst) = 0;
    };
/** Обёртки */
    // Заголовок-файл
    struct instance : __zipper::instance {
        inline instance(const char *FileName, const char *FilePath, const size_t __SizeofInst)
            : __zipper::instance(FileName, FilePath, __SizeofInst)
        {}
        inline ~instance() {}
        inline operator modules::__zipper::entries & () { return *this; }
    };
    // Заголовок-группа
    struct group : __zipper::group {
        inline group(const char *GroupName, const size_t __SizeofGroup)
            : __zipper::group(GroupName, __SizeofGroup)
        {}
        inline ~group() {}
    };
    // целочисленная со знаком, 32 бита
    struct zipp_int32 : public __zipper::__base<int32_t> {
        inline zipp_int32(const char *VarName, const int32_t Dflt)
            : __zipper::__base<int32_t>(__zipper::INT32, VarName, Dflt)
        {}
        inline ~zipp_int32() {}
    };
    // целочисленная без знака, 32 бита
    struct zipp_uint32 : public __zipper::__base<u_int32_t> {
        inline zipp_uint32(const char *VarName, const u_int32_t Dflt)
            : __zipper::__base<u_int32_t>(__zipper::UINT32, VarName, Dflt)
        {}
        inline ~zipp_uint32() {}
    };
    // целочисленная со знаком, 64 бита
    struct zipp_int64 : public __zipper::__base<int64_t> {
        inline zipp_int64(const char *VarName, const int64_t Dflt)
            : __zipper::__base<int64_t>(__zipper::INT64, VarName, Dflt)
        {}
        inline ~zipp_int64() {}
    };
    // целочисленная без знака, 64 бита
    struct zipp_uint64 : public __zipper::__base<u_int64_t> {
        inline zipp_uint64(const char *VarName, const u_int64_t Dflt)
            : __zipper::__base<u_int64_t>(__zipper::UINT64, VarName, Dflt)
        {}
        inline ~zipp_uint64() {}
    };
    // вещественное 32 битв
    struct zipp_float : public __zipper::__base<float> {
        inline zipp_float(const char *VarName, const float Dflt)
            : __zipper::__base<float>(__zipper::FLOAT, VarName, Dflt)
        {}
        inline ~zipp_float() {}
    };
    // вещественное 64 битв
    struct zipp_double : public __zipper::__base<double> {
        inline zipp_double(const char *VarName, const double Dflt)
            : __zipper::__base<double>(__zipper::DOUBLE, VarName, Dflt)
        {}
        inline ~zipp_double() {}
    };
    // "Си"-строка
    struct zipp_cstring : public __zipper::__base<char*> {
        inline zipp_cstring(const char *VarName, const char *Dflt)
            : __zipper::__base<char*>(__zipper::CSTRING, VarName, Dflt)
        {}
        inline ~zipp_cstring() {}
    };
};
