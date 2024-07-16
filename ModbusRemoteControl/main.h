#pragma once

#include "ModbusHMI.h"
#include "kinco_sz7s.h"
#include "modbus/modbus-tcp.h"
#include "scpi.h"
#include <arpa/inet.h>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <mutex>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

const std::string VERSION = "1.3.0";

typedef enum
{
    CUSTOM = 0,
    SYS_IDN,
    SYS_STAT,
    DEVSTOTAL,
    AXESTOTAL,
    UPOS,
    POS,
    PARAM,
    AXIS_DEVS,
    AXIS_UBACKLIM,
    AXIS_UFORWLIM,
    AXIS_IDN,
    DEV_IDN,
    DEV_TYPE,
    DEV_TORQUE
} ScpiQueryId;

extern std::recursive_mutex global_mutex;

void ButtonCallback(uint16_t buttonId, bool state);
bool checkRunArgs(int argc,
                  char *argv[],
                  std::string &hmiIp,
                  uint16_t &scpiPort,
                  uint16_t &ncpiPort);
