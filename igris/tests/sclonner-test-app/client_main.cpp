#include <nos/input.h>
#include <nos/io/stdfile.h>
#include <nos/print.h>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    nos::println("stdout:HelloWorld");
    nos::println_to(nos::cerr, "stderr:HelloWorld");

    std::this_thread::sleep_for(10ms);
}
