#pragma once

#include <stdint.h>
#include <string>

#define DEFAULT_RFDAEMON_PROTO_PORT 27015

class RFDaemonServer;
class AppManager;

void exitHandler(int sig);
bool checkRunArgs(int argc, char *argv[]);
int tcpServerSendThreadHandler(RFDaemonServer *, AppManager *);
int tcpServerReceiveThreadHandler(RFDaemonServer *, AppManager *);
int userIOThreadHandler();
