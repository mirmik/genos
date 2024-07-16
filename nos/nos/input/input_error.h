#ifndef NOS_INPUT_ERROR_H
#define NOS_INPUT_ERROR_H

namespace nos
{
    struct output_error
    {
        bool _is_closed = false;

        static constexpr output_error closed()
        {
            return {true};
        }

        static output_error ok()
        {
            return {false};
        }
    };

    struct input_error
    {
        bool _is_timeout = false;
        bool _is_closed = false;

        static constexpr input_error timeout()
        {
            return {true, false};
        }

        static constexpr input_error closed()
        {
            return {false, true};
        }

        static constexpr input_error eof()
        {
            return {false, true};
        }

        static input_error ok()
        {
            return {false, false};
        }

        bool is_timeout() const
        {
            return _is_timeout;
        }

        bool is_closed() const
        {
            return _is_closed;
        }

        bool is_ok() const
        {
            return !(_is_timeout || _is_closed);
        }

        explicit operator bool() const
        {
            return is_ok();
        }

        bool operator==(const input_error &oth)
        {
            return _is_timeout == oth._is_timeout &&
                   _is_closed == oth._is_closed;
        }
    };
}

#endif