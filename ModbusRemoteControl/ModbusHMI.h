#pragma once

#include "modbus/modbus-tcp.h"
#include <mutex>
#include <stdint.h>
#include <thread>
#include <vector>

#define HMI_MODE (1 << 0)
#define HMI_BUTTONS (1 << 1)
#define HMI_AXIS (1 << 2)
#define HMI_DEVICE (1 << 3)
#define HMI_SPEED (1 << 4)
#define HMI_ALL (HMI_MODE | HMI_BUTTONS | HMI_AXIS | HMI_DEVICE | HMI_SPEED)

#define HMI_FRAMERATE 30

class ModbusHMI
{
public:
    enum Mode
    {
        TargetMove = 0,
        ImmediateMove = 1
    };
    ModbusHMI(const char *ip = "192.168.1.253", const char *port = "502");
    ModbusHMI(const ModbusHMI &) = delete;
    ModbusHMI &operator=(const ModbusHMI &) = delete;
    ~ModbusHMI();
    bool connect(int hmiSlaveAddr);
    void disconnect();
    void setContinuousDataScanning(uint8_t enableFlags,
                                   uint8_t pollingIntervalMs);
    void setCurrentModeAddr(uint16_t addr);
    void setMoveSpeedAddr(uint16_t addr);
    void setTargetPosAddr(uint16_t addr);
    void setSystemReadyStatusAddr(uint16_t addr);
    void setSelectedAxisAddr(uint16_t addr);
    void setSelectedDevAddr(uint16_t addr);
    void addLeds(uint16_t startCoil, uint16_t count);
    void addIndicationLabel(uint16_t coilAddress);
    void addValueLabel(uint16_t addr, bool fractional = false);
    void addButtons(uint16_t startCoil,
                    uint16_t count,
                    void (*callback)(uint16_t btNum, bool state));
    void addGauge(uint16_t addr, float min, float max);
    void
    addGaugeVariable(uint16_t addr, uint16_t minValReg, uint16_t maxValReg);
    void addSlider(uint16_t addr);

    uint64_t readAllButtons();
    uint16_t readSelectedAxis();
    uint16_t readSelectedDevice();
    float readMoveSpeed();
    float readTargetPos();
    Mode readCurrentMode();
    void setSystemReadyStatus(bool status);
    void setLed(uint16_t ledNum, bool state);
    void setLabelValue(uint16_t id, float value);
    void setGaugeValue(uint16_t id, float value);
    void setGaugeLimits(uint16_t id, float min, float max);
    void setIndicationLabel(uint16_t id, bool state);
    uint16_t readSliderValue(uint16_t id);
    void setSliderValue(uint16_t id, int16_t value);

    int getLastCmdStatus();
    bool isConnected();

private:
    void modbusIOThread();
    uint16_t readReg(uint16_t reg);
    void writeReg(uint16_t reg, uint16_t value);
    void writeFloat(uint16_t startReg, float value);
    void writeCoil(uint16_t coil, bool state);
    modbus_t *ct = NULL;
    std::mutex m = {};
    struct GaugeSettings
    {
        uint16_t valueReg = 0;
        float min = 0;
        float max = 0;
        bool isVariable = 0;
    };
    struct LabelSettings
    {
        uint16_t valueReg = 0;
        bool isFrac = 0;
    };
    Mode currentMode = Mode::ImmediateMove;
    uint16_t currentAxis = 0, currentDev = 0;
    float speed = 0;
    uint16_t btStartAddr = 0;
    uint16_t btCount = 0;
    uint16_t currentModeAddr = 0;
    uint16_t moveSpeedAddr = 0;
    uint16_t targetPosAddr = 0;
    uint16_t systemReadyAddr = 0;
    uint16_t selectedAxisAddr = 0;
    uint16_t selectedDevAddr = 0;
    uint16_t ledStartAddr = 0, ledCount = 0;
    void (*btCallback)(uint16_t btNum, bool state) = nullptr;
    std::vector<GaugeSettings> gaugeSettings = {};
    std::vector<LabelSettings> labels = {};
    std::vector<uint16_t> sliders = {};
    std::vector<uint16_t> indicationLabels = {};
    std::thread *_thread = NULL;

public:
    int lastCmdStatus = 0;
    int slaveAddr = 1;
    bool continuousModeScan = false;
    bool continuousButtonsScan = false;
    bool continuousCurAxisScan = false;
    bool continuousCurDevScan = false;
    bool continuousSpeedScan = false;
    uint8_t scanInterval = 1;
    bool exit = false;
    bool connected = false;
    bool firstConnectAttempt = true;
    int fault_counter = 0;
};
