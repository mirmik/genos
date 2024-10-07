#pragma once

#include "App.h"
#include <chrono>
#include <fstream>
#include <list>
#include <mutex>
#include <memory>
#include <nos/inet/tcpspam_server.h>
#include <sched.h>
#include <string>
#include <thread>
#include <vector>

constexpr auto APP_MAX_RESTART_ATTEMPTS = 3;

class AppManager
{
private:
    std::vector<std::shared_ptr<App>> apps = {};
    std::vector<std::string> systemLogPaths = {};
    std::list<uint8_t> errorList = {};
    std::string appFilename = {};
    std::string settingsFilename = "/home/rfmeas/project/settings.json";
    std::string runtimeSettingsFilename = "/home/rfmeas/project/runtime.json";
    nos::inet::tcpspam_server spamserver = {};
    std::mutex spam_mutex = {};
    static std::mutex ioMutex;

public:
    enum Errors
    {
        NoError = 0,
        AppListNotFound,
        AppListSyntaxError,
        AppListConfigPath,
        AppListRuntimePath,
        AppAttemptsEmpty,
        AppRfmeasNotFound
    };
    struct Log
    {
        std::string path = {};
        std::vector<uint8_t> data = {}; // First 4 bytes are the size of data
    };
    AppManager(const std::string &appListFileName);
    bool loadConfigFile();
    void runApps();
    void start_all() { runApps(); }
    void closeApps();
    void stop_all() { closeApps(); }
    void restartApps();
    std::vector<std::shared_ptr<App>> &getAppsList();
    size_t getAppCount() const;
    const std::string &
    getDeviceDescFilename() const; // rfmeask "config.json" file with path
    const std::string &
    getDeviceRuntimeFilename() const; // rfmeask "runtime.json" file with path
    std::list<uint8_t> &errors();
    std::vector<std::string> &getSystemLogPaths();
    void pushError(Errors error);
    std::vector<AppManager::Log> packLogs();
    std::vector<std::shared_ptr<App>> &applications()
    {
        return apps;
    }
    const std::string &getAppConfigFilename();
    std::shared_ptr<App> getApp(size_t index);
    std::shared_ptr<App> findApp(const std::string &name);
    void send_spam(const std::string &message);
    void send_spam(const std::vector<uint8_t> &message);
    void reload_config();

    void on_child_finished(pid_t pid);
    std::shared_ptr<App> get_app_by_pid(pid_t pid);

    void update_systemctl_projects_status();
};
