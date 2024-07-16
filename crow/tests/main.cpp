#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include <crow/gates/udpgate.h>
#include <crow/gates/loopgate.h>
#include <crow/tower.h>
#ifdef __WIN32__
#include <winsock2.h>
WSADATA wsaData;
#endif

crow::udpgate udpgate;
crow::loopgate loopgate;
int main(int argc, char** argv)
{
#ifdef __WIN32__
int iResult;

// Initialize Winsock
iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
}
#endif

	crow::retransling_allowed = true;
	loopgate.bind(99);
	udpgate.bind(12);
	udpgate.open(10099);

	doctest::Context context;

	int res = context.run(); // run

	if (context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
		return res;          // propagate the result of the tests
	int client_stuff_return_code = 0;
	return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
