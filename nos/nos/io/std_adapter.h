#ifndef NOS_IO_STD_ADAPTER_H
#define NOS_IO_STD_ADAPTER_H

namespace nos
{

    template <class C> class streambuf_adapter : public std::basic_streambuf<C>
    {
    private:
        typedef typename std::basic_streambuf<C>::int_type int_type;
        typedef typename std::basic_streambuf<C>::traits_type traits_type;

        nos::ostream &out;

    public:
        streambuf_adapter(nos::ostream &_out) : out(_out) {}

    protected:
        int_type overflow(int_type ch = traits_type::eof()) override
        {
            if (!traits_type::eq_int_type(ch, traits_type::eof()))
                out.putbyte(ch);
            return ch;
        }

        std::streamsize xsputn(const C *s, std::streamsize count) override
        {
            out.write(s, count);
            return count;
        }
    };

    template <class C> class std_ostream_adapter : public std::basic_ostream<C>
    {
    private:
        streambuf_adapter<C> buf;

    public:
        std_ostream_adapter(nos::ostream &_out)
            : std::basic_ostream<C>(&buf), buf(_out)
        {
        }
    };

    static inline std_ostream_adapter<char> make_std_adapter(nos::ostream &out)
    {
        return std_ostream_adapter<char>(out);
    }
}

#endif
