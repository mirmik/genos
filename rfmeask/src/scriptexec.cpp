#include <nos/print.h>
#include <scriptexec.h>
#include <unistd.h>

#include <igris/sclonner.h>
#include <memory>
#include <nos/fprint.h>

#include <chrono>
#include <thread>

#include <AbstractAxis.h>
#include <tables.h>

using namespace std::chrono_literals;

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

static std::string SCRIPTDIR = "scriptdir";
std::unique_ptr<igris::sclonner> clonner = std::make_unique<igris::sclonner>();

void set_script_directory(const std::string &dirpath)
{
    SCRIPTDIR = dirpath;
}

std::string script_directory()
{
    return SCRIPTDIR;
}

int script_execution(const std::string &name)
{
    int status = -1;
    auto scriptdir = fs::absolute(SCRIPTDIR);
    auto scriptpath = scriptdir / name;

    if (fs::exists(scriptpath))
    {
        if (clonner->childs().size() > 0)
        {
            clonner->terminate_childs();
            std::this_thread::sleep_for(10ms);

            for (auto a : get_axis_list())
            {
                a->stop();
            }

            std::this_thread::sleep_for(2000ms);
        }

        // child = popen(scriptpath.c_str(), "r");
        clonner->start_subprocess(scriptpath.c_str());
        status = 0;
    }

    return status;
}

void script_terminate_all()
{
    nos::fprintln("terminate scripts : {}", clonner->childs().size());

    clonner->terminate_childs();
}