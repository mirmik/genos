/**
    @file main.cpp
*/

#include <LogicalDevice.h>
#include <MitsubishiCommunicator.h>
#include <RS232Listener.h>
#include <System.h>
#include <comm/console.h>
#include <comm/jsonsocket.h>
#include <comm/ncpi.h>
#include <comm/notifymap.h>
#include <comm/scpi-def.h>
#include <config.h>
#include <consoleMessage.h>
#include <crow-support.h>
#include <crow/tower.h>
#include <deprecated/lazerproxy.h>
#include <devices/DevAlarmLogger.h>
#include <devices/Device.h>
#include <devices/DeviceChecker.h>
#include <getopt.h>
#include <groups/InterpolationGroup.h>
#include <httpserver.h>
#include <igris/getopt/cliopts.h>
#include <igris/path/path.h>
#include <ircc/ircc.h>
#include <logging.h>
#include <modes.h>
#include <nos/io/stdfile.h>
#include <nos/print.h>
#include <rxnotifier.h>
#include <scriptexec.h>
#include <selftest.h>
#include <tables.h>
#include <thread>
#include <util.h>
#include <version.h>

#if (HAVE_LAZER_PROXY)
extern LaserProxy laser_proxy;
#endif

#if HAVE_MITSUBISHI_BOARD
#include <MitsubishiBoard.h>
#endif

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#if MODBUS_ENABLED
#include <ModbusCommunicator.h>
ModbusCommunicator modbus;
#endif

bool WITH_CONSOLE = false;
std::string executable_path;
std::string executable_directory;
std::string config_path = "settings.json";
std::string confdir_path = "";
std::string runtime_path = "runtime.json";
std::string errFile = "errlog.txt";

std::string home = get_user_dir();
std::string simulator_config_path = home == ""
                                        ? "/var/local/rfmeask/simulator-configs"
                                        : home + "/.rfmeask/simulator-configs";

// TelemetryMonitor telemetry_monitor;
// MitsubishiCommunicator mrs;
std::list<std::thread> threadlist;

std::optional<int> simulator_test;

int scpi_port = -1; // default: 5025
int ncpi_port = -1; // default: 5026
// int data_port = -1;           // default: 5802
int proto_socket_port = 5800; // default: 5800
int http_port = 9003;         // default: 9003
int crow_port = 0;            // default: dynamic

#if (HAVE_MITSUBISHI_BOARD)
MitsubishiBoard mboard;
#endif

#include "modes.h"

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <time.h>

DevAlarmLogger devAlarmLogger;

#ifdef WIN32
#include <winsock2.h>
#endif

#include <cankollm/cankollm.h>
#include <cankollm/canopen.h>
#include <cankollm/kollmmap.h>

void open_hooks();
void open_global_hooks();
void open_mitsuservo_communicator_ports();

void rfmeask_start()
{
    // crow_configure(crow_port);

    messageBusNotify->setNCPIHeader(NPattern_MessageBus,
                                    std::vector<int32_t>({}));
    brocker.themeRegistry(messageBusNotify, Tag_MessageBus, {});

    messageChannelNotify->setNCPIHeader(NPattern_MessageChannel,
                                        std::vector<int32_t>({}));
    brocker.themeRegistry(messageChannelNotify, Tag_MessageChannel, {});

    /*Инициализация таблиц*/

    initialize_the_system_according_to_the_configuration();

    if (PARSE_ONLY)
        exit(0);
    nos::log::info("init system device checker");
    System::instance()->setDevices(devices_list);
    nos::log::info("system device list:");
    for (unsigned int i = 0; i < devices_list.size(); ++i)
        nos::log::info(devices_list[i]->name());
    nos::log::info("setup system device notifies");
    System::instance()->notifiesRegistry();
    nos::log::info("init communication ports");

    /*Открытие портов для коммуникаторов*/
    // if (have_mrs)
    // {
    //     nos::log::info("open mrs port");
    //     mrs.open(
    //         config_settings.node()["paths"]["mrs"].as_string_except().c_str());
    // }

    open_mitsuservo_communicator_ports();

#if MODBUS_ENABLED == 1
    if (have_mrs_modbus)
    {
        nos::log::info("open mrs modbus port");
        modbus.open(
            config_settings.node()["paths"]["mrs"].as_string_except().c_str());
    }
#endif

    if (have_drv)
    {
        nos::log::info("open drv port");
        drv.open(
            config_settings.node()["paths"]["drv"].as_string_except().c_str());
    }

    if (have_can)
    {
        kollmmap_init();
        can.driver = &cankollm;
        cankollm.can = &can;
        can.open(
            config_settings.node()["paths"]["can"].as_string_except().c_str());
    }

    nos::log::trace("check ports config");

    if (scpi_port == -1)
        scpi_port = config_settings.node()["ports"]["scpi"].as_integer_except();

    if (ncpi_port == -1)
        ncpi_port = config_settings.node()["ports"]["ncpi"].as_integer_except();

#if (HAVE_MITSUBISHI_BOARD)
    nos::log::info("open mitsubishi board");

    if (have_mitsu_board_servos)
        mboard.open(0);
#endif

#if (HAVE_LAZER_PROXY)
    nos::log::info("init lazer proxy");
    laser_proxy.init(config_settings.node()["paths"]["laz"].as_string_except());
#endif

    nos::log::info("connect dev to signals");
    for (auto dev : devices_list)
    {
        nos::log::info("connect {} to signals", dev->name());
        /*Подключение отладочных сообщений консоли к сигналам устройств*/
        dev->get_device_status_observable().subscribe([](const auto &pair) {
            auto status = pair.first;
            if (status == DeviceStatus::Ready)
                consuccess(pair.second);
            else if (status == DeviceStatus::Refuse)
                debugErrorRefuse(pair.second);
            else if (status == DeviceStatus::Alarm)
                debugErrorAlarm(pair.second);
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    init_interpolation_groups();
    open_global_hooks();
    open_hooks();

    /*Запуск потоков управления, серверов, коммуникации*/
    start_httpserver(http_port);
    JsonApiServer::instance().start(proto_socket_port);
    start_scpi_server(scpi_port);
    start_ncpi_server(ncpi_port);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    if (have_can)
        threadlist.emplace_back(+[]() { can.recv_func(); });

    if (have_drv)
        threadlist.emplace_back(rs232listener_func);

    // telemetry_monitor.start_spin();
    if (WITH_CONSOLE)
        start_io_console_thread();

    for (auto dev : devices_list)
    {
        nos::log::info("Start updater thread {}", dev->name());
        /*Запуск опроса состояния устройств*/
        dev->startUpdate();
    }

    RxNotifier::instance().set_devices(devices_list);

    if (simulator_test)
    {
        // Запускаем один из режимов самотестирования.
        simulator_selftest_start(simulator_test.value());
    }

    scpi_server_join();
    ncpi_server_join();
}

// Обрабатывает опцию симулятора. Если опция задана,
// подгружает конфиг из файла в папке simulator-configs
//
// args:
// @ simulator - название файла настроек.
//    если необходимого названия нет в списке, выводит
//    список доступных файлов на печать.
// return:
// @0 путь до файла постоянных настроек
// @1 путь до файла обновляемых настроек
// @2 статус
std::tuple<std::string, std::string, int>
load_simulator(const std::string &simulator, const std::string &directory)
{
    std::set<std::string> simulator_names;

    auto simulator_path = fs::path(simulator_config_path);
    // fs::path(directory) / "simulator-configs";

    for (const auto &entry : fs::directory_iterator(simulator_path))
    {
        auto s = entry.path().string();
        auto l = nos::split(s, '/');
        auto last = l[l.size() - 1];
        auto name = nos::split(last, '-')[0];

        simulator_names.insert(name);
    }

    if (simulator_names.count(simulator))
    {
        auto config = simulator_path / (simulator + "-settings.json");
        auto runtime = simulator_path / (simulator + "-runtime.json");
        return std::tuple<std::string, std::string, int>{config, runtime, 0};
    }

    nos::fprintln("Simulator settings for \"{}\" not found."
                  " Existed configs:",
                  simulator);
    for (auto k : simulator_names)
    {
        nos::fprintln("\t{}", k);
    }

    return std::tuple<std::string, std::string, int>{"", "", -1};
}

void print_help()
{
    printf("Usage: rfmeask [OPTION]...\n"
           "\n"
           "Simulation:\n"
           "  -s, --simulator <PROFILE> - start with preconfigured simulation "
           "config\n"
           "  -q, --simtest <TESTNO> - start simulator selftest number TESTNO\n"
           "Configuration:\n"
           "  -p, --scpiport - port for SCPI server. If setted, configuration "
           "ignore.\n"
           "  -N, --ncpiport - port for NCPI server. If setted, configuration "
           "ignore.\n"
           "  -P, --protoport - port for ProtoBuf server. If setted, "
           "configuration ignore.\n"
           "  -Q, --crowport - port for crow server. If setted, "
           "configuration ignore.\n"
           "  -H, --httport - port for Http server.\n"
           "  -d, --confdir - path to settings and runtime directory\n"
           "  -c, --config - path to settings file\n"
           "  -r, --runtime - path to runtime file\n"
           "  -s, --scriptdir - path of scripts\n"
           "  -L, --logerrors - alarm log path\n"
           "Debug:\n"
           "  -C  --crowdiag - enable crow diagnostic\n"
           "  -n, --nofault - allow work with some fault errors\n"
           "  -T, --immediate-exit, Immediate exit for library linking test\n"
           "  -D, --debug - force debug\n"
           "  -v, --version - get version\n"
           "\n");
}

void create_simulator_configs_directory_if_not_exists()
{
    fs::path path = fs::path(simulator_config_path);

    if (!fs::exists(path))
    {
        nos::log::info("Create simulator config directory");
        fs::create_directories(path);
    }

    auto keys = ircc_keys();

    for (auto &key : keys)
    {
        auto filename = nos::split(key, '/')[1];
        if (nos::split(key, '/')[0] != "simulator-configs")
            continue;

        auto filepath = path / filename;

        if (!fs::exists(filepath) || FORCE_UPDATE_SIMULATOR_SCRIPTS)
        {
            nos::log::info("Create simulator config file {}", filepath);
            std::ofstream file(filepath);
            file << ircc_string(key);
            file.close();
        }
    }
}

void init_inites()
{
    void init_common_axis();
    void init_kollmorgen_servo();
    void init_ralgocnc_igroup();
    void init_ralgocnc_device();
    void init_simulator_servo();
    void init_simulator_syncro();

    init_common_axis();
    init_kollmorgen_servo();
    init_ralgocnc_igroup();
    init_ralgocnc_device();
    init_simulator_servo();
    init_simulator_syncro();
}

/*main*/
int main(int argc, char **argv)
{
    init_inites();

    NOFAULT = false;
    nos::cout.flush_on_write(true);

    // set line buffering
    setvbuf(stdout, NULL, _IOLBF, 0);

    executable_path = argv[0];
    executable_directory = igris::path::dirname(argv[0]);
    std::string simulator_mode = "";

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"debug", no_argument, NULL, 'D'},
        {"nofault", no_argument, NULL, 'n'},
        {"confdir", required_argument, NULL, 'd'},
        {"simulator", required_argument, NULL, 'S'},
        {"updsim", no_argument, NULL, 'm'},
        {"config", required_argument, NULL, 'c'},
        {"scpiport", required_argument, NULL, 'p'},
        {"ncpiport", required_argument, NULL, 'N'},
        {"protoport", required_argument, NULL, 'P'},
        {"httpport", required_argument, NULL, 'H'},
        {"crowport", required_argument, NULL, 'Q'},
        {"runtime", required_argument, NULL, 'r'},
        {"scriptdir", required_argument, NULL, 's'},
        {"crowdiag", no_argument, NULL, 'C'},
        {"console", no_argument, NULL, 'l'},
        {"logerrors", required_argument, NULL, 'L'},
        {"immediate-exit", no_argument, NULL, 'T'},
        {"simtest", required_argument, NULL, 'q'},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}};

    int long_index = 0;
    int opt = 0;

    while ((opt = getopt_long(argc, argv, "", long_options, &long_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);

        case 'c':
            config_path = optarg;
            break;

        case 'r':
            runtime_path = optarg;
            break;

        case 'n':
            NOFAULT = true;
            break;

        case 'l':
            WITH_CONSOLE = true;
            break;

        case 'D':
            FORCE_DEBUG = true;
            break;

        case 'p':
            scpi_port = std::stoi(optarg);
            break;

        case 'N':
            ncpi_port = std::stoi(optarg);
            break;

        case 'P':
            proto_socket_port = std::stoi(optarg);
            break;

        case 'H':
            http_port = std::stoi(optarg);
            break;

        case 'Q':
            crow_port = std::stoi(optarg);
            break;

        case 'S':
            simulator_mode = optarg;
            break;

        case 'm':
            FORCE_UPDATE_SIMULATOR_SCRIPTS = true;
            break;

        case 's':
            set_script_directory(optarg);
            break;

        case 'T':
            nos::println("immediate exit");
            exit(0);

        case 'd':
            confdir_path = optarg;
            break;

        case 'C':
            CROW_DIAG = true;
            break;

        case 'L':
            errFile = optarg;
            break;

        case 'q':
            simulator_test = std::atoi(optarg);
            break;

        case 'v':
            nos::fprintln("{} SCPI_API:{} ({})", VERSION, API_VERSION, "---");
            exit(0);

        default:
            print_help();
            exit(0);
        }
    }
    nos::println("scriptdir:", script_directory());

    create_simulator_configs_directory_if_not_exists();

    if (simulator_test)
    {
        nos::log::info("Simulator Test mode.");
        simulator_mode = simulator_test_profile(simulator_test.value());
    }

#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(WINSOCK_VERSION, &wsaData))
    {
        nos::println("winsock not be initialized!");
        exit(-1);
    }
#endif

    if (CROW_DIAG)
    {
        crow::diagnostic_setup(true);
    }

    devAlarmLogger.init(errFile.c_str());
    Device::setAlarmLogger(&devAlarmLogger);

    if (simulator_mode != "")
    {
        if (scpi_port == -1)
            scpi_port = 5024;

        int status;
        std::tie(config_path, runtime_path, status) =
            load_simulator(simulator_mode, executable_directory);
        if (status == -1)
            exit(0);
    }

    bool config_path_is_directory = fs::is_directory(config_path);

    if (config_path != "" && config_path_is_directory)
    {
        confdir_path = config_path;
    }

    if (confdir_path != "")
    {
        nos::log::info("Config directory: ", config_path);
        std::filesystem::path dir = std::filesystem::path(confdir_path);
        config_path = dir / "settings.json";
        runtime_path = dir / "runtime.json";
    }

    nos::log::info("Load settings from {}, {}", config_path, runtime_path);
    config_load(config_path, runtime_path);
    nos::log::info("Load settings done");

    auto loglvltr = config_settings.node()["loglevel"];
    auto loglevel_str = loglvltr.as_string_default("trace");

    init_logging_system(loglevel_str, FORCE_DEBUG);

    nos::log::info("Environment succesfually inited. Start RFmeask core.");
    nos::log::info("...");
    nos::log::info("...");
    nos::log::info("...");

    rfmeask_start();
}
