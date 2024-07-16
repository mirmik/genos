#ifndef NOS_CANCEL_H
#define NOS_CANCEL_H

#include <atomic>
#include <memory>

namespace nos
{
    class cancel_token
    {
        std::shared_ptr<std::atomic<bool>> token;

    public:
        cancel_token(std::shared_ptr<std::atomic<bool>> token) : token(token) {}
        cancel_token(const cancel_token &) = default;
        cancel_token &operator=(const cancel_token &) = default;

        bool is_canceled()
        {
            return *token;
        }

        operator bool()
        {
            return *token;
        }
    };

    class cancel_token_source
    {
        std::shared_ptr<std::atomic<bool>> _token =
            std::make_shared<std::atomic<bool>>(false);

    public:
        operator bool()
        {
            return is_cancelled();
        }

        bool is_cancelled()
        {
            return *_token;
        }

        void cancel()
        {
            *_token = true;
        }

        cancel_token token()
        {
            return cancel_token(_token);
        }
    };
}

#endif