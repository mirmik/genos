#ifndef CROW_PUBSUB_DEFS_H
#define CROW_PUBSUB_DEFS_H

#include <crow/proto/node.h>
#include <igris/buffer.h>

namespace crow
{
    enum class PubSubTypes : uint8_t
    {
        Publish = 0,
        Subscribe = 1,
        Consume = 2,
        Request = 3,
        Subscribe_v2 = 4,
        Unsubscribe = 5,
    };

    const char *pubsub_type_to_string(PubSubTypes type);

    struct pubsub_subheader : public node_subheader
    {
        PubSubTypes type = {};
    } __attribute__((packed));

    struct publish_subheader : public pubsub_subheader
    {
        uint8_t thmsize = 0;
        uint16_t datsize = 0;
        nos::buffer theme()
        {
            return {(char *)(this + 1), thmsize};
        }
        nos::buffer message()
        {
            return {(char *)(this + 1) + thmsize, datsize};
        }
    } __attribute__((packed));

    struct subscribe_subheader : public pubsub_subheader
    {
        uint8_t rqos = 0;
        uint16_t rackquant = 0;
        uint8_t thmsize = 0;
        nos::buffer theme()
        {
            return {(char *)(this + 1), thmsize};
        }
    } __attribute__((packed));

    struct subscribe_subheader_v2 : public pubsub_subheader
    {
        uint8_t rqos = 0;
        uint16_t rackquant = 0;
        uint8_t thmsize = 0;
        union cmd_u
        {
            uint32_t all = 0;
            struct
            {
                uint8_t subscribe_on_updates : 1;
                uint32_t request_latest : 31;
            } f;
        } cmd = {};
        nos::buffer theme()
        {
            return {(char *)(this + 1), thmsize};
        }
    } __attribute__((packed));

    struct consume_subheader : public pubsub_subheader
    {
        uint8_t thmsize = 0;
        uint16_t datsize = 0;
        nos::buffer theme()
        {
            return {(char *)(this + 1), thmsize};
        }
        nos::buffer message()
        {
            return {(char *)(this + 1) + thmsize, datsize};
        }
    } __attribute__((packed));

    struct request_subheader : public pubsub_subheader
    {
        uint8_t rqos = 0;
        uint16_t rackquant = 0;
        uint8_t thmsize = 0;
        uint8_t repthmsize = 0;
        uint16_t datsize = 0;
        nos::buffer theme()
        {
            return {(char *)(this + 1), thmsize};
        }
        nos::buffer reply_theme()
        {
            return {(char *)(this + 1) + thmsize, repthmsize};
        }
        nos::buffer message()
        {
            return {(char *)(this + 1) + thmsize + repthmsize, datsize};
        }
    } __attribute__((packed));

    template <class T>
    void load_timestamped_object(nos::buffer data, int64_t *timestamp, T *obj)
    {
        memcpy(timestamp, data.data(), sizeof(int64_t));
        memcpy(obj, data.data() + sizeof(int64_t), sizeof(T));
    }
}

#endif