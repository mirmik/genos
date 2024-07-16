#include <igris/clonner.h>
#include <nos/io/stdfile.h>
#include <nos/print.h>

#include <chrono>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;
using namespace std::chrono_literals;

int main()
{
    igris::clonner clonner;

    auto path = fs::absolute("client");
    nos::println(path.c_str());

    auto subproc = clonner.start_subprocess(path.c_str());

    std::this_thread::sleep_for(10ms);

    nos::println("before wait");
    subproc->wait();
    nos::println("after wait");
}
