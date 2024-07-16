#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#if defined(__WIN32__) || defined(_MSC_VER)
#include <winsock2.h>
WSADATA wsaData;
#endif

int main(int argc, char **argv)
{
#if defined(__WIN32__) || defined(_MSC_VER)
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
#endif

    doctest::Context context;
    int res = context.run();  // run
    if (context.shouldExit()) // important - query flags (and --exit) rely on
                              // the user doing this
        return res;           // propagate the result of the tests
    int client_stuff_return_code = 0;
    return res + client_stuff_return_code; // the result from doctest is
                                           // propagated here as well
}
