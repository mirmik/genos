#ifndef CANKOLLM_H
#define CANKOLLM_H

class Can;

#include "utilxx/hserial.h"
#include <condition_variable>
#include <igris/util/bits.h>
#include <mutex>
#include <sstream>
#include <stdarg.h>
#include <string>
//#include <utilxx/formatedString.h>

#define NMT 0
#define EMERGENCY 0x01
#define SYNC 0x01 // without module
#define TIME 0x02
#define TPDO1 0x03
#define RPDO1 0x04
#define TPDO2 0x05
#define RPDO2 0x06
#define TPDO3 0x07
#define RPDO3 0x08
#define TPDO4 0x09
#define RPDO4 0x0A
#define SDO_TX 0x0B
#define SDO_RX 0x0C
#define NODEGUARD 0x0E

struct canframe
{
    char type = 0;
    uint8_t function = 0;
    uint8_t _module = 0;
    uint8_t length = 0;
    union u_t
    {
        uint64_t data = 0;
        struct d_t
        {
            uint8_t sdo_rw;
            uint16_t sdo_index;
            uint8_t sdo_subindex;
            uint32_t sdo_data;
        } __attribute__((packed)) d;
    } u = {};

    canframe() = default;

    unsigned int hserial(char *buf, uint8_t flag)
    {
        char *_buf = buf;
        uint8_t cobid_0h = 0;
        uint8_t cobid_1b = 0;

        if (bits_mask_eq(flag, SERDIR_MASK, SER))
        {
            if (_module >= 128 || function >= 16)
                throw std::runtime_error(
                    "Can::hserial: module or function is too big");
            cobid_0h = (function >> 1);
            cobid_1b = ((function & 0x1) << 7) + _module;
        }

        if (bits_mask_eq(flag, SERDIR_MASK, SER))
            *_buf++ = type;
        else
            type = *_buf++;
        _buf += ::hserial_half(_buf, &cobid_0h, flag);
        _buf += ::hserial(_buf, &cobid_1b, flag);
        _buf += ::hserial_half(_buf, &length, flag);
        _buf += ::__hserial(_buf, &u.data, length, flag);

        if (bits_mask_eq(flag, SERDIR_MASK, DESER))
        {
            function = ((cobid_1b & 0x80) >> 7) + (cobid_0h << 1);
            _module = cobid_1b & 0x7F;
        }

        return _buf - buf;
    }
} __attribute__((packed));

class CanKollm
{
public:
    Can *can = nullptr;
    bool sdoerr = false;
    std::condition_variable sdocond = {};
    std::mutex sdomutex = {};
    std::mutex canmutex = {};
    uint16_t sdoindex = 0;
    uint8_t sdosubindex = 0;
    volatile int64_t sdodata = 0;

public:
    int Parse(char *str);
    int send_SDO_RX(uint8_t _module,
                    uint8_t cb,
                    uint16_t index,
                    uint8_t subindex,
                    uint32_t data);
    int send_SDO_TX(uint8_t _module,
                    uint8_t cb,
                    uint16_t index,
                    uint8_t subindex,
                    uint32_t data);
    int32_t sdoQuery(uint8_t &errstat,
                     uint8_t _module,
                     uint8_t cb,
                     uint16_t index,
                     uint8_t subindex,
                     uint32_t data);
    void sendFrame(canframe &frame);
    void debugPrintFrame(canframe &frame);
    void unvalid_parameters();

    void sdoTxParse(canframe &cf);
};

extern Can can;
extern CanKollm cankollm;

#endif