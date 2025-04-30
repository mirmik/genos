#include <Python.h>
#include <filesystem>
#include <iostream>
#include <nos/trent/json.h>
#include <nos/trent/json_print.h>
#include <plugins/py.h>
#include <pybind11/pybind11.h>
#include <signal.h>

python_executor *python_executor::_instance;

int state = 42; // Глобальное состояние

int get_testvariable()
{
    return state;
}

void set_testvariable(int new_state)
{
    state = new_state;
}

void python_executor::destroy_instance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

// Оборачиваем код в pybind11
PYBIND11_MODULE(my_module, m)
{
    m.doc() = "Example module with shared state";
    m.def("get_testvariable", &get_testvariable, "Get the current state");
    m.def("set_testvariable", &set_testvariable, "Set a new state");
}

python_executor::python_executor()
{
    _old_sigint_handler = PyOS_getsig(SIGINT);
    Py_Initialize();
    _sys_modules = PyImport_GetModuleDict();
    _my_module = PyInit_my_module();
    if (_my_module == nullptr)
    {
        nos::println("Failed to create Python module");
        Py_Finalize();
        return;
    }
    _inited = true;
    PyDict_SetItemString(_sys_modules, "my_module", _my_module);
    PyOS_setsig(SIGINT, SIG_DFL);
}

python_executor::~python_executor()
{
    if (_inited)
    {
        Py_DECREF(_my_module);
        Py_Finalize();
    }
}

void python_script_hook::init(std::string path, std::string default_script)
{
    _script = default_script;
    _path = path;

    if (path.empty())
    {
        set_script(default_script);
        return;
    }

    if (std::filesystem::exists(path))
    {
        open_file(path);
    }
    else
    {
        set_script(default_script);
        save_to_file();
    }
}

python_script_hook::python_script_hook(std::string path,
                                       std::string default_script)
{
    init(path, default_script);
}

void python_script_hook::open_file(const std::string &path)
{
    nos::file file(path.c_str(), O_RDONLY);
    _path = path;
    if (!file.good())
    {
        is_existed = false;
        return;
    }
    _script = *nos::readall_from(file);
    is_existed = true;
}

void python_script_hook::set_script(const std::string &script)
{
    _script = script;
    is_existed = true;
}

void python_script_hook::save_to_file()
{
    // make directory
    std::filesystem::create_directories(
        std::filesystem::path(_path).parent_path());

    nos::file file(_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
    if (!file.good())
    {
        return;
    }
    file.write(_script.c_str(), _script.size());
    is_existed = true;

    int fd = file.fd();

    // set file permissions 666
    fchmod(fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

void PyObjectToTrent(PyObject *input, nos::trent &output)
{
    if (PyBool_Check(input))
    {
        bool value = PyObject_IsTrue(input);
        output = value;
    }
    else if (PyLong_Check(input))
    {
        long value = PyLong_AsLong(input);
        output = value;
    }
    else if (PyFloat_Check(input))
    {
        double value = PyFloat_AsDouble(input);
        output = value;
    }
    else if (PyUnicode_Check(input))
    {
        const char *value = PyUnicode_AsUTF8(input);
        output = value;
    }
    else if (PyDict_Check(input))
    {
        PyObject *pKeys = PyDict_Keys(input);
        PyObject *pValues = PyDict_Values(input);

        Py_ssize_t nKeys = PyList_Size(pKeys);
        // Py_ssize_t nValues = PyList_Size(pValues);

        for (Py_ssize_t i = 0; i < nKeys; i++)
        {
            PyObject *pKey = PyList_GetItem(pKeys, i);
            PyObject *pValue = PyList_GetItem(pValues, i);

            const char *key = PyUnicode_AsUTF8(pKey);
            nos::trent subresult;
            PyObjectToTrent(pValue, subresult);
            output[key] = subresult;
        }

        Py_DECREF(pKeys);
        Py_DECREF(pValues);
    }
    else if (PyList_Check(input))
    {
        Py_ssize_t n = PyList_Size(input);
        for (Py_ssize_t i = 0; i < n; i++)
        {
            PyObject *pValue = PyList_GetItem(input, i);
            nos::trent subresult;
            PyObjectToTrent(pValue, subresult);
            output.push_back(subresult);
        }
    }
    else if (PyTuple_Check(input))
    {
        Py_ssize_t n = PyTuple_Size(input);
        for (Py_ssize_t i = 0; i < n; i++)
        {
            PyObject *pValue = PyTuple_GetItem(input, i);
            nos::trent subresult;
            PyObjectToTrent(pValue, subresult);
            output.push_back(subresult);
        }
    }
    else
    {
        std::cerr << "Unsupported type in Python script result" << std::endl;
    }
}

nos::trent GetResultFromGlobal(PyObject *pLocal)
{
    nos::trent result;

    PyObject *pResult = PyDict_GetItemString(pLocal, "result");

    if (pResult == nullptr)
    {
        std::cerr << "Failed to get result from Python script" << std::endl;
        return result;
    }

    PyObjectToTrent(pResult, result);

    return result;
}

PyObject *PyObjectFromTrent(const nos::trent &input)
{
    if (input.is_numer())
    {
        double value = input.as_numer();
        return PyFloat_FromDouble(value);
    }
    else if (input.is_bool())
    {
        bool value = input.as_bool();
        return PyBool_FromLong(value);
    }
    else if (input.is_string())
    {
        const char *value = input.as_string().c_str();
        PyObject *out = PyUnicode_FromString(value);
        const char *out_str = PyUnicode_AsUTF8(out);
        (void)out_str; // suppress unused variable warning
        return out;
    }
    else if (input.is_dict())
    {
        PyObject *pDict = PyDict_New();
        for (auto &pair : input.as_dict())
        {
            PyObject *pKey = PyUnicode_FromString(pair.first.c_str());
            PyObject *pValue = PyObjectFromTrent(pair.second);
            PyDict_SetItem(pDict, pKey, pValue);
            Py_DECREF(pKey);
            Py_DECREF(pValue);
        }
        return pDict;

        // PyObject *pObj =
        //     PyObject_CallObject((PyObject *)&PyBaseObject_Type, NULL);
        // for (auto &pair : input.as_dict())
        // {
        //     PyObject *pKey = PyUnicode_FromString(pair.first.c_str());
        //     PyObject *pValue = PyObjectFromTrent(pair.second);
        //     PyObject_SetAttr(pObj, pKey, pValue);
        //     Py_DECREF(pKey);
        //     Py_DECREF(pValue);
        // }
        // return pObj;

        // as object. not dict. We need state.a instead of state["a"]
        // PyObject *pDict = PyObject_New(PyObject, nullptr);
        // for (auto &pair : input.as_dict())
        // {
        //     PyObject *pKey = PyUnicode_FromString(pair.first.c_str());
        //     PyObject *pValue = PyObjectFromTrent(pair.second);
        //     PyObject_SetAttr(pDict, pKey, pValue);
        //     Py_DECREF(pKey);
        //     Py_DECREF(pValue);
        // }
        // return pDict;
    }
    else if (input.is_list())
    {
        auto &list = input.as_list();
        PyObject *pList = PyList_New(list.size());
        for (size_t i = 0; i < list.size(); i++)
        {
            PyObject *pValue = PyObjectFromTrent(input[i]);
            PyList_SetItem(pList, i, pValue);
        }
        return pList;
    }
    else
    {
        std::cerr << "Unsupported type in Trent:" << nos::json::to_string(input)
                  << std::endl;
        return nullptr;
    }
}

void SetIndataToGlobal(const nos::trent &indata, PyObject *pGlobal)
{
    if (indata.is_nil())
    {
        return;
    }

    PyObject *pIndata = PyObjectFromTrent(indata);
    PyDict_SetItemString(pGlobal, "indata", pIndata);
    Py_DECREF(pIndata);
}

void python_executor::run(const std::string &script,
                          const nos::trent &indata,
                          nos::trent &outdata)
{
    if (!_inited)
        return;

    PyObject *pGlobal = PyDict_New();
    PyObject *pLocal = PyDict_New();

    SetIndataToGlobal(indata, pGlobal);

    PyRun_String(script.c_str(), Py_file_input, pGlobal, pLocal);
    PyOS_setsig(SIGINT, _old_sigint_handler);

    if (PyErr_Occurred())
    {
        nos::println(PyErr_Occurred());
        PyErr_Print();
        std::cerr << "LALALA:: Failed to run Python script" << std::endl;
    }

    auto result = GetResultFromGlobal(pLocal);

    outdata.init(nos::trent_type::dict);
    outdata["result"] = result;

    // 6. Очистка
    Py_DECREF(pGlobal);
    Py_DECREF(pLocal);
}