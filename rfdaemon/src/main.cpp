#include "main.h"
#include "AppManager.h"
#include "RFDaemonServer.h"
//#include <Beam.h>
#include <console.h>
#include <defs.h>
#include <getopt.h>
#include <httpserver.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <version.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

bool VERBOSE = false;
bool TERMINAL_MODE = true;
bool NOCONSOLE_MODE = false;
bool EDIT_MODE = false;
bool USE_LEGACY_API_PORT = true;
int API_CONSOLE_PORT = RFDAEMON_DEFAULT_API_PORT;
uint16_t RFDAEMON_PORT = DEFAULT_RFDAEMON_PROTO_PORT;
bool NO_HTTP_SERVER = false;
int HTTP_SERVER_PORT = RFDAEMON_DEFAULT_HTTP_PORT;
bool PRINT_LOGS = false;

std::unique_ptr<AppManager> appManager = {};
std::unique_ptr<RFDaemonServer> srv = {};
std::thread srvRxThread;
std::thread systemd_updater_thread;
std::string APPLICATION_LIST_FILE_NAME = "/etc/rfdaemon/applications.json";

void interrupt_signal_handler(int signum);

void interrupt_child(int signum)
{
    (void)signum;
    int status;

    for (;;)
    {
        pid_t p = waitpid(-1, &status, WUNTRACED | WNOHANG);
        if (p < 0)
        {
            nos::println(status, p);
            perror("waitpid");
            exit(1);
        }
        if (p == 0)
            break;

        appManager->on_child_finished(p);
    }
}

void proc_exit(int sig)
{
    (void)sig;
    int retcode;

    while (true)
    {
        pid_t pid = wait3(&retcode, WNOHANG, (struct rusage *)NULL);
        if (pid == 0 || pid == -1)
        {
            // it is not a error
            return;
        }
        nos::fprintln("Proccess {} was stopped. (SIGCHLD)", pid);
        appManager->on_child_finished(pid);
    }
}

int main(int argc, char *argv[])
{
    pid_t daemonPid = 0;

    signal(SIGINT, interrupt_signal_handler);
    signal(SIGTERM, interrupt_signal_handler);
    signal(SIGSEGV, interrupt_signal_handler);
    signal(SIGCHLD, proc_exit);

    checkRunArgs(argc, argv);

    if (EDIT_MODE)
    {
        nos::println("Edit mode");
        std::vector<char *> args = {(char *)"/usr/bin/nano",
                                    (char *)APPLICATION_LIST_FILE_NAME.c_str(),
                                    0};
        int sts = execve(args[0], args.data(), {});
        nos::println("execve status:", sts);
        perror("execve");
        exit(0);
    }

    nos::fprintln("Starting in {} mode.\n",
                  TERMINAL_MODE ? "terminal" : "server");

    if (!TERMINAL_MODE)
        daemonPid = fork();
    if (daemonPid == -1) // main process code part
        nos::println("Error: RFDaemon process fork failed.");
    else if (daemonPid == 0) // fork process code part
    {
        try
        {
            nos::fprintln("Starting RFDaemon legacy server on port {}",
                          RFDAEMON_PORT);
            srv = std::make_unique<RFDaemonServer>(RFDAEMON_PORT);
        }
        catch (const std::exception &ex)
        {
            nos::fprintln(
                "Server exception (maybe the daemon is already running): {}",
                ex.what());
        }

        appManager = std::make_unique<AppManager>(APPLICATION_LIST_FILE_NAME);

        int appfile_error = appManager->loadConfigFile();
        if (appfile_error)
        {
            nos::println(
                "Application script has errors. Server-only mode runned.");
        }
        appManager->runApps();

        if (!NO_HTTP_SERVER)
            start_httpserver(HTTP_SERVER_PORT);

        start_tcp_console(API_CONSOLE_PORT);
        if (USE_LEGACY_API_PORT)
        {
            nos::fprintln("Legacy API port {} is used", 5000);
            start_tcp_console(5000);
        }

        nos::println("RFDaemon started");
        srvRxThread = std::thread(tcpServerReceiveThreadHandler, srv.get(),
                                  appManager.get());
        

        nos::println("Starting systemd updater thread");
        systemd_updater_thread = std::thread(
            &AppManager::update_systemctl_projects_status, 
            appManager.get());


        if (TERMINAL_MODE && !NOCONSOLE_MODE)
        {
            start_stdstream_console();
        }
        try
        {
            srvRxThread.join();
        }
        catch (const std::exception &e)
        {
            nos::println("Error: {}", e.what());
        }
    
    
    }
    return 0;
}

void print_help()
{
    nos::fprint(
        "Usage: rfdaemon [OPTION]...\n"
        "\n"
        "Configuration:\n"
        "  -c, --config - path to settings file\n"
        "  -p, --port - RFDaemon protocol port\n"
        "  -d, --daemon - Start daemon mode (default)\n"
        "  -t, --terminal - Start terminal mode \n"
        "  -n, --noext - Disable rfmeask extention \n"
        "  -e, --edit - Edit configuration file \n"
        "  -J, --nohttp - Without http server \n"
        "  -u  --nolegacyapi - Without legacy api port 5000"
        "  -U  --uselegacyapi - Force use legacy api port 5000"
        "  -H, --http_port - Http server port \n"
        "  -A, --http_port - API server port \n"
        "Debug:\n"
        "  -v, --debug - Print more information\n"
        "  -h  --help - print this\n"
        "  -T, --immediate-exit, Immediate exit for library linking test\n"
        "  -V, --version, Print version\n"
        "  -P, --print-logs - Print logs \n"
        "\n");
}

bool checkRunArgs(int argc, char *argv[])
{
    uint16_t portVal = 0;
    size_t portArgLen = 0;
    int opt = 0;
    bool wrongArg = false;
    char *portArgEnd = NULL;
    int long_index = 0;

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"terminal", no_argument, NULL, 't'},
        {"daemon", no_argument, NULL, 'd'},
        {"debug", no_argument, NULL, 'v'},
        {"config", required_argument, NULL, 'c'},
        {"noext", no_argument, NULL, 'n'},
        {"noconsole", no_argument, NULL, 'N'},
        {"port", required_argument, NULL, 'p'},
        {"edit", no_argument, NULL, 'e'},
        {"immediate-exit", no_argument, NULL, 'T'},
        {"version", no_argument, NULL, 'V'},
        {"nohttp", no_argument, NULL, 'J'},
        {"nolegacyapi", no_argument, NULL, 'U'},
        {"http_port", required_argument, NULL, 'H'},
        {"api_port", required_argument, NULL, 'A'},
        {"print-logs", no_argument, NULL, 'P'},
        {NULL, 0, NULL, 0}};

    while ((opt = getopt_long(argc, argv, "htdvc:np:eTNU", long_options,
                              &long_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);

        case 't':
            TERMINAL_MODE = true;
            break;

        case 'd':
            TERMINAL_MODE = false;
            break;

        case 'v':
            VERBOSE = true;
            break;

        case 'e':
            EDIT_MODE = true;
            break;

        case 'N':
            NOCONSOLE_MODE = true;
            break;

        case 'P':
            PRINT_LOGS = true;
            break;

        case 'T':
            nos::println("immediate exit");
            exit(0);

        case 'V':
            nos::fprintln("RFDaemon version: {}", VERSION);
            exit(0);

        case 'c':
            APPLICATION_LIST_FILE_NAME = std::string(optarg);
            break;

        case 'J':
            NO_HTTP_SERVER = true;
            break;

        case 'H':
            HTTP_SERVER_PORT = std::stoi(std::string(optarg));
            break;

        case 'A':
            API_CONSOLE_PORT = std::stoi(std::string(optarg));
            break;

        case 'U':
            USE_LEGACY_API_PORT = true;
            break;

        case 'u':
            USE_LEGACY_API_PORT = false;
            break;

        case 'p':
        {
            portArgLen = strlen(optarg);
            portArgEnd = optarg;
            portVal = (uint16_t)strtoul(optarg, &portArgEnd, 10);

            if ((size_t)(portArgEnd - optarg) < portArgLen)
            {
                portVal = (uint16_t)strtoul(optarg, &portArgEnd, 16);
                wrongArg = (size_t)(portArgEnd - optarg) < portArgLen;
                if (!wrongArg)
                    RFDAEMON_PORT = portVal;
            }
            else
                RFDAEMON_PORT = portVal;
        }
        break;

        default:
            print_help();
            exit(0);
        }
    }
    return 0;
}

int tcpServerReceiveThreadHandler(RFDaemonServer *srv, AppManager *)
{
    while (!srv)
        usleep(1000);
    return srv->receiveThread();
}

void exitHandler(int sig)
{
    quick_exit(sig);
}

// TODO
void stop_world()
{
    appManager->closeApps();
}

void interrupt_signal_handler(int sig)
{
    nos::println("Interrupt signal received.");
    stop_world();
    exitHandler(sig);
}
