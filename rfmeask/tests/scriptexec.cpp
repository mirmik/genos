#include <doctest/doctest.h>
#include <scriptexec.h>
#include <sys/wait.h>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

TEST_CASE("scriptexec")
{
/*    int pid, pid2, sts;
    CHECK_EQ(fs::exists("kekeke"), false);

    set_script_directory("scriptdir");
    sts = script_execution("test.py");

    CHECK_EQ(sts, 0);
    pid2 = wait(&sts);

    CHECK_EQ(WEXITSTATUS(sts), 42);
*/
}