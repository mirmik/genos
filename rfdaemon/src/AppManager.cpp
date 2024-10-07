#include "AppManager.h"
#include <byteswap.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <mutex>
#include <nos/trent/json.h>
#include <unistd.h>

extern bool VERBOSE;
std::mutex AppManager::ioMutex = {};
using namespace std::chrono_literals;

AppManager::AppManager(const std::string &appListFileName)
{
    appFilename = appListFileName;
    spamserver.start(5001);
}

void AppManager::send_spam(const std::string &message)
{
    std::lock_guard<std::mutex> lock(spam_mutex);
    nos::print_to(spamserver, message);
}

void AppManager::send_spam(const std::vector<uint8_t> &message)
{
    std::lock_guard<std::mutex> lock(spam_mutex);
    spamserver.write(message.data(), message.size());
}

RestartMode  parse_restart_mode(std::string str) 
{
        if (str == "once")
            return RestartMode::ONCE;
        else if (str == "always")
            return RestartMode::ALWAYS;
        else
            return RestartMode::ONCE;
}

std::unordered_map<std::string, std::string> environment_map_parse(
    const nos::flat_map<std::string, nos::trent>& env 
) 
{
    std::unordered_map<std::string, std::string> env_map;
    for (const auto &rec : env)
            env_map[rec.first] = rec.second.as_string();
    return env_map;    
}


void parse_application_record(nos::trent& apptrent, std::vector<std::shared_ptr<App>>& apps) 
{
    std::vector<LinkedFile> linked_files;
    std::string name = apptrent["name"].as_string();
    if (name.empty())
    {
        nos::println("Error: app name is empty");
        return;
    }

    std::string cmd = apptrent["command"].as_string();
    if (name.empty())
    {
        nos::println("Error: app command is empty");
        return;
    }
    
    if (VERBOSE)
        nos::fprintln("\t{}", name);

    RestartMode restartMode = parse_restart_mode(
            apptrent["restart"].as_string_default("once"));

    auto& files = apptrent["files"].as_list();
    for (const auto &rec : files)
    {
        LinkedFile file;
        file.path = rec["path"].as_string();
        file.name = rec["name"].as_string();
        file.editable = rec["editable"].as_bool();
        linked_files.push_back(file);
    }
    
    auto systemd_bind = apptrent["systemd_bind"].as_string_default("");
        
    auto &app = apps.emplace_back(std::make_shared<App>(
        apps.size(), 
        name, 
        cmd, 
        restartMode, 
        linked_files,
        apptrent["user"].as_string_default("")));

    std::unordered_map<std::string, std::string> env_map = environment_map_parse(apptrent["env"].as_dict());
    app->set_environment_variables(env_map);
    app->set_systemd_bind(systemd_bind);
}

bool AppManager::loadConfigFile()
{
    if (VERBOSE)
        nos::println("loading config file from", appFilename);

    nos::trent root = nos::json::parse_file(appFilename);

    auto& root_list = root["apps"].as_list();
    if (root_list.size() == 0)    
    {
        nos::fprintln("No apps found in file '{}'.", appFilename);
        return false;
    }

    closeApps();
    apps.clear();

    if (VERBOSE)
        nos::println("Parse application list:");
    
    for (auto& apptrent: root_list)
        parse_application_record(apptrent, apps);

    auto& logs = root["sys_logs"].as_list();
    for (auto& log : logs)
        systemLogPaths.push_back(log.as_string());

    return false;
}


void AppManager::runApps()
{
    for (auto &a : apps)
    {
        if (a->stopped())
            a->start();
    }
}

void AppManager::closeApps()
{
    for (auto &a : apps)
    {
        if (!a->stopped())
            a->stop();
    }
    std::lock_guard lock(ioMutex);
    nos::println("All created processes have just been killed.");
}

void AppManager::restartApps()
{
    closeApps();
    std::this_thread::sleep_for(500ms);
    runApps();
}

size_t AppManager::getAppCount() const
{
    return apps.size();
}

const std::string &AppManager::getAppConfigFilename()
{
    return appFilename;
}

std::vector<std::shared_ptr<App>> &AppManager::getAppsList()
{
    return apps;
}

std::shared_ptr<App> AppManager::findApp(const std::string &name)
{
    if (std::isdigit(name[0]))
    {
        return apps[stoi(name)];
    }

    for (auto &a : apps)
    {
        if (!a->name().compare(name))
            return a;
    }
    return nullptr;
}

std::shared_ptr<App> AppManager::getApp(size_t index)
{
    if (index < apps.size())
        return apps[index];

    return nullptr;
}

const std::string &AppManager::getDeviceDescFilename() const
{
    return settingsFilename;
}

const std::string &AppManager::getDeviceRuntimeFilename() const
{
    return runtimeSettingsFilename;
}

std::list<uint8_t> &AppManager::errors()
{
    return errorList;
}

std::vector<std::string> &AppManager::getSystemLogPaths()
{
    return systemLogPaths;
}

void AppManager::pushError(Errors error)
{
    errorList.push_back((uint8_t)error);
    if (errorList.size() > 100)
        errorList.pop_front();
}

std::vector<AppManager::Log> AppManager::packLogs()
{
    std::vector<AppManager::Log> data;
    std::vector<std::string> paths = systemLogPaths;
    for (const auto &app : apps)
    {
        auto logPaths = app->logPaths();
        if (!logPaths.empty())
        {
            for (const auto &path : logPaths)
                paths.push_back(path);
        }
    }

    for (const auto &path : paths)
    {
        std::ifstream f(path);
        if (f.is_open())
        {
            /*std::string s(std::istreambuf_iterator<char>{f}, {});
            size_t fileSize = s.size();
            size_t packedSize = fileSize;
            uLongf ulf_packedSize = fileSize;
            std::vector<uint8_t> output(fileSize);
            if (compress(output.data(), &ulf_packedSize, (Bytef *)s.data(),
                         fileSize) == Z_OK)
            {
                std::vector<uint8_t> packed(4);
                *(uint32_t *)(packed.data()) = bswap_32((uint32_t)packedSize);
                packed.insert(packed.end(), output.begin(),
                              output.begin() + packedSize);
                data.push_back({path, packed});
            }*/
        }
    }
    return data;
}

void AppManager::reload_config()
{
    closeApps();
    loadConfigFile();
    runApps();
}

void AppManager::on_child_finished(pid_t pid)
{
    auto app = get_app_by_pid(pid);

    if (app)
        app->on_child_finished();
}

std::shared_ptr<App> AppManager::get_app_by_pid(pid_t pid)
{
    for (auto &a : apps)
    {
        if (a->pid() == pid)
            return a;
    }
    return nullptr;
}
