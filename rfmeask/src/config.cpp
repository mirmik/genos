/**
    @file
*/

#include <config.h>
#include <fcntl.h>
#include <unistd.h>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

//#include <igris/debug/debug_ostream.h>
#include <chrono>
#include <igris/util/string.h>
#include <logging.h>
#include <nos/log.h>
#include <thread>

nos::json_settings config_settings;
nos::json_settings objects_runtime;
nos::json_syncer runtime_syncer;

void config_load(std::string path, std::string runtime)
{
    nos::fprintln("config_path: {}", path);
    nos::fprintln("objects_runtime: {}", runtime);

    if (!fs::exists(path))
    {
        int fd = ::open(path.c_str(), O_WRONLY | O_CREAT, 0666);

        if (fd < 0)
        {
            nos::fprintln("Fault: file \"{}\" is not exists", path);
            nos::fprintln("Try to create fault: sts:{}\n", fd);
            exit(-1);
        }

        const char *settings_template =
            R"({
    "loglevel": "debug",
    "idn": ["RADIOLINE", "a", "b", "c"],
    "paths": {
        "mrs": "/dev/ttyS3",
        "laz": "/dev/ttyS0",
        "drv": "/dev/ttyS2"
    },
    "ports": {
        "scpi": 5025,
        "ncpi": 5026,
        "data": 5802
    },
    "axes": [],
    "devices": [],
    "intgroups": []
}
)";
        ::write(fd, settings_template, strlen(settings_template));
        ::close(fd);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!fs::exists(runtime))
    {
        int fd = ::open(runtime.c_str(), O_WRONLY | O_CREAT, 0666);

        if (fd < 0)
        {
            nos::fprintln("Fault: file \"{}\" is not exists", runtime);
            nos::fprintln("Try to create fault: sts:{}\n", fd);
            exit(-1);
        }

        ::write(fd, "{}\n", 3);
        ::close(fd);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    config_settings.path(path);
    objects_runtime.path(runtime);
    runtime_syncer.path(runtime);

    nos::print("config_settings parse");
    fflush(stdout);
    config_settings.sync();
    nos::println("...ok");

    nos::print("objects_runtime parse");
    fflush(stdout);
    objects_runtime.sync();
    nos::println("...ok");

    nos::print("runtime syncer parse");
    fflush(stdout);
    runtime_syncer.sync();
    nos::printhexln(&runtime_syncer);
    nos::println("...ok");
    fflush(stdout);

    nos::trent &js = config_settings.node();
    if (js.is_nil())
    {
        nos::println("DPRINT: Config file wasn't found or has invalid format");
        exit(-1);
    }

    nos::trent &rjs = objects_runtime.node();
    if (rjs.is_nil())
    {
        nos::println("DPRINT: Runtime file wasn't found or has invalid format");
        exit(-1);
    }

    nos::println("configs was sucessfually readed");
}
