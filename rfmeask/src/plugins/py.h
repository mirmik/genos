#ifndef PLUGINS_PY_H
#define PLUGINS_PY_H

#include <Python.h>
#include <fcntl.h>
#include <nos/input.h>
#include <nos/io/file.h>
#include <nos/trent/trent.h>
#include <pybind11/pybind11.h>

class python_executor
{
    static python_executor *_instance;

    bool _inited = false;
    PyObject *_sys_modules;
    PyObject *_my_module;
    PyOS_sighandler_t _old_sigint_handler;

public:
    python_executor();
    ~python_executor();
    void run(const std::string &script,
             const nos::trent &indata,
             nos::trent &outdata);

    static python_executor &instance()
    {
        if (_instance == nullptr)
            _instance = new python_executor();
        return *_instance;
    }

    static void destroy_instance()
    {
        if (_instance != nullptr)
        {
            delete _instance;
            _instance = nullptr;
        }
    }
};

class hook
{
protected:
    bool is_existed = false;

public:
    virtual void run(const nos::trent &indata, nos::trent &outdata) = 0;

    void execute(const nos::trent &indata, nos::trent &outdata)
    {
        if (!is_existed)
            return;
        run(indata, outdata);
    }
};

class python_script_hook : public hook
{
    std::string _script;

public:
    python_script_hook();
    ~python_script_hook();

    void open_file(const std::string &path);
    void set_script(const std::string &script);

    void run(const nos::trent &indata, nos::trent &outdata) override
    {
        python_executor::instance().run(_script, indata, outdata);
    }
};

#endif