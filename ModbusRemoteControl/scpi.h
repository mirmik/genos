#pragma once

#include <arpa/inet.h>
#include <deque>
#include <exception>
#include <mutex>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>

#include <igris/event/delegate.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_socket.h>

#define MAX_AXES_DEVS 20

class scpi_connection_error : public std::exception
{
    const char *what() const noexcept override
    {
        return "scpi_connection_error";
    }
};

class Scpi;
class Axis
{
public:
    int number = -1;
    std::string idn = "";
    float pos = {};
    float back_limit = {}, forw_limit = {};
    float speed = {}, max_speed = {}, default_speed = {};

public:
    explicit Axis(int num) : number(num) {}

    void requestState(Scpi &parent);
    void printState();
};

class Scpi
{
public:
    enum NotificationType
    {
        Position,
        Torque,
        SystemStatus
    };
    enum SocketType
    {
        SCPI = 0,
        NCPI
    };

public:
    std::vector<Axis> axis_list = {};

private:
    nos::inet::tcp_client scpisock = {};
    nos::inet::tcp_client ncpisock = {};

    std::unique_ptr<std::thread> hNCPIThread = {};
    std::string idn = "";
    std::vector<std::string> devs = {};
    std::vector<float> devTorque = {};
    std::vector<int> devMovableType = {};
    bool systemIsReady = false;
    bool devAlarm = false;

    std::deque<uint8_t> notifyQueue = {};
    std::deque<int> requestArgsQueue = {};
    bool connected = false;
    std::string _ip = "127.0.0.1";
    uint16_t _scpiPort = 0, _ncpiPort = 0;
    igris::delegate<void, int, float> _position_updated_signal = {};

public:
    Scpi(const char *ip = "127.0.0.1",
         uint16_t port = 5025,
         uint16_t notifyPort = 5026);
    void reconnect();
    void disconnect();

    void set_position_updated_callback(
        igris::delegate<void, int, float> position_updated_signal)
    {
        _position_updated_signal = position_updated_signal;
    }

    bool updateFullSystemInfo();
    void updateAxisList();

    std::string requestSysIdn();
    int requestSysReadyStatus();
    size_t requestTotalAxesCount();
    size_t requestTotalDevsCount();
    std::string requestAxisName(uint32_t axis);
    double requestAxisForwardLimit(uint32_t axis);
    double requestAxisBackwardLimit(uint32_t axis);
    double requestAxisSpeed(uint32_t axis);
    double requestAxisMaxSpeed(uint32_t axis);
    double requestAxisDefaultSpeed(uint32_t axis);
    double requestAxisPos(uint32_t axis);
    std::string requestDevName(uint32_t dev);
    double requestDevTorque(uint32_t dev);

    int axesTotalCount();
    void axisSetSpeed(uint32_t axis, float speed);
    void axisMoveAbs(uint32_t axis, float pos);
    void axisMove(uint32_t axis, float pos);
    void axisJog(uint32_t axis, bool forw);
    void axisStop(uint32_t axis);
    void stopAll();
    void setNotifications(NotificationType type, uint32_t id, bool enabled);

    bool getDevAlarmStatus();
    bool getSystemReadyStatus();
    size_t getAxesCount();
    float getAxisSpeed(uint32_t axis);
    float getAxisDefaultSpeed(uint32_t axis);
    float getAxisMaxSpeed(uint32_t axis);
    float getAxisPos(uint32_t axis);
    float getAxisForwardLimit(uint32_t axis);
    float getAxisBackwardLimit(uint32_t axis);
    size_t getDevsCount();
    float getDevTorque(uint32_t dev);
    bool axisMoving(uint32_t axis);
    std::string getIDN();
    std::string getAxisName(uint32_t axis);
    std::string getDevName(uint32_t dev);

    bool isConnected();

private:
    int sendCmd(const std::string &cmd, SocketType socketType = SCPI);
    std::string sendQuery(const std::string &query);
    double sendQueryNumer(const std::string &query);
    void ncpiThread();
    void parseNotificationData(const std::string &data);
    void assignValue(std::vector<float> &vec,
                     uint32_t num,
                     const std::string &valueStr);
    void assignString(std::vector<std::string> &vec,
                      uint32_t num,
                      const std::string &str);
    bool checkIsMovable(const std::string &devTypeStr);
};
