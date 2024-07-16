#include "ModbusHMI.h"
#include <iostream>
#include <main.h>
#include <nos/print.h>
#include <string.h>
#include <unistd.h>

using namespace std;

ModbusHMI::ModbusHMI(const char *ip, const char *port)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    ct = modbus_new_tcp_pi(ip, port);
    _thread = new thread(&ModbusHMI::modbusIOThread, this);
}

ModbusHMI::~ModbusHMI()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    exit = true;
    _thread->join();
    auto g = lock_guard(m);
    modbus_close(ct);
    modbus_free(ct);
}

bool ModbusHMI::connect(int hmiSlaveAddr)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    slaveAddr = hmiSlaveAddr;
    auto g = lock_guard(m);
    lastCmdStatus = modbus_connect(ct);
    if (lastCmdStatus != -1)
    {
        lastCmdStatus = modbus_set_slave(ct, hmiSlaveAddr);
    }
    if (lastCmdStatus != -1)
        connected = true;
    firstConnectAttempt = false;
    return lastCmdStatus != -1;
}

void ModbusHMI::disconnect()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    auto g = lock_guard(m);
    connected = false;
    modbus_close(ct);
}

void ModbusHMI::setContinuousDataScanning(uint8_t enableFlags,
                                          uint8_t pollingIntervalMs)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    scanInterval = pollingIntervalMs;
    continuousModeScan = (enableFlags & HMI_MODE) != 0;
    continuousButtonsScan = (enableFlags & HMI_BUTTONS) != 0;
    continuousCurAxisScan = (enableFlags & HMI_AXIS) != 0;
    continuousCurDevScan = (enableFlags & HMI_DEVICE) != 0;
    continuousSpeedScan = (enableFlags & HMI_SPEED) != 0;
}

void ModbusHMI::setCurrentModeAddr(uint16_t addr)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    currentModeAddr = addr;
}

void ModbusHMI::setMoveSpeedAddr(uint16_t addr)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    moveSpeedAddr = addr;
}

void ModbusHMI::setTargetPosAddr(uint16_t addr)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    targetPosAddr = addr;
}

void ModbusHMI::setSystemReadyStatusAddr(uint16_t addr)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    systemReadyAddr = addr;
}

void ModbusHMI::setSelectedAxisAddr(uint16_t addr)
{
    selectedAxisAddr = addr;
}

void ModbusHMI::setSelectedDevAddr(uint16_t addr)
{
    selectedDevAddr = addr;
}

void ModbusHMI::addLeds(uint16_t startCoil, uint16_t count)
{
    ledStartAddr = startCoil;
    ledCount = count;
}

void ModbusHMI::addIndicationLabel(uint16_t coilAddress)
{
    indicationLabels.push_back(coilAddress);
}

void ModbusHMI::addValueLabel(uint16_t addr, bool fractional)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    labels.push_back({addr, fractional});
}

void ModbusHMI::addButtons(uint16_t startCoil,
                           uint16_t count,
                           void (*callback)(uint16_t btNum, bool state))
{
    btCallback = callback;
    btStartAddr = startCoil;
    btCount = count;
}

void ModbusHMI::addGauge(uint16_t addr, float min, float max)
{
    gaugeSettings.push_back({addr, min, max, false});
}

void ModbusHMI::addGaugeVariable(uint16_t addr,
                                 uint16_t minValReg,
                                 uint16_t maxValReg)
{
    gaugeSettings.push_back({addr, (float)minValReg, (float)maxValReg, true});
}

void ModbusHMI::addSlider(uint16_t addr)
{
    sliders.push_back(addr);
}

uint64_t ModbusHMI::readAllButtons()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    uint64_t bitstates = 0;
    uint8_t states[sizeof(bitstates) * 8];
    memset(states, 0, sizeof(states));
    uint16_t count = btCount;
    if (count > sizeof(bitstates) * 8)
        count = sizeof(bitstates) * 8;
    m.lock();
    lastCmdStatus = modbus_read_bits(ct, btStartAddr, count, states);
    m.unlock();
    for (uint32_t i = 0; i < sizeof(bitstates) * 8; i++)
    {
        if (states[i])
            bitstates |= 1 << i;
    }
    return bitstates;
}

uint16_t ModbusHMI::readSelectedAxis()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return readReg(selectedAxisAddr);
}

uint16_t ModbusHMI::readSelectedDevice()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return readReg(selectedDevAddr);
}

float ModbusHMI::readMoveSpeed()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return readReg(moveSpeedAddr);
}

float ModbusHMI::readTargetPos()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return readReg(targetPosAddr);
}

ModbusHMI::Mode ModbusHMI::readCurrentMode()
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    return (ModbusHMI::Mode)readReg(currentModeAddr);
}

void ModbusHMI::setSystemReadyStatus(bool status)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    writeCoil(systemReadyAddr, status);
}

void ModbusHMI::setLed(uint16_t ledNum, bool state)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    writeCoil((uint16_t)(ledNum + ledStartAddr), state);
}

void ModbusHMI::setLabelValue(uint16_t id, float value)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);

    if (id < labels.size())
    {
        if (labels[id].isFrac)
            writeFloat(labels[id].valueReg, value);
        else
            writeReg(labels[id].valueReg, (uint16_t)value);
    }
}

void ModbusHMI::setGaugeValue(uint16_t id, float value)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    writeFloat(gaugeSettings[id].valueReg, value);
}

void ModbusHMI::setGaugeLimits(uint16_t id, float min, float max)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (gaugeSettings[id].isVariable)
    {
        writeFloat((uint16_t)gaugeSettings[id].min, min);
        writeFloat((uint16_t)gaugeSettings[id].max, max);
    }
}

void ModbusHMI::setIndicationLabel(uint16_t id, bool state)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (id < indicationLabels.size())
        writeCoil(indicationLabels[id], state);
}

uint16_t ModbusHMI::readSliderValue(uint16_t id)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (id < sliders.size())
        return readReg(sliders[id]);
    else
        return 0;
}

void ModbusHMI::setSliderValue(uint16_t id, int16_t value)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (id < sliders.size())
        writeReg(sliders[id], (uint16_t)value);
}

int ModbusHMI::getLastCmdStatus()
{
    return lastCmdStatus;
}

bool ModbusHMI::isConnected()
{
    return connected;
}

void ModbusHMI::modbusIOThread()
{
    uint64_t oldBtState = 0, btState = 0;
    while (1)
    {
        if (connected)
        {
            if (continuousButtonsScan)
            {
                btState = readAllButtons();
                if (lastCmdStatus != -1)
                    for (uint16_t i = 0; i < btCount; i++)
                    {
                        bool state = btState & (1 << i);
                        bool prevState = oldBtState & (1 << i);

                        if (state != prevState)
                        {
                            if (btCallback)
                                btCallback((uint16_t)(i + btStartAddr), state);
                            oldBtState ^= 1 << i;
                        }
                    }
            }
            if (continuousModeScan)
                currentMode = readCurrentMode();
            if (continuousCurAxisScan)
                currentAxis = readSelectedAxis();
            if (continuousCurDevScan)
                currentDev = readSelectedDevice();
            if (continuousSpeedScan)
                speed = readMoveSpeed();
            if (lastCmdStatus == -1)
            {
                fault_counter++;
                nos::println("Fault", fault_counter);
                if (fault_counter >= 20)
                {
                    fault_counter = 0;
                    disconnect();
                }
            }
            else
            {
                fault_counter = 0;
            }
        }
        else
        {
            if (!firstConnectAttempt)
            {
                usleep(100000);
                cout << "MODBUS Disconnected. Attempt to reconnect..." << endl;
                connect(slaveAddr);
            }
        }
        usleep(1000 * scanInterval);
    }
}

uint16_t ModbusHMI::readReg(uint16_t reg)
{
    auto g = lock_guard(m);
    uint16_t value = 0;
    lastCmdStatus = modbus_read_registers(ct, reg, 1, &value);
    return (lastCmdStatus == -1) ? 0 : value;
}

void ModbusHMI::writeReg(uint16_t reg, uint16_t value)
{
    auto g = lock_guard(m);
    lastCmdStatus = modbus_write_register(ct, reg, value);
}

void ModbusHMI::writeFloat(uint16_t startReg, float value)
{
    auto g = lock_guard(m);
    lastCmdStatus = modbus_write_registers(ct, startReg, 2, (uint16_t *)&value);
}

void ModbusHMI::writeCoil(uint16_t coil, bool state)
{
    auto g = lock_guard(m);
    lastCmdStatus = modbus_write_bit(ct, coil, state);
}
