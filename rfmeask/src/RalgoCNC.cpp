#include <RalgoCNC.h>
#include <crow/address.h>
#include <crow/nodes/pubsub_defs.h>
#include <igris/string/random_string.h>
#include <igris/util/string.h>
#include <logging.h>
#include <nos/to_string.h>
#include <ralgo/cnc/themes.h>
#include <ralgo/cnc/util.h>
#include <ralgo/linalg/vecops.h>

using namespace std::chrono_literals;
using namespace nos::argument_literal;

RalgoCNC::RalgoCNC(const std::string &name) : Device(name), sl(512)
{
    device_status_subject.get_observable().subscribe([&](const auto &pair) {
        auto [status, dev] = pair;
        if (status == DeviceStatus::Ready)
            this->onReadyHandler(dev);
    });

    for (int i = 0; i < NMAX_AXES; i++)
    {
        feedback_steps[i] = 0;
        feedback_feedback_steps[i] = 0;
    }

    serial = new nos::serial_port("/dev/ttyS0", 115200, 'N', 8, 1);
    listen_thread = std::thread([this]() { listen_thread_func(); });
    sl.reset();
}

void RalgoCNC::listen_thread_func_execute()
{
    const char *s = sl.getline();
    size_t sz = sl.current_size();

    std::string str(s, sz);
    if (str == "pulse\r\n")
    {
        pulse_handler();
    }
    else if (str == "stop\r\n")
    {
        stop_handler();
    }
    else if (str == "start\r\n")
    {
        start_handler();
    }
    else
    {
        answer_handler(str);
    }
    sl.reset();
}

void RalgoCNC::listen_thread_func()
{
    while (1)
    {
        char buf[256];
        int sz = *serial->read(buf, 256);

        if (sz == 0 || sz == -1)
            return;

        for (int i = 0; i < sz; ++i)
        {
            char c = buf[i];
            sl.newdata(c);
            if (c == '\n')
                listen_thread_func_execute();
        }
    }
}

void RalgoCNC::answer_handler(const std::string &str)
{
    nos::println("Answer:", str);
    std::lock_guard<std::mutex> guard(answer_mtx);
    answer = str;
    answer_cv.notify_all();
}

void RalgoCNC::onReadyHandler(Device *dev) {}

void RalgoCNC::updaterThreadFunc()
{
    while (1)
    {
        auto curtime = std::chrono::system_clock::now();

        if (!no_one_message && curtime - last_telemetry_time > 2000ms)
        {
            nos::log::warn("RalgoCNC driver refused: timeout");
            transactionRefuseError();
        }

        std::this_thread::sleep_for(10ms);
    }
}

void RalgoCNC::preset()
{
    nos::println("Preset:", name());

    std::lock_guard<std::mutex> guard(lock);
    std::string cmd = "cmd setprotect 0";
    command_send(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string cmd2 = "cmd stop";
    command_send(cmd2);
    nos::println("VERSION:!!!!!!!!!!!!!!!!!!!");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto version = command_send("api_version", true);
    nos::println("VERSION:!!!!!!!!!!!!!!!!!!!", version);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    _firmware_version = std::stoi(version);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

/** Формирует комманду на перемещение по массиву точек */
int RalgoCNC::absmove(const std::vector<ndmath::vector> &pnts,
                      const std::vector<std::string> &symbols)
{
    std::lock_guard<std::mutex> guard(lock);
    for (auto &p : pnts)
    {
        std::string poses_str = poses_command_string(symbols, p);
        auto cmd = nos::format("cnc G01 {poses}", "poses"_a = poses_str);
        command_send(cmd);
    }
    return 0;
}

std::string RalgoCNC::command_send(const std::string &str, bool wait_answer)
{
    serial->write(str.data(), str.size());
    serial->write("\r\n", 2);
    nos::println("Command:", str);

    if (!wait_answer)
        return "";

    // wait cv
    std::unique_lock<std::mutex> lock(answer_mtx);
    nos::println("Wait answer");
    answer_cv.wait(lock);
    nos::println("Answer received");

    std::string ans;
    {
        // std::lock_guard<std::mutex> guard(answer_mtx);
        ans = answer;
    }

    lock.unlock();
    return ans;
}

double RalgoCNC::command_send_ans_double(const std::string &str)
{
    auto ans = command_send(str);
    return std::stod(ans);
}

int32_t RalgoCNC::command_send_ans_int32(const std::string &str)
{
    auto ans = command_send(str);
    return std::stod(ans);
}

int RalgoCNC::unsafe_relative_move_iu(const std::vector<std::string> &symbols,
                                      const std::vector<double> &multes,
                                      double dist,
                                      double velocity_accel_iu_per_sec,
                                      double accel_iu_per_sqrsec)
{
    std::lock_guard<std::mutex> guard(lock);
    double diagweight = sqrt(double(symbols.size()));
    std::string poses_str = poses_command_string(symbols, dist, multes);
    std::string speed_str =
        nos::format("F{}", velocity_accel_iu_per_sec * diagweight);
    std::string accel_str =
        nos::format("M{}", accel_iu_per_sqrsec * diagweight);

    auto cmd = nos::format("cmd relmove {poses} {speed} {accel}",
                           "poses"_a = poses_str,
                           "speed"_a = speed_str,
                           "accel"_a = accel_str);
    start_operation_time = std::chrono::system_clock::now();
    command_send(cmd);
    return 0;
}

int RalgoCNC::unsafe_absolute_move_iu_vec(
    const std::vector<std::string> &symbols,
    const std::vector<double> &dists,
    double velocity_accel_iu_per_sec,
    double accel_iu_per_sqrsec)
{
    std::lock_guard<std::mutex> guard(lock);
    double diagweight = sqrt(double(symbols.size()));
    std::string poses_str = poses_command_string_vec(symbols, dists);
    std::string speed_str =
        nos::format("F{}", velocity_accel_iu_per_sec * diagweight);
    std::string accel_str =
        nos::format("M{}", accel_iu_per_sqrsec * diagweight);

    auto cmd = nos::format("cmd absmove {poses} {speed} {accel}",
                           "poses"_a = poses_str,
                           "speed"_a = speed_str,
                           "accel"_a = accel_str);
    start_operation_time = std::chrono::system_clock::now();
    command_send(cmd);
    return 0;
}

int RalgoCNC::absolute_move(const std::vector<std::string> &symbols,
                            const std::vector<double> &dists,
                            double velocity_accel_iu_per_sec,
                            double accel_iu_per_sqrsec,
                            bool diagonal_accvel_multiplication)
{
    std::lock_guard<std::mutex> guard(lock);
    std::string poses_str = poses_command_string(symbols, dists);

    double diagweight =
        diagonal_accvel_multiplication ? sqrt(double(symbols.size())) : 1.0;
    std::string speed_str =
        nos::format("F{}", velocity_accel_iu_per_sec * diagweight);
    std::string accel_str =
        nos::format("M{}", accel_iu_per_sqrsec * diagweight);

    auto cmd = nos::format("cmd absmove {poses} {speed} {accel}",
                           "poses"_a = poses_str,
                           "speed"_a = speed_str,
                           "accel"_a = accel_str);
    start_operation_time = std::chrono::system_clock::now();
    command_send(cmd);
    return 0;
}

int RalgoCNC::absolute_move(const std::vector<std::string> &symbols,
                            const std::vector<double> &dists)
{
    std::lock_guard<std::mutex> guard(lock);
    std::string poses_str = poses_command_string(symbols, dists);

    auto cmd = nos::format("cmd absmove {poses}", "poses"_a = poses_str);
    start_operation_time = std::chrono::system_clock::now();
    command_send(cmd);
    return 0;
}

void RalgoCNC::set_pause_mode(bool en)
{
    command_send(nos::format("cmd planner_pause {}", en ? 1 : 0));
}

int RalgoCNC::unsafe_absolute_move_iu(const std::vector<std::string> &symbols,
                                      const std::vector<double> &multes,
                                      double dist,
                                      double velocity_accel_iu_per_sec,
                                      double accel_iu_per_sqrsec)
{
    assert(!isnan(dist));

    for (auto m : multes)
        assert(!isnan(m));

    std::lock_guard<std::mutex> guard(lock);
    nos::println("unsafe_absolute_move_iu",
                 symbols,
                 multes,
                 dist,
                 velocity_accel_iu_per_sec,
                 accel_iu_per_sqrsec);
    double diagweight = sqrt(double(symbols.size()));
    std::string poses_str = poses_command_string(symbols, dist, multes);
    std::string speed_str =
        nos::format("F{}", velocity_accel_iu_per_sec * diagweight);
    std::string accel_str =
        nos::format("M{}", accel_iu_per_sqrsec * diagweight);
    auto cmd = nos::format("cmd absmove {poses} {speed} {accel}",
                           "poses"_a = poses_str,
                           "speed"_a = speed_str,
                           "accel"_a = accel_str);
    start_operation_time = std::chrono::system_clock::now();
    command_send(cmd);
    return 0;
}

std::string
RalgoCNC::poses_command_string(const std::vector<std::string> &symbols,
                               double dist,
                               const std::vector<double> &multes)
{
    std::vector<double> dists = ralgo::vecops::mul_vs(multes, dist);
    return poses_command_string(symbols, dists);
}

std::string
RalgoCNC::poses_command_string_vec(const std::vector<std::string> &symbols,
                                   const std::vector<double> &dists)
{
    return poses_command_string(symbols, dists);
}

std::string
RalgoCNC::poses_command_string(const std::vector<std::string> &symbols,
                               const ralgo::vector<double> &dist)
{
    std::vector<double> dist_vec;
    std::copy(dist.begin(), dist.end(), std::back_inserter(dist_vec));
    return poses_command_string(symbols, dist_vec);
}

std::string
RalgoCNC::poses_command_string(const std::vector<std::string> &symbols,
                               const std::vector<double> &dist)
{
    std::string cmd;
    for (size_t i = 0; i < symbols.size(); ++i)
    {
        assert(!isnan(dist[i]));
        cmd += nos::format("{}{} ", symbols[i], dist[i]);
    }
    cmd.resize(cmd.size() - 1);
    return cmd;
}

void RalgoCNC::pulse_handler()
{
    std::lock_guard<std::mutex> guard(lock_telemetry);
    no_one_message = false;
    last_telemetry_time = std::chrono::system_clock::now();
    transactionReady();
}

void RalgoCNC::stop_handler()
{
    auto curtime = std::chrono::system_clock::now();
    auto dur = curtime - start_operation_time;
    auto dur_sec =
        std::chrono::duration_cast<std::chrono::milliseconds>(dur) / 1000.0;
    nos::println("CNC: STOP SIGNAL: time for operation:", dur_sec.count());
    stop_operation_callback.invoke();
}

void RalgoCNC::start_handler()
{
    nos::println("CNC: START SIGNAL");
    start_operation_callback.invoke();
}

void RalgoCNC::set_crowker_address(crow::hostaddr crowker_address,
                                   const std::string &prefix)
{
    //     this->crowker_address = crowker_address;
    //     crowker_theme_control = prefix + RALGO_CNC_CLI_SERVICE;
    //     crowker_theme_logging = prefix + RALGO_CNC_LOG_THEME;
    //     crowker_theme_opstat = prefix + RALGO_CNC_OPSTAT_THEME;
    //     crowker_theme_telemetry = prefix + RALGO_CNC_POSES_BIN_THEME;
    //     crowker_theme_feedback_telemetry =
    //         prefix + RALGO_CNC_FEEDBACK_POSES_BIN_THEME;
    //     init_requestor();
    //     init_telemetry_subscriber();
    //     init_feedback_telemetry_subscriber();
    //     init_opstat_subscriber();
    //     init_logging_subscriber();
}

void RalgoCNC::opstat_subscribe_handle(nos::buffer buf)
{
    // auto str = std::string{buf.data(), buf.size()};
    // if (nos::trim(str) == "stop")
    // {
    //     auto curtime = std::chrono::system_clock::now();
    //     auto dur = curtime - start_operation_time;
    //     auto dur_sec =
    //         std::chrono::duration_cast<std::chrono::milliseconds>(dur) /
    //         1000.0;
    //     nos::println("CNC: STOP SIGNAL: time for operation:",
    //     dur_sec.count()); stop_operation_callback.invoke();
    // }
}

// void RalgoCNC::init_requestor()
// {
//     requestor.bind();
//     requestor.set_address(crowker_address);
//     requestor.set_qos(0, 50);
//     requestor.set_rqos(0, 50);
//     requestor.set_theme(crowker_theme_control);
//     requestor.set_reply_theme(std::string(crowker_theme_control) +
//                               igris::random_string(8));
// }

// void RalgoCNC::init_telemetry_subscriber()
// {
//     position_subscriber.bind();
//     position_subscriber.set_theme(crowker_theme_telemetry);
//     position_subscriber.set_address(crowker_address);
//     position_subscriber.set_handle(
//         [this](auto data) { position_subscribe_handle(data); });
//     position_subscriber.set_qos(0, 100);
//     position_subscriber.install_keepalive(2000, true);
// }

// void RalgoCNC::init_feedback_telemetry_subscriber()
// {
//     feedback_position_subscriber.bind();
//     feedback_position_subscriber.set_theme(crowker_theme_feedback_telemetry);
//     feedback_position_subscriber.set_address(crowker_address);
//     feedback_position_subscriber.set_handle(
//         [this](auto data) { feedback_position_subscribe_handle(data); });
//     feedback_position_subscriber.set_qos(0, 100);
//     feedback_position_subscriber.install_keepalive(2000, true);
// }

// void RalgoCNC::init_opstat_subscriber()
// {
//     opstat_subscriber.bind();
//     opstat_subscriber.set_theme(crowker_theme_opstat);
//     opstat_subscriber.set_address(crowker_address);
//     opstat_subscriber.set_handle(
//         [this](auto data) { opstat_subscribe_handle(data); });
//     opstat_subscriber.set_qos(0, 100);
//     opstat_subscriber.install_keepalive(2000, true);
// }

// void RalgoCNC::init_logging_subscriber()
// {
//     logging_subscriber.bind();
//     logging_subscriber.set_theme(crowker_theme_logging);
//     logging_subscriber.set_address(crowker_address);
//     logging_subscriber.set_handle(
//         [this](auto data) { logging_subscribe_handle(data); });
//     logging_subscriber.set_qos(0, 100);
//     logging_subscriber.install_keepalive(2000, true);
// }

void RalgoCNC::logging_subscribe_handle(nos::buffer msg)
{
    std::string msgstr = {msg.data(), msg.size()};
    nos::fprintln("[CNC.Log] {}", msgstr);
}

void RalgoCNC::set_stepper_gear(const std::string &symbol, double gear)
{
    assert(!isinf(gear));
    std::lock_guard<std::mutex> guard(lock);
    command_send(nos::format("cmd setgear {} {}", symbol, gear));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void RalgoCNC::set_stepper_feedback_gear(const std::string &symbol, double gear)
{
    assert(!isinf(gear));
    std::lock_guard<std::mutex> guard(lock);
    command_send(nos::format("cmd set_feedback_gear {} {}", symbol, gear));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int64_t RalgoCNC::get_feedback_steps_position(std::string symbol)
{
    std::lock_guard<std::mutex> guard(lock_telemetry);
    int index = cnc::symbol_to_index(symbol.c_str()[0]);
    return feedback_steps[index];
}

void RalgoCNC::send_stop_command()
{
    std::lock_guard<std::mutex> guard(lock);
    command_send("cmd stop");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void RalgoCNC::set_axis_position(const std::string &symbol,
                                 double reference_position)
{
    std::lock_guard<std::mutex> guard(lock);
    command_send(nos::format("cmd setpos {} {}", symbol, reference_position));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void RalgoCNC::command(const nos::argv &args, nos::ostream &os)
{
    if (args.size() == 0)
    {
        nos::println_to(os, command_help());
        return;
    }

    std::string cmd = args[0];

    if (cmd == "send")
    {
        std::string s;
        for (size_t i = 1; i < args.size(); ++i)
            s += std::string(args[i]) + " ";
        command_send(s);
        return;
    }

    else if (cmd == "info")
    {
        nos::println_to(os, info());
        return;
    }

    Device::command(args, os);
}

std::string RalgoCNC::info()
{
    std::string control_steps = nos::format("steps: {} {}\r\n"
                                            "feedback: {} {}\r\n",
                                            feedback_steps[0],
                                            feedback_steps[1],
                                            feedback_feedback_steps[0],
                                            feedback_feedback_steps[1]);
    return control_steps;
}

std::string RalgoCNC::command_help()
{
    std::string ret = Device::command_help();
    ret += "cnc send [command] - send command to cnc\n";
    return ret;
}

void RalgoCNC::stop()
{
    send_stop_command();
}

void RalgoCNC::set_max_velocities(
    const std::vector<std::pair<std::string, double>> &symbols_maxvelocities)
{
    std::lock_guard<std::mutex> guard(lock);
    std::string str = "";
    for (auto &p : symbols_maxvelocities)
    {
        str += nos::format("{}:{} ", p.first, p.second);
    }
    std::string cmd = nos::format("cmd velmaxs {}", str);
    command_send(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void RalgoCNC::set_max_accelerations(
    const std::vector<std::pair<std::string, double>> &symbols_maxaccelerations)
{
    std::lock_guard<std::mutex> guard(lock);
    std::string str = "";
    for (auto &p : symbols_maxaccelerations)
    {
        str += nos::format("{}:{} ", p.first, p.second);
    }
    std::string cmd = nos::format("cmd accmaxs {}", str);
    command_send(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void RalgoCNC::declare_symbols_as_tandem(const std::vector<std::string> &tandem,
                                         const std::vector<double> muls,
                                         double maxmistake)
{
    if (_firmware_version < 210)
        return;

    std::lock_guard<std::mutex> guard(lock);
    std::string str = "cmd enable_tandem_protection";
    for (size_t i = 0; i < tandem.size(); ++i)
    {
        str += nos::format(" {}:{}", tandem[i], muls[i]);
    }

    if (_firmware_version >= 212)
        str += nos::format(" maxmistake:{}", maxmistake);

    command_send(str);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
