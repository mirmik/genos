#ifndef COMMON_AXIS_H
#define COMMON_AXIS_H

#include <AbstractAxis.h>
#include <moveapi/MoveApiClient.h>
#include <rxcpp/rx.hpp>
#include <set>
#include <string>

class CommonAxis : public AbstractAxis
{
public:
    rxcpp::composite_subscription checker_ready_subscription = {};
    // std::unique_ptr<ScanApiClient> scan_client = {};
    std::unique_ptr<MoveApiClient> move_client = {};

public:
    CommonAxis(const std::string &name);

    GearSettings *gear_settings() override
    {
        return move_client->gear_settings();
    }

    FullyClosedGear *fully_closed_gear() override
    {
        return move_client->fully_closed_gear();
    }

    MoveApiClient *move_api()
    {
        return move_client.get();
    }
    ScanApiClient *scan_api()
    {
        return scan_client.get();
    }

    void setSpeed_ups(double arg) final;
    double getSpeed_ups() const final;
    int32_t getSpeed_rpm() const final;
    double getSpeed_upm() const final;
    // int32_t getPosition() final;

    void preset() final;
    void init();

    void maxSpeed_ups(double arg) final;
    void maxAccel_ups2(double arg) final;
    void minAccel_ms(uint32_t arg) final;
    void setAccel_ms(int32_t ms) final
    {
        minAccel_ms(ms);
    }
    void setAccel_ups2(double upss) final;
    double maxAccel_ups2() const final;

    uint32_t minAccel_ms() const final;
    int32_t getAccel_ms() const final;
    double maxSpeed_ups() const final;
    void defaultSpeed_ups(double arg) final;
    void defaultAccel_ups2(double arg) final;
    double defaultSpeed_ups() const final;
    double defaultAccel_ups2() const final;
    void set_speed_procent(double proc) final;

    void setSpeed_rpm(int32_t rpm) final;
    uint32_t maxSpeed_rpm() const final;
    uint32_t defaultSpeed_rpm() const final;
    void defaultSpeed_rpm(uint32_t) final;
    void maxSpeed_rpm(uint32_t) final;
    uint32_t defaultAccel_ms() const final;

    double getAccel_ups2() const final;

    int prepare_move_operation();

    // void relative_move_impl(double arg, bool lock_mode);
    // void relativeUnitMove_impl(double arg, bool lock_mode);
    // void absolute_move_impl(double arg, bool lock_mode);
    void absoluteUnitMove_impl(double arg, bool lock_mode) final;

    void limits_update() final;
    void on_ready_status_changed(bool ready) override;

    double unitRatio() const final
    {
        assert(move_client->gain() != 0);
        return move_client->gain();
    }

    void setNullPosition() final
    {
        move_client->nullify_position();
    }

    // double getUnitPosition() final;

    void stop_impl() final;

    void compare_table_enable(bool en) final
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void compare_table_add_point(int32_t pos) final
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void compare_table_clear() final
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void scan_subsystem_reset() final
    {
        FUNCTION_IS_NOT_RELEASED
    }

    void command(const nos::argv &args, nos::ostream &out) override;
    std::string info() override;
    std::string command_help() override;

    std::string clients_types_onestring() const
    {
        std::string move_client_str =
            move_client ? move_client->typestr() : "none";
        std::string scan_client_str =
            scan_client ? scan_client->typestr() : "none";
        return nos::format("{},{}", move_client_str, scan_client_str);
    }

    std::string declared_type() const override
    {
        return nos::format("CommonAxis[{}]", clients_types_onestring());
    }

    std::vector<double>
    scan_points_position(int offset, int size, bool from_end) override;
};

#endif