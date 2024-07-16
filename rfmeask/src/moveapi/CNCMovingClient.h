#ifndef CNC_MOVING_CLIENT_H
#define CNC_MOVING_CLIENT_H

#include <FullyClosedGear.h>
#include <GearSettings.h>
#include <moveapi/MoveApiClient.h>
#include <RalgoCNC.h>
#include <ralgo/cnc/util.h>
#include <servos/MitsubishiServo.h>

class CommonAxis;
class CNCTandemMovingClient : public MoveApiClient
{
    class Record
    {
    public:
        AbstractServo *servo = nullptr;
        std::string cncax_symbol = {};

        // Множитель может быть использован для задания реверса.
        double multiplier = 1;

        // electronic gear связанного сервоусилителя падает сюда.
        double gear_coeff = 1;
        double feedback_gear_coeff = 1;

    public:
        Record(AbstractServo *servo,
               std::string cncax_symbol,
               double multiplier) :
            servo(servo), cncax_symbol(cncax_symbol), multiplier(multiplier)
        {
        }

        std::string info();
        Record(const Record &) = default;
        Record &operator=(const Record &) = default;
    };

    std::vector<Record> records = {};
    RalgoCNC *cnc_driver = nullptr;
    bool _last_is_tandem_synchronized = false;
    double maxmistake_units = 0.1;

    rxcpp::subjects::behavior<bool> _is_tandem_syncronized =
        rxcpp::subjects::behavior<bool>(false);
    std::vector<rxcpp::subscription> _servo_subscriptions;

    std::vector<size_t> last_positions = {};

private:
    bool _is_position_updated_before_moving = true;

public:
    GearSettings _gear_settings = {};
    FullyClosedGear _fully_closed_gear = {};

    bool use_cnc_feedback = false;

public:
    double maxmistake_pulses()
    {
        return maxmistake_units * native_pulses_per_unit();
    }

    void set_tandem_synchronization_limit(double val)
    {
        maxmistake_units = val;
    }

    GearSettings *gear_settings() override
    {
        return &_gear_settings;
    }

    FullyClosedGear *fully_closed_gear() override
    {
        return &_fully_closed_gear;
    }

    CNCTandemMovingClient(CommonAxis *axis) : MoveApiClient(axis) {}
    CNCTandemMovingClient(const CNCTandemMovingClient &) = default;
    CNCTandemMovingClient &operator=(const CNCTandemMovingClient &) = default;
    std::vector<Device *> get_devices() override;
    void add_axis_record(Record record);
    void set_is_position_updated_before_moving(bool en)
    {
        _is_position_updated_before_moving = en;
        if (_is_position_updated_before_moving == false)
            throw std::runtime_error("not implemented");
    }

    AbstractServo *servo_with_index(size_t index);

    void set_cnc_driver(RalgoCNC *cnc);
    int unsafe_absolute_move_iu(double tgt,
                                PolymorphSpeed speed,
                                PolymorphAccel accel) override;
    int unsafe_relative_move_iu(double dist,
                                PolymorphSpeed speed,
                                PolymorphAccel accel) override;
    int unsafe_absolute_move_units(double dist_units,
                                   PolymorphSpeed speed,
                                   PolymorphAccel accel) override;
    double get_position_iu() override;
    void nullify_position() override;

    std::vector<std::string> symbols();
    std::vector<std::pair<AbstractServo *, std::string>> servos_and_symbols();
    bool all_servos_is_ready();
    std::vector<double> multipliers();
    std::vector<AbstractServo *> servos();
    void init_scale_multipliers();
    void init(CommonAxis *parent) override;
    void preset() override;

    // void operation_finish_callback();
    // void operation_start_callback();
    void set_use_cnc_feedback(bool en);
    void stop() override;

    void notifyCurrentPosition(int32_t arg);
    void clear_position_notification();
    void subscribe_to_servo_position_notification();
    bool check_tandem_synchronization();
    bool is_tandem_synchronized();

    std::string typestr() override
    {
        return "CNCMovingClient";
    }

    std::vector<size_t> servo_indexes();

    void update_cnc_positions();

private:
    AbstractServo const &main_servo() const;
    AbstractServo &main_servo();
    void update_cnc_gear_coeffs();
    bool all_servos_in_fully_closed_mode();
    bool any_servo_in_fully_closed_mode();
    double native_pulses_per_unit() const;
    double native_pulses_per_unit_by_fully_closed() const;
    double native_pulses_per_unit_by_gear() const;

    bool is_ready() override;
    bool is_unready() override;

    void setup_axes_accelvelocities();

protected:
    bool command(const nos::argv &args, nos::ostream &os) override;
    std::string info() override;
    std::string command_help() override;
};

#endif
