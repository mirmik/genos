#ifndef NOS_SHELL_RPC_EXECUTOR_H
#define NOS_SHELL_RPC_EXECUTOR_H

#include <nos/shell/rpc/invoker.h>
#include <nos/binary/bytearray.h>
#include <nos/io/bytearray_istream.h>
#include <map>
#include <memory>

namespace nos 
{
    enum class RpcError : int16_t
    {
        OK = 0,
        MethodNotFoundCode = -1,
        UndefinedErrorCode = -2,
    };

    template <class Id>
    class rpc_executor 
    {
        std::map<Id, std::unique_ptr<nos::abstract_invoker>> _methods = {};

    public:
        template <class Ret, class ... Args>
        void add(const Id& id, const std::function<Ret(Args...)>& foo) 
        {
            _methods.insert(std::make_pair(id, 
                std::unique_ptr<nos::abstract_invoker>(
                    new nos::invoker<Ret,std::remove_reference_t<Args>...>(foo))));
        }

        auto& methods() { return _methods; }

        nos::abstract_invoker* find(const Id& id) 
        {
            auto iterator = _methods.find(id);
            if (iterator == _methods.end()) 
                return nullptr;
            else 
                return iterator->second.get();
        }

        nos::bytearray execute(const nos::buffer& msg) 
        {
            try 
            {
                nos::bytearray ans;
                nos::bytearray_ostream writer(ans);
                nos::bytearray_istream reader(msg);
                auto id = nos::deserialize_from<Id>(reader);
                nos::abstract_invoker* method = find(id);
                if (method == nullptr)
                    return nos::serialize<int16_t>((int16_t)nos::RpcError::MethodNotFoundCode);
                nos::serialize_to<int16_t>(writer, 0);
                (*method)(reader, writer);
                return ans;
            }
            catch (...) 
            {
                return nos::serialize<int16_t>((int16_t)nos::RpcError::UndefinedErrorCode);    
            }
        }
    };
}

#endif