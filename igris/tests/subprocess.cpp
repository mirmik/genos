#include <doctest/doctest.h>
#include <igris/sclonner.h>

TEST_CASE("SCLONNER")
{
    char txt[48];
    memset(txt, 0, 48);
    igris::subprocess proc;

    proc.exec("/bin/echo ECHO: subprocess test 1");
    proc.wait();

    proc.exec("/bin/cat tests/text.txt");
    proc.wait();

    read(proc.output_fd(), txt, 48);
    // CHECK_EQ(r, 7);
    // CHECK_EQ(strcmp(txt, "AAABBB\n"), 0);
}