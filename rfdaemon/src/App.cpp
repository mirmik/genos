#include "App.h"
#include "AppManager.h"
#include <igris/util/base64.h>
#include <igris/util/string.h>
#include <iostream>
#include <modes.h>
#include <nos/fprint.h>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

extern AppManager *appManager;

// std::mutex App::ioMutex;
using namespace std::chrono_literals;

nos::trent LinkedFile::to_trent() const
{
    nos::trent tr;
    tr["path"] = path;
    tr["name"] = name;
    tr["editable"] = editable;
    return tr;
}

void App::set_user(const std::string &user)
{
    this->_username = user;
    if (!user.empty())
    {
        struct passwd *pw = getpwnam(user.c_str());
        if (pw)
        {
            this->_uid = pw->pw_uid;
            //    this->_gid = pw->pw_gid;
        }
    }
}

std::string App::status_string() const
{
    if (isStopped)
        return "stopped";
    else
        return "running";
}

std::string App::command() const
{
    std::string cmd = "";
    for (auto &arg : tokens)
    {
        cmd += arg + " ";
    }
    return cmd;
}

std::string App::token_list_as_string() const
{
    std::string cmd = "";
    cmd += "[";
    for (unsigned int i = 0; i < tokens.size(); ++i)
    {
        auto &arg = tokens[i];
        cmd += arg;
        if (i != tokens.size() - 1)
            cmd += ",";
    }
    cmd += "]";
    return cmd;
}

std::string GetStdoutFromCommand(std::string cmd)
{

    std::string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
        {
            if (fgets(buffer, max_buffer, stream) != NULL)
                data.append(buffer);
        }
        pclose(stream);
    }
    return data;
}

App::App(int task_index, const std::string &name, const std::string &cmd,
         RestartMode mode, const std::vector<LinkedFile> &linkeds,
         std::string user)
    : _linked_files(linkeds), task_index(task_index), _name(name)
{
    tokens = igris::split_cmdargs(cmd);
    _restartMode = mode;
    set_user(user);
}


std::string execute_and_read_output(const std::string& cmd) 
{
    igris::subprocess proc;
    std::vector<std::string> args = igris::split(cmd);
    proc.exec(args[0], args, {});
    int fd = proc.output_fd();
    proc.wait();
    std::string out;
    out.resize(1024);
    int n = read(fd, out.data(), out.size());
    if (n>=0) {
        out.resize(n);
        return out;
    }
    else 
        return "";    
}

void App::stop()
{
    if (systemd_bind != "") 
    {
        std::string cmd = "/usr/bin/systemctl stop " + systemd_bind;
        std::string out = execute_and_read_output(cmd);
        nos::println(out);
        return;
    }

    if (!isStopped)
    {
        _attempts = 0;
        proc.kill();
    }
}

void App::restart()
{
    stop();
    std::this_thread::sleep_for(0.5s);
    start();
}

void App::start()
{
    if (systemd_bind != "") 
    {
        std::string cmd = "/usr/bin/systemctl start " + systemd_bind;
        nos::fprintln("Start in systemctl mode: {}", cmd);
        restart_attempt_counter();
        std::string out = execute_and_read_output(cmd);
        nos::println(out);
        isStopped = false;
        _startTime = std::chrono::system_clock::now();
        systemd_pid = 0;
        return;
    }

    if (isStopped)
    {
        restart_attempt_counter();
        run();
    }
}

std::vector<char *> App::tokens_for_execve(const std::vector<std::string> &args)
{
    std::vector<char *> res;
    for (auto &arg : args)
    {
        res.push_back(const_cast<char *>(arg.c_str()));
    }
    res.push_back(nullptr);
    return res;
}

std::vector<std::string> App::envp_base_for_execve()
{
    std::vector<std::string> res;
    for (auto &arg : _env)
    {
        std::string str = arg.first + "=" + arg.second;
        res.push_back(str);
    }
    return res;
}

std::vector<char *> App::envp_for_execve(const std::vector<std::string> &args)
{
    std::vector<char *> res;
    for (auto &arg : args)
    {
        res.push_back(const_cast<char *>(arg.c_str()));
    }
    res.push_back(nullptr);
    return res;
}

void App::set_systemd_bind(const std::string& service) 
{
    this->systemd_bind = service;
}

// Call only in a separate thread
void App::appFork()
{
    _exitStatus = 0;
    increment_attempt_counter();

    // int wr[2];
    // pipe(wr);
    _startTime = std::chrono::system_clock::now();

    auto envp_base = envp_base_for_execve();
    auto envp = envp_for_execve(envp_base);
    auto args = tokens_for_execve(tokens);

    nos::println("Start subprocess:", name());
    nos::println("tokens:", tokens);
    proc.exec(tokens[0].data(), args, envp);
    int fd = proc.output_fd();

    std::vector<uint8_t> buffer;
    char buf[1024];
    buffer.reserve(2048);

    isStopped = false;
    while (true)
    {
        int n = read(fd, buf, sizeof(buf));
        if (n > 0)
        {
            logdata_append(buf, n);
            buffer.clear();
            buffer.push_back((uint8_t)task_index);
            buffer.push_back((uint8_t)name().size());
            buffer.insert(buffer.end(), _name.begin(), _name.end());
            uint16_t len = n;
            buffer.push_back((uint8_t)(len >> 8));
            buffer.push_back((uint8_t)(len & 0xFF));
            buffer.insert(buffer.end(), buf, buf + n);
            appManager->send_spam(buffer);
        }

        if (cancel_reading) {
            nos::println("stopped because reading was canceled");
            break;
        }
        std::this_thread::sleep_for(100ms);
    }
    isStopped = true;
    cancel_reading = false;
    nos::println("Finish subprocess:", name());
}

bool App::stopped() const
{
    return isStopped;
}

RestartMode App::restartMode() const
{
    return _restartMode;
}

int App::pid() const
{
    if (!systemd_bind.empty()) 
    {
        return systemd_pid; 
    }

    return proc.pid();
}

const std::string &App::name() const
{
    return _name;
}

// retrurn log pathes
std::vector<std::string> App::logPaths() const
{
    return {};
}

int64_t App::uptime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now() - _startTime)
        .count();
}

void App::restart_attempt_counter()
{
    _attempts = _attempts_initializer;
}

void App::increment_attempt_counter()
{
    _attempts--;
    assert(_attempts >= 0);
}

bool App::need_to_another_attempt() const
{
    return restartMode() == RestartMode::ALWAYS && _attempts != 0;
}

void App::watchFunc()
{
    while (1)
    {
        std::this_thread::sleep_for(10ms);
        appFork();

        if (!need_to_another_attempt())
            break;
    }
    _watcher_guard = false;
    //_pid = 0;
    proc.invalidate();
}

void App::run()
{
    if (!_watcher_guard)
    {
        _watcher_guard = true;
        if (_watcher_thread.joinable())
            _watcher_thread.join();
        _watcher_thread = std::thread(&App::watchFunc, this);
    }
    else
        nos::fprintln(
            "Can't start app '{}' because it's wather is already running",
            name());
}

std::queue<int> &App::errors()
{
    return _errors;
}

void App::logdata_clear()
{
    _stdout_record.clear();
    _stdout_record.reserve(1024 * 1024);
}

void App::logdata_append(const char *data, size_t size)
{
    if (PRINT_LOGS)
        nos::print(nos::buffer(data, size));
    logstream_subject.get_subscriber().on_next(std::string{data, size});
    _stdout_record.append(data, size);
}

size_t App::logdata_size() const
{
    return _stdout_record.size();
}

int64_t App::logdata_read(char *buf, size_t size, size_t offset)
{
    if (offset >= _stdout_record.size())
        return 0;
    auto fullsize = logdata_size();
    if (offset + size > fullsize)
        size = fullsize - offset;
    memcpy(buf, _stdout_record.data() + offset, size);
    return size;
}

const std::string &App::show_stdout() const
{
    return _stdout_record;
}

void App::on_child_finished()
{
    cancel_reading = true;
}

void App::set_environment_variables(
    const std::unordered_map<std::string, std::string> &env)
{
    _env = env;
}

bool App::is_systemctl_process() 
{
    return systemd_bind != "";
}

void App::set_pid(int p) 
{
    systemd_pid = p;
}

void AppManager::update_systemctl_projects_status() 
{
    while (true) 
    {
        for (std::shared_ptr<App> p: applications()) 
        {
            if (p->is_systemctl_process()) 
            {
                auto name = p->systemd_bind;

                if (p->pid() == 0) 
                {
                    auto out = execute_and_read_output(nos::format("/usr/bin/systemctl show --property MainPID --value {}", name));
                    auto trimmed = nos::trim(out);
                    int pid = std::stoi(trimmed);
                    p->set_pid(pid);
                }

                {
                    auto out = execute_and_read_output(nos::format("/usr/bin/systemctl is-active {}", name));
                    auto trimmed = nos::trim(out);
                    bool is_active = trimmed == "active";
                    p -> isStopped = !is_active;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}