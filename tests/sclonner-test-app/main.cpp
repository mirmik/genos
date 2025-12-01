#include <igris/sclonner.h>
#include <nos/io/file.h>
#include <nos/io/stdfile.h>
#include <nos/print.h>

#include <chrono>
#include <filesystem>
#include <thread>

#include <signal.h>

namespace fs = std::filesystem;
using namespace std::chrono_literals;

void sigpipe(int sig)
{
    nos::println("SIGPIPE");
    exit(0);
}

void on_close() { nos::println("ON_CLOSE"); }

int main()
{
    signal(SIGPIPE, sigpipe);

    igris::sclonner clonner;
    clonner.install_sigchild_trap();

    auto path = fs::absolute("client");
    nos::println(path.c_str());

    auto subproc = clonner.start_subprocess(path.c_str());
    subproc->on_close = on_close;

    std::this_thread::sleep_for(1000ms);
}
