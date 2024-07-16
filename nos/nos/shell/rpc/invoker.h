#ifndef NOS_RPC_BIND_H
#define NOS_RPC_BIND_H

#include <nos/binary/bytearray.h>

namespace nos 
{
    class abstract_invoker 
    {
    public:
        virtual void operator()(nos::istream& input, nos::ostream& output) = 0;
        virtual nos::bytearray  operator()(const nos::buffer& in) 
        {
            nos::bytearray ret;
            nos::bytearray_istream input(in);
            nos::bytearray_ostream output(ret);
            this->operator()(input, output);
            return ret;
        }
        virtual ~abstract_invoker() = default;
    };

    template <class R, class ... Args >
    class invoker : public abstract_invoker 
    {
        std::function<R(Args...)> foo;

    public:
        invoker(const std::function<R(Args...)>& foo) : foo(foo) {}

        void operator()(nos::istream& input, nos::ostream& output) override
        {
            std::tuple<Args...> args = nos::deserialize_from<std::tuple<Args...>>(input);
            R ret = std::apply(foo, args);
            nos::serialize_to<R>(output, ret);
        }
    };

}

#endif