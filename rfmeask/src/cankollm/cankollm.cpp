#include "cankollm/canopen.h"
#include <cankollm/cankollm.h>
#include <nos/print.h>
#include <thread>

Can can;
CanKollm cankollm;

void CanKollm::debugPrintFrame(canframe &frame)
{
    char buf[128];
    memset(buf, 0, 128);
    frame.hserial(buf, SER | PROTOORD_LITTLE);
}

void CanKollm::sendFrame(canframe &frame)
{
    char buf[128];
    memset(buf, 0, 128);
    auto len = frame.hserial(buf, SER | PROTOORD_LITTLE);
    buf[len] = '\r';
    len++;
    can->send(buf, len);
}

void CanKollm::unvalid_parameters()
{
    nos::log::warn("CAN request unvalid parameters");
}

int32_t CanKollm::sdoQuery(uint8_t &errstat,
                           uint8_t _module,
                           uint8_t cb,
                           uint16_t index,
                           uint8_t subindex,
                           uint32_t data)
{
    nos::log::trace("sdoQuery");
    std::lock_guard<std::mutex> canlock(canmutex);
    std::unique_lock<std::mutex> lock(sdomutex);
    sdoindex = index;
    sdosubindex = subindex;
    send_SDO_RX(_module, cb, index, subindex, data);

    sdoerr = true;
    sdocond.wait_for(lock, std::chrono::milliseconds(20));
    sdoindex = 0;
    sdosubindex = 0;

    if (sdoerr == true)
        errstat = 1;
    else
        errstat = 0;

    nos::log::trace("sdoQuery: status: {}", errstat);

    return sdodata;
}

int CanKollm::send_SDO_RX(uint8_t _module,
                          uint8_t cb,
                          uint16_t index,
                          uint8_t subindex,
                          uint32_t data)
{

    canframe frame = {};
    frame.type = 't';
    frame.function = SDO_RX;
    frame._module = _module;
    frame.length = 0x8;

    frame.u.d.sdo_rw = cb;
    frame.u.d.sdo_index = index;
    frame.u.d.sdo_subindex = subindex;
    frame.u.d.sdo_data = data;

    sendFrame(frame);
    return 0;
}

int CanKollm::send_SDO_TX(uint8_t _module,
                          uint8_t cb,
                          uint16_t index,
                          uint8_t subindex,
                          uint32_t data)
{

    canframe frame = {};
    frame.type = 't';
    frame.function = SDO_TX;
    frame._module = _module;
    frame.length = 0x8;

    frame.u.d.sdo_rw = cb;
    frame.u.d.sdo_index = index;
    frame.u.d.sdo_subindex = subindex;
    frame.u.d.sdo_data = data;

    sendFrame(frame);
    return 0;
}

void CanKollm::sdoTxParse(canframe &cf)
{

    std::unique_lock<std::mutex> lock(sdomutex);
    // printf("%s\r\n","sdoTxParse");
    if (cf.u.d.sdo_index == sdoindex && cf.u.d.sdo_subindex == sdosubindex)
    {
        sdodata = cf.u.d.sdo_data;
        sdoerr = false;
        sdocond.notify_one();
    };

    nos::log::trace("after sdoindex: {} {}", this->sdoindex, cf.u.d.sdo_index);
    nos::log::trace(
        "after sdosubindex: {} {}", this->sdosubindex, cf.u.d.sdo_subindex);
}

int CanKollm::Parse(char *str)
{
    if (*str == '!')
        return 2;
    if (*str == '?')
    {
        return -1;
    }

    canframe cf;

    hserial(str, cf, DESER | PROTOORD_LITTLE);

    switch (cf.function)
    {
    case NMT:
        printf("NMT\r\n");
        break;
    case EMERGENCY:
        if (cf._module)
        {
            printf("%s\r\n", "EMERGENCY");
            break;
        }
        else
        {
            printf("%s\r\n", "SYNC");
            break;
        }
    case TIME:
        printf("%s\r\n", "TIME");
        break;
    case TPDO1:
        printf("%s\r\n", "TPDO1");
        break;
    case RPDO1:
        printf("%s\r\n", "RPDO1");
        break;
    case TPDO2:
        printf("%s\r\n", "TPDO2");
        break;
    case RPDO2:
        printf("%s\r\n", "RPDO2");
        break;
    case TPDO3:
        printf("%s\r\n", "TPDO3");
        break;
    case RPDO3:
        printf("%s\r\n", "RPDO3");
        break;
    case TPDO4:
        printf("%s\r\n", "TPDO4");
        break;
    case RPDO4:
        printf("%s\r\n", "RPDO4");
        break;
    case SDO_TX:
        // printf("%s\r\n","SDO_TX");
        // printf("%s\r\n",cf.to_str_co().c_str());
        sdoTxParse(cf);
        break;
    case SDO_RX:
        printf("%s\r\n", "SDO_RX");
        break;
    case NODEGUARD:
        printf("%s\r\n", "NODEGUARD");
        break;
    default:
        printf("%s\r\n", "unregistred");
        break;
    }

    return 0;
}
