#include "RFDaemonServer.h"
#include "App.h"
#include "AppManager.h"
#include <cstring>
#include <fstream>
#include <modes.h>
#include <nos/fprint.h>
#include <sstream>
#include <string>

#define AddCmd(a, b) addCmd(a, #a, b)

RFDaemonServer::RFDaemonServer(uint16_t port) : TcpServer(port)
{
    auto make_bind = [&](auto mptr) {
        return std::bind(mptr, this, std::placeholders::_1,
                         std::placeholders::_2);
    };
    AddCmd(GET_APPS_INFO, make_bind(&RFDaemonServer::getAppsInfo));
    AddCmd(APPS_START, make_bind(&RFDaemonServer::startAllApps));
    AddCmd(APPS_STOP, make_bind(&RFDaemonServer::stopAllApps));
    AddCmd(APPS_RESTART, make_bind(&RFDaemonServer::restartAllApps));
    AddCmd(GET_CONFIG, make_bind(&RFDaemonServer::getConfig));
    AddCmd(SET_CONFIG, make_bind(&RFDaemonServer::setConfig));
    AddCmd(UPDATE_IMG, make_bind(&RFDaemonServer::updateSysImg));
    AddCmd(UPDATE_FIRMWARE, make_bind(&RFDaemonServer::updateControllerFW));
    AddCmd(GET_LOGS, make_bind(&RFDaemonServer::getLogs));
    AddCmd(GET_APPS_LIST, make_bind(&RFDaemonServer::getAppsList));
    AddCmd(SET_APPS_LIST, make_bind(&RFDaemonServer::setAppsList));
}

RFDaemonServer::~RFDaemonServer()
{
}

void RFDaemonServer::addCmd(uint32_t code, std::string name,
                            const cmdfunction &cmd)
{
    SrvCmd c(code, cmd, name);
    commands.push_back(c);
}

bool RFDaemonServer::writeFile(const std::string &filename, const uint8_t *data,
                               uint32_t size)
{
    bool error = false;
    std::fstream f;
    f.open(filename, std::fstream::out | std::fstream::trunc);
    if (f.is_open())
    {
        f.seekp(0);
        f.write((const char *)data, size);
        error = (f.rdstate() & (std::ios::failbit | std::ios::badbit)) != 0;
        f.flush();
    }
    return error;
}

std::vector<uint8_t> RFDaemonServer::startAllApps(const uint8_t *, uint32_t)
{
    AppManager::instance()->runApps();
    return std::vector<uint8_t>();
}

std::vector<uint8_t> RFDaemonServer::stopAllApps(const uint8_t *, uint32_t)
{
    AppManager::instance()->closeApps();
    return std::vector<uint8_t>();
}

std::vector<uint8_t> RFDaemonServer::restartAllApps(const uint8_t *, uint32_t)
{
    AppManager::instance()->restartApps();
    return std::vector<uint8_t>();
}

std::vector<uint8_t> RFDaemonServer::getConfig(const uint8_t *, uint32_t)
{
    auto settings_path = AppManager::instance()->getDeviceDescFilename();
    auto runtime_path = AppManager::instance()->getDeviceRuntimeFilename();

    std::vector<uint8_t> answer(9);
    std::fstream cfg, runtime;
    cfg.open(settings_path, std::fstream::in);
    if (cfg.is_open())
    {
        answer[0]++;
        std::stringstream cfgBuf;
        cfgBuf << cfg.rdbuf();
        std::string s = cfgBuf.str();
        *(uint32_t *)(answer.data() + 1) = (uint32_t)s.length();
        answer.insert(answer.end(), s.begin(), s.end());
    }

    runtime.open(runtime_path, std::fstream::in);
    if (runtime.is_open())
    {
        answer[0]++;
        std::stringstream runtimeBuf;
        runtimeBuf << runtime.rdbuf();
        std::string s = runtimeBuf.str();
        *(uint32_t *)(answer.data() + 5) = (uint32_t)s.length();
        answer.insert(answer.end(), s.begin(), s.end());
    }

    return answer;
}

// TODO: Файлы принимаются не абстрактно.
std::vector<uint8_t> RFDaemonServer::setConfig(const uint8_t *data, uint32_t)
{
    uint8_t fileCount = data[0];
    bool error = false;
    std::vector<uint8_t> answer(1);
    if (fileCount--)
    {
        uint32_t cfgSize = *(uint32_t *)(data + 1);
        error = writeFile(AppManager::instance()->getDeviceDescFilename(), data + 5, cfgSize);
    }
    answer[0] = error;
    return answer;
}

std::vector<uint8_t> RFDaemonServer::getAppsInfo(const uint8_t *, uint32_t)
{
    struct __attribute__((packed)) AppData
    {
        uint8_t state;
        uint8_t startSuccess;
        int8_t error;
        int32_t pid;
        int64_t uptime;
    };

    uint8_t appCount = (uint8_t)AppManager::instance()->getAppCount();
    std::vector<uint8_t> answer(2 + appCount * sizeof(AppData));
    answer[0] = appCount;

    if (AppManager::instance()->errors().size())
    {
        answer[1] = AppManager::instance()->errors().front();
        AppManager::instance()->errors().pop_front();
    }
    else
        answer[1] = 0;

    AppData *pAppData = (AppData *)(answer.data() + 2);
    auto &apps = AppManager::instance()->getAppsList();
    for (int i = 0; i < appCount; i++)
    {
        pAppData[i].state = !apps[i]->stopped();
        pAppData[i].startSuccess = true; // Не используется
        pAppData[i].uptime = apps[i]->uptime();
        pAppData[i].pid = apps[i]->pid();
        if (apps[i]->errors().size())
        {
            pAppData[i].error = (int8_t)apps[i]->errors().front();
            apps[i]->errors().pop();
        }
    }
    return answer;
}

std::vector<uint8_t> RFDaemonServer::updateSysImg(const uint8_t *data,
                                                  uint32_t size)
{
    std::vector<uint8_t> answer(1);
    answer[0] = writeFile("new_docker_img.tgz", data, size);
    return answer;
}

std::vector<uint8_t> RFDaemonServer::updateControllerFW(const uint8_t *data,
                                                        uint32_t size)
{
    std::vector<uint8_t> answer(1);
    answer[0] = writeFile("new_controller_fw.bin", data, size);
    return answer;
}

std::vector<uint8_t> RFDaemonServer::setAppsList(const uint8_t *data,
                                                 uint32_t size)
{
    std::vector<uint8_t> answer(1);
    answer[0] = writeFile(AppManager::instance()->getAppConfigFilename(), data, size);
    if (!answer[0])
    {
        AppManager::instance()->loadConfigFile();
        AppManager::instance()->restartApps();
    }
    return answer;
}

std::vector<uint8_t> RFDaemonServer::getAppsList(const uint8_t *, uint32_t)
{
    std::ifstream f =
        std::ifstream(AppManager::instance()->getAppConfigFilename(), std::ifstream::in);
    std::string s(std::istreambuf_iterator<char>{f}, {});
    size_t strSize = s.length() + 1;
    std::vector<uint8_t> answer(strSize);
    memcpy(answer.data(), s.c_str(), strSize);
    return answer;
}

std::vector<uint8_t> RFDaemonServer::getLogs(const uint8_t *, uint32_t)
{
    size_t namesLen = 0, offset = 0;
    auto logs = AppManager::instance()->packLogs();
    for (const auto &l : logs)
        namesLen += l.path.length() + 1;
    size_t logsNum = logs.size();
    std::vector<uint8_t> answer(1 + (1 + sizeof(uint32_t)) * logsNum +
                                namesLen);
    answer[0] = (uint8_t)logsNum;

    for (size_t i = 0; i < logsNum; i++)
    {
        ((uint32_t *)(answer.data() + 1 + logsNum))[i] =
            (uint32_t)logs[i].data.size();
        size_t pathStrLen = logs[i].path.length() + 1;
        memcpy(answer.data() + 1 + (1 + sizeof(uint32_t)) * logsNum + offset,
               logs[i].path.c_str(), pathStrLen);
        offset += pathStrLen;
        answer.insert(answer.end(), logs[i].data.begin(), logs[i].data.end());
    }
    return answer;
}

std::vector<uint8_t>
RFDaemonServer::parseReceivedData(const std::vector<uint8_t> &data)
{
    std::vector<uint8_t> answer;
    bool fromClientToSrv = data[0] != 0;
    uint8_t cmdNum = data[1];
    uint32_t argOffset =
        2 + cmdNum * (uint32_t)(sizeof(uint16_t) + sizeof(uint32_t));

    if (VERBOSE)
    {
        nos::println("Received data:");
        nos::print_dump(data.data(), data.size());
    }

    if (fromClientToSrv && (data.size() >= argOffset))
    {
        uint16_t *pCmdList = (uint16_t *)(data.data() + 2);
        uint32_t *pArgSizeList =
            (uint32_t *)(data.data() + 2 + cmdNum * sizeof(uint16_t));
        answer.resize(2 + cmdNum * (sizeof(uint16_t) + sizeof(uint32_t)));
        answer[0] =
            0; // '0' is indicating that this is answer from server to client
        answer[1] = cmdNum; // Number of commands

        for (size_t i = 0; i < cmdNum; i++)
        {
            answer[i * 2 + 2] = (uint8_t)pCmdList[i];
            auto cmd = commands[pCmdList[i]];
            if (VERBOSE)
                nos::println("Command: ", cmd.name);
            auto cmdRet = cmd.cmd(data.data() + argOffset, pArgSizeList[i]);
            argOffset += pArgSizeList[i];
            *(uint32_t *)(answer.data() + i * sizeof(uint32_t) + 2 +
                          cmdNum * sizeof(uint16_t)) = (uint32_t)cmdRet.size();
            if (cmdRet.size() != 0)
                answer.insert(answer.end(), cmdRet.begin(), cmdRet.end());
        }
    }
    return answer;
}
