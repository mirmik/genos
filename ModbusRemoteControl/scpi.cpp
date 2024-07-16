#include "scpi.h"
#include <igris/util/dstring.h>
#include <iostream>
#include <main.h>
#include <nos/input.h>
#include <nos/log.h>
#include <nos/util/string.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

Scpi::Scpi(const char *ip, uint16_t port, uint16_t notifyPort)
{
    idn = "";

    _ip = ip;
    _scpiPort = port;
    _ncpiPort = notifyPort;

    signal(SIGPIPE, SIG_IGN);
}

void Scpi::reconnect()
{
    try
    {
        scpisock.connect(_ip, _scpiPort);
        ncpisock.connect(_ip, _ncpiPort);

        notifyQueue.clear();
        connected = true;

        hNCPIThread.reset(new thread(&Scpi::ncpiThread, this));
    }
    catch (const nos::inet::tcp_stream_error &)
    {
        connected = false;
        throw scpi_connection_error();
    }
}

void Scpi::disconnect()
{
    connected = false;
    scpisock.close();
    ncpisock.close();
}

void Scpi::updateAxisList()
{
    axis_list.clear();
    auto axtot = requestTotalAxesCount();

    for (unsigned int i = 0; i < axtot; ++i)
    {
        auto &ref = axis_list.emplace_back(i);
        ref.requestState(*this);
    }
}

void Axis::requestState(Scpi &parent)
{
    idn = parent.requestAxisName(number);
    pos = parent.requestAxisPos(number);
    speed = parent.requestAxisSpeed(number);
    forw_limit = parent.requestAxisForwardLimit(number);
    back_limit = parent.requestAxisBackwardLimit(number);
    max_speed = parent.requestAxisMaxSpeed(number);
    default_speed = parent.requestAxisDefaultSpeed(number);
}

void Axis::printState()
{
    PRINT(idn);
    PRINT(pos);
    PRINT(speed);
    PRINT(forw_limit);
    PRINT(back_limit);
    PRINT(max_speed);
    PRINT(default_speed);
}

bool Scpi::updateFullSystemInfo()
{
    try
    {
        this->idn = requestSysIdn();

        updateAxisList();

        // auto syssts = requestSysReadyStatus();
        // auto devtot = requestTotalDevsCount();

        for (int i = 0; i < std::min<int>(devs.size(), 15); i++)
        {
            auto devname = requestDevName(i);
        }
    }
    catch (const nos::inet::tcp_stream_error &)
    {
        nos::println("Fault system info update.");
        disconnect();
        return false;
    }

    return true;
}

std::string Scpi::requestSysIdn()
{
    return sendQuery("*IDN?");
}

size_t Scpi::requestTotalAxesCount()
{
    return sendQueryNumer("SYST:AXESTOT?");
}

int Scpi::requestSysReadyStatus()
{
    return sendQueryNumer("SYST:STAT?");
}

size_t Scpi::requestTotalDevsCount()
{
    return sendQueryNumer("SYST:DEVSTOTAL?");
}

std::string Scpi::requestAxisName(uint32_t axis)
{
    return sendQuery("AXIS" + std::to_string(axis) + ":STAT:IDN?");
}

double Scpi::requestAxisForwardLimit(uint32_t axis)
{
    return sendQueryNumer("AXIS" + to_string(axis) + ":SETTINGS:UFORWLIM?");
}

double Scpi::requestAxisBackwardLimit(uint32_t axis)
{
    return sendQueryNumer("AXIS" + to_string(axis) + ":SETTINGS:UBACKLIM?");
}

double Scpi::requestAxisSpeed(uint32_t axis)
{
    return sendQueryNumer("AXIS" + to_string(axis) + ":SPE?");
}

double Scpi::requestAxisMaxSpeed(uint32_t axis)
{
    return sendQueryNumer("AXIS" + to_string(axis) + ":SETTINGS:MAXSPE?");
}

double Scpi::requestAxisDefaultSpeed(uint32_t axis)
{
    return sendQueryNumer("AXIS" + to_string(axis) + ":SETTINGS:DEFSPE?");
}

double Scpi::requestAxisPos(uint32_t axis)
{
    return sendQueryNumer("AXIS" + to_string(axis) + ":STAT:UPOS?");
}

std::string Scpi::requestDevName(uint32_t dev)
{
    return sendQuery("DEV" + to_string(dev) + ":IDN?");
}

double Scpi::requestDevTorque(uint32_t dev)
{
    return sendQueryNumer("DEV" + to_string(dev) + ":TORQUE?");
}

void Scpi::axisSetSpeed(uint32_t axis, float speed)
{
    axis_list[axis].speed = speed;
    sendCmd("AXIS" + to_string(axis) + ":SPE " + to_string(speed));
}

void Scpi::axisMoveAbs(uint32_t axis, float pos)
{
    sendCmd("AXIS" + to_string(axis) + ":UMOV:ABS " + to_string(pos));
}

void Scpi::axisMove(uint32_t axis, float pos)
{
    sendCmd("AXIS" + to_string(axis) + ":UMOV " + to_string(pos));
}

void Scpi::axisJog(uint32_t axis, bool forw)
{
    sendCmd("AXIS" + to_string(axis) + ":JOG " +
            std::string(forw ? "1" : "-1"));
}

void Scpi::axisStop(uint32_t axis)
{
    sendCmd("AXIS" + to_string(axis) + ":STOP");
}

void Scpi::stopAll()
{
    sendCmd("SYST:STOP");
}

int Scpi::sendCmd(const std::string &cmd, SocketType socketType)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    nos::log::debug("send cmd: {}", igris::dstring(cmd));
    try
    {
        if (connected)
        {
            if (socketType == SocketType::SCPI)
            {
                auto estatus = nos::print_to(scpisock, cmd + "\n");
                if (estatus.is_error())
                {
                    nos::println("Fault send cmd.");
                    disconnect();
                    return -1;
                }
                return 0;
            }

            else
            {
                auto estatus = nos::print_to(ncpisock, cmd + "\n");
                if (estatus.is_error())
                {
                    nos::println("Fault send cmd.");
                    disconnect();
                }
                return 0;
            }
        }
        else
        {
            nos::println("Fault send cmd.");
            return -1;
        }
    }
    catch (const nos::inet::tcp_stream_error &)
    {
        nos::println("Fault send cmd.");
        disconnect();
        return -1;
    }
}

int Scpi::axesTotalCount()
{
    return axis_list.size();
}

std::string Scpi::sendQuery(const std::string &query)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    std::string ans;

    try
    {
        sendCmd(query);
        ans = nos::trim(*nos::readline_from(scpisock));
    }
    catch (const nos::inet::tcp_stream_error &)
    {
        throw scpi_connection_error();
    }

    return ans;
}

double Scpi::sendQueryNumer(const std::string &query)
{
    double ans;
    try
    {
        ans = stod(sendQuery(query));
    }
    catch (const std::invalid_argument &)
    {
        throw scpi_connection_error();
    }
    return ans;
}

void Scpi::setNotifications(NotificationType type, uint32_t id, bool enabled)
{
    if (type == Position)
    {
        if (enabled)
            sendCmd("NOT:AXIS" + to_string(id) + ":UPOS SMOOTH, 0.0000001",
                    NCPI);
        else
            sendCmd("NOT:AXIS" + to_string(id) + ":UPOS 0", NCPI);
    }
    else if (type == Torque)
        sendCmd("NOT:DEV" + to_string(id) + ":TORQUE " +
                    to_string(enabled ? 1 : 0),
                NCPI);
    else if (type == SystemStatus)
        sendCmd("NOT:SYST:STAT " + to_string(enabled ? 1 : 0), NCPI);
}

size_t Scpi::getAxesCount()
{
    return axis_list.size();
}

float Scpi::getAxisSpeed(uint32_t axis)
{
    return axis_list[axis].speed;
}

float Scpi::getAxisDefaultSpeed(uint32_t axis)
{
    return axis_list[axis].default_speed;
}

float Scpi::getAxisMaxSpeed(uint32_t axis)
{
    return axis_list[axis].max_speed;
}

float Scpi::getAxisPos(uint32_t axis)
{
    return axis_list[axis].pos;
}

float Scpi::getAxisForwardLimit(uint32_t axis)
{
    return axis_list[axis].forw_limit;
}

float Scpi::getAxisBackwardLimit(uint32_t axis)
{
    return axis_list[axis].back_limit;
}

size_t Scpi::getDevsCount()
{
    return devs.size();
}

float Scpi::getDevTorque(uint32_t dev)
{
    if (dev < devTorque.size())
        return devTorque[dev];
    else
        return 0;
}

bool Scpi::axisMoving(uint32_t axis)
{
    if (axis < axis_list.size())
        return axis_list[axis].speed > 0.00001f;
    else
        return false;
}

string Scpi::getIDN()
{
    return idn;
}

string Scpi::getAxisName(uint32_t axis)
{
    return axis_list[axis].idn;
}

string Scpi::getDevName(uint32_t dev)
{
    if (dev < devs.size())
        return devs[dev];
    else
        return "";
}

bool Scpi::isConnected()
{
    return connected;
}

void Scpi::ncpiThread()
{
    static std::array<uint8_t, 65536> rxNotifyBuf;

    while (1)
    {
        if (connected)
        {
            auto estatus =
                ncpisock.read(rxNotifyBuf.data(), sizeof(rxNotifyBuf));

            ssize_t status;
            if (estatus.is_ok())
                status = *estatus;
            else
                status = -1;

            if (status)
            {
                if (notifyQueue.size() < 10000000)
                {
                    notifyQueue.insert(notifyQueue.end(),
                                       rxNotifyBuf.begin(),
                                       rxNotifyBuf.begin() + status);
                    for (size_t i = 0; i < notifyQueue.size(); i++)
                    {
                        if (notifyQueue[i] == '\n' || notifyQueue[i] == ';')
                        {
                            string str(notifyQueue.begin(),
                                       notifyQueue.begin() + i);
                            notifyQueue.erase(notifyQueue.begin(),
                                              notifyQueue.begin() + i + 1);
                            parseNotificationData(str);
                            i = 0;
                        }
                    }
                }
                else
                {
                    for (int i = 0; (i < status) && (notifyQueue.size() != 0);
                         i++)
                        notifyQueue.pop_front();
                }
            }
        }
        else
        {
            return;
        }
        usleep(1000);
    }
}

void Scpi::parseNotificationData(const std::string &data)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);

    int offset = 0;
    if (data.find("NOT:AXIS") != string::npos)
    {
        uint32_t axis = (uint32_t)stoul(data.substr(8));
        offset = (axis > 9) ? 9 : 8;
        if ((data.find("UPOS", offset) != string::npos) &&
            (axis <= MAX_AXES_DEVS))
        {
            axis_list[axis].pos = std::stod(data.substr(20));
            _position_updated_signal(axis, axis_list[axis].pos);
        }
    }
    else if (data.find("NOT:DEV") != string::npos)
    {
        uint32_t dev = (uint32_t)stoul(data.substr(7));
        offset = (dev > 9) ? 8 : 7;
        if ((data.find("TORQUE", offset) != string::npos) &&
            (dev <= MAX_AXES_DEVS))
        {
            if (dev >= devTorque.size())
                devTorque.resize(dev + 1);
            devTorque[dev] = stof(data.substr(17));
        }
    }
    else if (data.find("NOT:SYST") != string::npos)
        systemIsReady = data.substr(18)[0] == '0';
}

void Scpi::assignValue(vector<float> &vec, uint32_t num, const string &valueStr)
{
    if (num <= MAX_AXES_DEVS)
    {
        if (num >= vec.size())
            vec.resize(num + 1);
        vec[num] = stof(valueStr);
    }
}

void Scpi::assignString(vector<string> &vec, uint32_t num, const string &str)
{
    if (num <= MAX_AXES_DEVS)
    {
        if (num >= vec.size())
            vec.resize(num + 1);
        vec[num] = str;
    }
}

bool Scpi::checkIsMovable(const string &devTypeStr)
{
    return devTypeStr.find("servo") != string::npos;
}

bool Scpi::getDevAlarmStatus()
{
    return devAlarm;
}

bool Scpi::getSystemReadyStatus()
{
    return systemIsReady;
}
