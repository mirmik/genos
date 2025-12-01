#include <doctest/doctest.h>
#include <igris/creader.h>

#include <string>

TEST_CASE("creader.readline")
{
    const char *str = " lalalal \n casdc  \r\n ";

    struct creader reader;
    const char *token;
    ptrdiff_t len;

    creader_init(&reader, str, strlen(str));

    len = creader_readline(&reader, &token);
    CHECK_EQ(len, 9);
    CHECK_EQ(std::string(token, len), std::string(" lalalal "));

    len = creader_readline(&reader, &token);
    CHECK_EQ(len, 8);
    CHECK_EQ(std::string(token, len), std::string(" casdc  "));
}
