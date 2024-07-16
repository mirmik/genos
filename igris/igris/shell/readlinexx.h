/**
@file
Расширение драйвера datastruct/sline, реализующее логику работы командной строки
в парадигме readline. Драйвер умеет преобразовывать управляющие входные
последовательности в команды sline и реализует механизм работы с историей
предыдущих команд.

Драйвер реализован в виде callback-less машины состояний и предполагает
наличие внешнего уровня, интерпретирующего возвращаемые машиной статусы.

TODO: Вынести драйвер работы с историей как отдельный объект.
*/

#ifndef IGRIS_SHELL_READLINE_H
#define IGRIS_SHELL_READLINE_H

#include <algorithm>
#include <igris/container/sline.h>
#include <igris/container/unbounded_array.h>
#include <igris/defs/ascii.h>
#include <igris/util/bug.h>

#define READLINE_OVERFLOW -1

#define READLINE_NOTHING 0
#define READLINE_ECHOCHAR 1

#define READLINE_NEWLINE 2
#define READLINE_BACKSPACE 3
#define READLINE_DELETE 4
#define READLINE_HOME 5
#define READLINE_END 6

#define READLINE_UPDATELINE 7

#define READLINE_LEFT 8
#define READLINE_RIGHT 9

#define READLINE_STATE_NORMAL 0
#define READLINE_STATE_ESCSEQ 1
#define READLINE_STATE_ESCSEQ_MOVE 2
#define READLINE_STATE_ESCSEQ_MOVE_WAIT_7E 3

namespace igris
{
    class readline
    {
        igris::sline _line = {};
        int _state = 0;

        char _last = 0;
        int _lastsize = 0;

        // char *_history_space; // Указатель на буффер истории.
        // uint8_t _history_size; // Количество строк в буффере истории.

        igris::unbounded_array<char> _history_space = {};
        igris::unbounded_array<char> _buffer_space = {};

        uint8_t _headhist = 0; // Индекс в массиве, куда будет перезаписываться
                               // новая строка истории.
        uint8_t _curhist = 0; // Индекс выбора строки истории (0-пустая,
                              // 1-последняя, 2-предпоследняя и т.д.)

    public:
        size_t lastsize()
        {
            return _lastsize;
        }

        sline &line()
        {
            return _line;
        }

        void init(size_t maxsize, size_t history_deep)
        {
            _last = 0;
            _state = 0;
            _curhist = 0;
            _headhist = 0;
            _buffer_space.resize(maxsize);
            _history_space.resize(history_deep * maxsize);
            std::fill(_buffer_space.begin(), _buffer_space.end(), '\0');
            std::fill(_history_space.begin(), _history_space.end(), '\0');
            _line.init(maxsize);
        }

        void newline_reset()
        {
            // Когда случается новая линия, сбрасываем буффер ввода и скидываем
            // переключатель строки истории на последнюю строку.
            _line.reset();
            _curhist = 0;
        }

        size_t history_size()
        {
            return _history_space.size() / _buffer_space.size();
        }

        // Указатель на строку, взятую от последней пришедшей. (1 - последняя)
        char *history_pointer(int num)
        {
            int idx = (_headhist + history_size() - num) % history_size();
            return _history_space.data() + idx * _buffer_space.size();
        }

        // Вернуть указатель на строку истории, на которую указывает _curhist.
        // Вызывается на кнопки вверх/вниз
        char *current_history_pointer()
        {
            return history_pointer(_curhist);
        }

        void _push_line_to_history(const char *str, size_t len)
        {
            char *ptr =
                _history_space.data() + _headhist * _buffer_space.size();
            memcpy(ptr, str, len);
            *(ptr + len) = '\0';
            _headhist = (_headhist + 1) % history_size();
        }

        void _push_line_to_history(const char *str)
        {
            _push_line_to_history(str, strlen(str));
        }

        // Обновить историю, записав туда новую строку.
        void _push_current_line_to_history()
        {
            _push_line_to_history(_line.data(), _line.current_size());
        }

        void load_history_line()
        {
            _lastsize = _line.current_size();

            if (_curhist == 0)
            {
                _line.clear();
                _line.reset();
                return;
            }

            unsigned int sz = (unsigned int)strlen(current_history_pointer());

            memcpy(_line.data(), current_history_pointer(), sz);
            _line.set_size_and_cursor(sz, sz);
        }

        int history_up()
        {
            // DTRACE();
            if (_history_space.size() == 0)
                return 0;

            if (_curhist == history_size())
                return 0;

            _curhist++;

            load_history_line();
            return 1;
        }

        int is_not_same_as_last()
        {
            return !_line.equal(history_pointer(1));
        }

        int history_down()
        {
            if (_history_space.size() == 0)
                return 0;

            if (_curhist == 0)
                return 0;

            _curhist--;

            load_history_line();
            return 1;
        }

        int newdata(char c)
        {
            int ret;
            int retcode;

            switch (_state)
            {
            case READLINE_STATE_NORMAL:
                switch (c)
                {
                case '\r':
                case '\n':
                    // TODO: Возможно тут некорректно отрабатывается комбинация
                    // rnrnrnrn
                    if ((_last == '\n' || _last == '\r') && _last != c)
                    {
                        _last = 0;
                        retcode = READLINE_NOTHING;
                    }
                    else
                    {
                        if (_history_space.size() && _line.current_size() &&
                            is_not_same_as_last())
                        {
                            // Если есть буффер истории и введенная строка не
                            // нулевая и отличается от последней, сохраняем
                            // строку в историю.
                            _push_current_line_to_history();
                        }
                        _curhist = 0;

                        retcode = READLINE_NEWLINE;
                    }
                    break;

                case ASCII_BS:
                    ret = _line.backspace(1);
                    retcode = ret ? READLINE_BACKSPACE : READLINE_NOTHING;
                    break;

                case ASCII_ESC:
                    _state = READLINE_STATE_ESCSEQ;
                    retcode = READLINE_NOTHING;
                    break;

                default:
                    _line.newdata(c);
                    retcode = READLINE_ECHOCHAR;
                    break;
                }
                break;

            case READLINE_STATE_ESCSEQ:
                switch (c)
                {
                case 0x5B:
                    _state = READLINE_STATE_ESCSEQ_MOVE;
                    break;

                default:
                    // dprln("?warning in readline?");
                    _state = READLINE_STATE_NORMAL;
                    break;
                }
                retcode = READLINE_NOTHING;
                break;

            case READLINE_STATE_ESCSEQ_MOVE:
                retcode = READLINE_NOTHING;
                switch (c)
                {
                case 0x41: // up
                    if (history_up())
                        retcode = READLINE_UPDATELINE;
                    break;
                case 0x42: // down
                    if (history_down())
                        retcode = READLINE_UPDATELINE;
                    break;
                case 0x43: // right
                    ret = _line.right();
                    if (ret)
                        retcode = READLINE_RIGHT;
                    break;
                case 0x44: // left
                    ret = _line.left();
                    if (ret)
                        retcode = READLINE_LEFT;
                    break;
                case 0x33:
                    ret = _line.del(1);
                    if (ret)
                        retcode = READLINE_DELETE;
                    _state = READLINE_STATE_ESCSEQ_MOVE_WAIT_7E;
                    _last = c;
                    return retcode;
                default:
                    // dpr("unr esc: hex:");
                    // dprhex(c);
                    // dpr(" dec:");
                    // dprln((int)c);
                    ;
                }
                _state = READLINE_STATE_NORMAL;
                break;

            case READLINE_STATE_ESCSEQ_MOVE_WAIT_7E:
                // if (c != '\x7E')
                //    dprln("?esc?");
                _state = READLINE_STATE_NORMAL;
                retcode = READLINE_NOTHING;
                break;

            default:
                // автомат находится в непредусмотренном состоянии
                // сбрасываем состояние и завершаем автомат.
                _state = READLINE_STATE_NORMAL;
                retcode = READLINE_NOTHING;
            }

            _last = c;
            return retcode;
        }

        int linecpy(char *data, size_t size)
        {
            int len = (int)size - 1 > (int)_line.current_size()
                          ? (int)_line.current_size()
                          : (int)size - 1;

            memcpy(data, _line.data(), len);
            data[len] = 0;

            return len;
        }
    };
}

#endif
