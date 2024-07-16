/**
    @file
*/

#ifndef STATUS_H
#define STATUS_H

enum ConnectionStatus
{
    CONNECT = 0,
    DISCONNECT = 1,

    __CON_INIT = 10,
};

enum class DeviceStatus : uint8_t
{
    Ready = 0,
    Alarm = 1,
    Refuse = 2,
    Init = 10,
};

enum class Error
{
    FuncIsNotSupported,
    DeviceRefuseError,
    DeviceAlarmError,
};

enum StopType
{
    NORMAL,
    MANUAL,
    ABORT
};

enum class Operation : int8_t
{
    NONE = 0,
    USERMOVE = 1,
    USERMOVEINIT = -1,
    LINEARSCAN = 2,
    TESTCTR = 3,
};

enum OperationStopNotify
{
    NOTIFY_START = 0,
    NOTIFY_STOP_NORMAL = 1,
    NOTIFY_STOP_MANUAL = 2,
    NOTIFY_STOP_ABORT = 3,
};

#endif