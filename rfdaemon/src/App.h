#pragma once

#include <igris/sclonner.h>
#include <mutex>
#include <nos/trent/trent.h>
#include <optional>
#include <pwd.h>
#include <queue>
#include <rxcpp/rx.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <rxcpp/rx-subjects.hpp>

class LinkedFile
{
public:
    std::string path = {};
    std::string name = {};
    bool editable = {};

    nos::trent to_trent() const;
    LinkedFile() = default;
    LinkedFile(const LinkedFile &) = default;
};

    enum RestartMode
    {
        ALWAYS = 0,
        ONCE = 1,
    };

class IApp
{
};



class App : public IApp
{
private:
    std::string _username = {};
    uid_t _uid = 0;
    std::vector<std::string> _args = {};
    std::vector<LinkedFile> _linked_files = {};
    int task_index = {};
    std::chrono::time_point<std::chrono::system_clock> _startTime = {};
    std::vector<std::string> tokens = {};
    int32_t _attempts_initializer = 5;
    int32_t _attempts = _attempts_initializer;
    bool _watcher_guard = false;
    std::thread _watcher_thread = {};
    int _exitStatus = 0;
    RestartMode _restartMode = RestartMode::ALWAYS;
    std::string _name = {};
    std::queue<int> _errors = {};
    // int _pid = 0;
    igris::subprocess proc = {};
    bool cancel_reading = false;
    std::string _stdout_record = {};
    std::unordered_map<std::string, std::string> _env;

    rxcpp::subjects::subject<std::string> logstream_subject;
    int systemd_pid = 0;

public:
    std::string systemd_bind;
    bool isStopped = true;

public:
    auto logstream_subject_observable()
    {
        return logstream_subject.get_observable();
    }

    App(int task_index, const std::string &name, const std::string &cmd,
        RestartMode mode, const std::vector<LinkedFile> &linkeds,
        std::string user);

    App(const App &) = delete;
    App &operator=(const App &) = delete;
    App(App &&) = default;
    App &operator=(App &&) = default;

    bool is_systemctl_process(); 
    void stop();
    void start();
    void restart();
    bool stopped() const;
    void set_user(const std::string &user);
    RestartMode restartMode() const;
    int pid() const;
    void set_pid(int p);
    const std::string &name() const;
    const std::vector<std::string> &args() const;
    int64_t uptime() const;
    uint32_t restartAttempts() const;
    pid_t waitFinish();
    void run();
    std::vector<std::string> logPaths() const;
    std::queue<int> &errors();
    std::string status_string() const;
    std::string command() const;
    std::vector<std::string> envp_base_for_execve();
    std::vector<char *> envp_for_execve(const std::vector<std::string> &args);
    void set_systemd_bind(const std::string& service);

    bool need_to_another_attempt() const;
    void increment_attempt_counter();
    void restart_attempt_counter();

    static std::vector<char *>
    tokens_for_execve(const std::vector<std::string> &args);

    void logdata_append(const char *data, size_t size);
    size_t logdata_size() const;
    void logdata_clear();
    int64_t logdata_read(char *data, size_t size, size_t offset);
    void on_child_finished();
    void set_environment_variables(
        const std::unordered_map<std::string, std::string> &env);

    std::string token_list_as_string() const;

    bool is_runned()
    {
        return !isStopped;
    }

    const std::string &show_stdout() const;
    const std::vector<LinkedFile> &linked_files() const
    {
        return _linked_files;
    }

    ~App()
    {
        try
        {
            stop();
            _watcher_thread.join();
        }
        catch (...)
        {
            // path
        }
    }

private:
    void watchFunc();
    void appFork();
};
