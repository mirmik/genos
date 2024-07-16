#ifndef INC_RALGO_CNC_CONTROLLER_H
#define INC_RALGO_CNC_CONTROLLER_H

#include <chrono>
#include <crow/hostaddr.h>
#include <crow/nodes/requestor_node.h>
#include <crow/nodes/subscriber_node.h>
#include <devices/Device.h>
#include <igris/container/sline.h>
#include <mutex>
#include <ndmath/cartesian_correction.h>
#include <nos/io/serial_port.h>
#include <optional>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector.h>

class RalgoCNC : public Device
{
    static constexpr int NMAX_AXES = 12;
    // std::optional<ndmath::cartesian_correction> _correction = {};
    int64_t _firmware_version = -1;

    std::mutex lock = {};
    std::mutex lock_telemetry = {};

    nos::serial_port *serial = nullptr;
    std::thread listen_thread = {};
    std::mutex answer_mtx = {};
    std::condition_variable answer_cv = {};
    std::string answer = {};

    igris::sline sl;

    // crow::hostaddr crowker_address = {};
    // crow::requestor_node requestor = {};
    // crow::subscriber_node position_subscriber = {};
    // crow::subscriber_node feedback_position_subscriber = {};
    // crow::subscriber_node opstat_subscriber = {};
    // crow::subscriber_node logging_subscriber = {};
    // std::string crowker_theme_control = {};
    // std::string crowker_theme_telemetry = {};
    // std::string crowker_theme_feedback_telemetry = {};
    // std::string crowker_theme_logging = {};
    // std::string crowker_theme_opstat = {};

    std::chrono::system_clock::time_point start_operation_time;

    bool no_one_message = true;
    std::chrono::time_point<std::chrono::system_clock> last_resubscribe_time =
        {};
    std::chrono::time_point<std::chrono::system_clock> last_telemetry_time = {};
    std::array<int64_t, NMAX_AXES> feedback_steps = {};
    std::array<int64_t, NMAX_AXES> feedback_feedback_steps = {};

public:
    igris::multiple_delegate<> stop_operation_callback = {};
    igris::multiple_delegate<> start_operation_callback = {};

public:
    RalgoCNC(const std::string &name);

    std::string declared_type() const override
    {
        return "ralgo_cnc";
    }

    void set_pause_mode(bool en);

    void listen_thread_func();

    void set_max_velocities(const std::vector<std::pair<std::string, double>>
                                &symbols_maxvelocities);

    void set_max_accelerations(const std::vector<std::pair<std::string, double>>
                                   &symbols_maxaccelerations);

    void set_crowker_address(crow::hostaddr crowker_address,
                             const std::string &prefix);
    void updaterThreadFunc() override;
    void preset() override;
    void onReadyHandler(Device *dev);

    void listen_thread_func_execute();
    void resubscribe();
    void disable_global_protection();
    int64_t get_feedback_steps_position(std::string symbol);
    ralgo::vector<double> current_position();
    int unsafe_relative_move_iu(const std::vector<std::string> &symbols,
                                const std::vector<double> &multes,
                                double dist,
                                double velocity_accel_iu_per_sec,
                                double accel_iu_per_sqrsec);

    int unsafe_absolute_move_iu(const std::vector<std::string> &symbols,
                                const std::vector<double> &multes,
                                double dist,
                                double velocity_accel_iu_per_sec,
                                double accel_iu_per_sqrsec);

    int unsafe_absolute_move_iu_vec(const std::vector<std::string> &symbols,
                                    const std::vector<double> &dists,
                                    double velocity_accel_iu_per_sec,
                                    double accel_iu_per_sqrsec);

    int absolute_move(const std::vector<std::string> &symbols,
                      const std::vector<double> &dists,
                      double velocity_accel_iu_per_sec,
                      double accel_iu_per_sqrsec,
                      bool diagonal_accvel_multiplication = true);

    int absolute_move(const std::vector<std::string> &symbols,
                      const std::vector<double> &dists);

    void set_axis_position(const std::string &symbol,
                           double reference_position);

    int absmove(const std::vector<ndmath::vector> &pnts,
                const std::vector<std::string> &symbols);

    void declare_symbols_as_tandem(const std::vector<std::string> &tandem,
                                   const std::vector<double> muls,
                                   double maxmistake);

    std::string poses_command_string(const std::vector<std::string> &symbols,
                                     const std::vector<double> &dist);

    std::string poses_command_string(const std::vector<std::string> &symbols,
                                     const ralgo::vector<double> &dist);

    std::string poses_command_string(const std::vector<std::string> &symbols,
                                     double dist,
                                     const std::vector<double> &multes);

    std::string
    poses_command_string_vec(const std::vector<std::string> &symbols,
                             const std::vector<double> &dist);

    void send_stop_command();
    void stop();

    void set_stepper_gear(const std::string &symbol, double gear);
    void set_stepper_feedback_gear(const std::string &symbol, double gear);
    std::string command_send(const std::string &str, bool wait_answer = false);
    int32_t command_send_ans_int32(const std::string &str);
    double command_send_ans_double(const std::string &str);

private:
    void position_subscribe_handle(nos::buffer msg);
    void feedback_position_subscribe_handle(nos::buffer msg);
    void opstat_subscribe_handle(nos::buffer msg);
    void logging_subscribe_handle(nos::buffer msg);

    void init_requestor();
    void init_telemetry_subscriber();
    void init_feedback_telemetry_subscriber();
    void init_logging_subscriber();
    void init_opstat_subscriber();

    std::string info();
    void command(const nos::argv &args, nos::ostream &os) override;
    std::string command_help() override;

    void pulse_handler();
    void stop_handler();
    void start_handler();
    void answer_handler(const std::string &str);
};

#endif
