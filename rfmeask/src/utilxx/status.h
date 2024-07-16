/**
    @file
*/

#ifndef igris_RPC_STATUS_H
#define igris_RPC_STATUS_H

#include <igris/result.h>
#include <igris/serialize/serialize.h>

namespace igris
{
    namespace rpc
    {
        enum class status : uint8_t
        {
            OK,
            WrongArgsFormat, //Что это?
            WrongArgsData,
            WrongArgsTotal,

            InternalError,

            NotSupported,
            NotImplemented,
        };

        inline const char *strerr(status sts)
        {
            switch (sts)
            {
            case status::OK:
                return "OK";
            case status::WrongArgsData:
                return "WrongArgsData";
            case status::WrongArgsFormat:
                return "WrongArgsFormat";
            case status::WrongArgsTotal:
                return "WrongArgsTotal";
            case status::InternalError:
                return "InternalError";
            case status::NotSupported:
                return "NotSupported";
            case status::NotImplemented:
                return "NotImplemented";
            default:
                return "NoStandartError";
            }
        }

        template <typename T>
        using result = igris::result_type::result<T, status>;
    }

    template <typename Archive> struct serialize_helper<Archive, rpc::status>
    {
        static void serialize(Archive &keeper, const rpc::status &sts)
        {
            igris::serialize(keeper, (const uint8_t &)sts);
        }

        static void deserialize(Archive &keeper, rpc::status &sts)
        {
            igris::deserialize(keeper, (uint8_t &)sts);
        }
    };

    template <> inline const char *what<rpc::status>(const rpc::status &sts)
    {
        return rpc::strerr(sts);
    }
}

#endif
