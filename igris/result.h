#ifndef IGRIS_RESULT_H
#define IGRIS_RESULT_H

#include <cassert>
#include <cstdlib>

#include <functional>
#include <utility>

#include <cstring>
#include <string>

#include <igris/util/bug.h>

namespace igris
{
    namespace result_type
    {
        struct error
        {
            std::string info;
            explicit error(const std::string &str) : info(str) {}

            error(error &&e) : info(std::move(e.info)) {}

            error &operator=(error &&other)
            {
                info = other.info;
                return *this;
            }

            const char *what() const { return info.c_str(); }
        };

        struct errcode
        {
            char buf[13];
            int info;
            explicit errcode(int code) : info(code) {}

            errcode(errcode &&e) : info(e.info) {}

            errcode &operator=(errcode &&other)
            {
                info = other.info;
                return *this;
            }

            int code() { return info; }

            const char *what() const
            {
                sprintf((char *)buf, "errcode:%d", info);
                return buf;
            }
        };
    }
    template <typename E> inline const char *what(const E &e)
    {
        return e.what();
    }

    namespace result_type
    {
        template <typename T> struct tryhelper
        {
            using type = T;
            using storedtype = T;
            template <typename Arg> static void constructor(type &ths, Arg oth)
            {
                new (&ths) type(std::forward<Arg>(oth));
            }
            static void destructor(type &ths) { ths.~type(); };
            static void constructor(type &ths) { new (&ths) type(); };
        };

        template <typename T> struct tryhelper<T &>
        {
            using type = T &;
            using storedtype = std::reference_wrapper<T>;
            template <typename Arg>
            static void constructor(storedtype &ths, Arg oth)
            {
                new (&ths) storedtype(oth);
            }
            static void destructor(storedtype &ths) { ths.~storedtype(); };
            static void constructor(storedtype &ths){ (void)ths; };
        };

        template <typename T> struct tryhelper<T &&>
        {
            using type = T &&;
            using storedtype = T &&;
            template <typename Arg> static void constructor(type &ths, Arg oth)
            {
                ths = oth;
            }
            static void destructor(type &ths){ (void)ths; }
            static void constructor(type &ths){ (void)ths; }
        };

        template <typename T, typename E = error> class result
        {
        public:
            using Stored = typename tryhelper<T>::storedtype;
            using Result = typename tryhelper<T>::type;
            uint8_t _iserror;
            union
            {
                Stored _data;
                E _error;
            };

        public:
            template <typename U>
            result(U &&r) : _iserror(false), _data(std::forward<U>(r))
            {
            }
            result(E &&e) : _iserror(true), _error(std::forward<E>(e)) {}
            result(E &e) : _iserror(true), _error(e) {}

            result(result &&res) : _iserror(res._iserror)
            {
                if (_iserror)
                {
                    new (&_error) E(std::move(res._error));
                }
                else
                {
                    tryhelper<T>::constructor(_data, std::move(res._data));
                };
                _iserror = 2;
            }

            result(result &res) : _iserror(res._iserror)
            {
                if (_iserror)
                {
                    new (&_error) E(std::move(res._error));
                }
                else
                {
                    tryhelper<T>::constructor(_data, std::move(res._data));
                };
                _iserror = 2;
            }

            ~result()
            {
                switch (_iserror)
                {
                case 1:
                    _error.~E();
                    break;
                case 0:
                    tryhelper<T>::destructor(_data);
                    break;
                default:
                    break;
                };
            }

            Result &unwrap()
            {
                if (is_error())
                {
                    BUG_ON(_error.what());
                }
                return _data;
            }

            Result &unwrap(const char *loc)
            {
                if (is_error())
                {
                    dpr(loc);
                    dpr(": ");
                    BUG_ON(_error.what());
                }
                return _data;
            }

            inline void restore(T &&r)
            {
                _iserror = false;
                _error.~error();
                _data = std::forward<T>(r);
            }

            Result &value()
            {
                assert(_iserror == 0);
                return _data;
            }

            E &&error()
            {
                assert(_iserror == 1);
                return std::move(_error);
            }

            bool is_error() { return _iserror; }

            operator T()
            {
                if (is_error())
                {
                    BUG_ON(igris::what<E>(_error));
                }
                return _data;
            }

            inline void restore()
            {
                assert(_iserror == 1);
                _iserror = false;
                _error.~error();
                tryhelper<T>::constructor(_data);
            }
        };

        template <typename E> struct result<void, E>
        {
            using Result = void;
            uint8_t _iserror;
            union
            {
                E _error;
            };

            result() : _iserror(false) {}
            result(E &&e) : _iserror(true), _error(std::move(e)) {}
            result(E &e) : _iserror(true), _error(e) {}

            result(result &&res) : _iserror(res._iserror)
            {
                if (_iserror)
                {
                    new (&_error) E(std::move(res._error));
                }
                else
                {
                };
                _iserror = 2;
            }

            ~result()
            {
                switch (_iserror)
                {
                case 1:
                    _error.~E();
                    break;
                default:
                    break;
                };
            }

            void unwrap()
            {
                if (is_error())
                {
                    BUG_ON(_error.what());
                }
            }

            [[deprecated]] void getData() { assert(_iserror == 0); }

            [[deprecated]] E &getError()
            {
                assert(_iserror == 1);
                return _error;
            }

            void value() { assert(_iserror == 0); }

            E &&error()
            {
                assert(_iserror == 1);
                return std::move(_error);
            }

            bool is_error() { return _iserror; }

            inline void restore()
            {
                assert(_iserror == 1);
                _iserror = false;
                _error.~error();
            }
        };
    }

    template <class T, class E = result_type::error>
    using result = result_type::result<T, E>;
}

#define try_restore(val)                                                       \
    ({                                                                         \
        __result.restore(val);                                                 \
        goto try_label;                                                        \
    })

#endif
