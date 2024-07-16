#ifndef AXIS_H
#define AXIS_H

/**
 @file
    Данный класс является интерфейсом простой оси, не поддерживающей режим
   сканирования. Может работать с разными типами сервоусилителей через интерфейс
   AbstractServo. Реализует логику для настройки defaultSettings и других
   абстрактных характеристик не зависящих от типа СУ.
*/

#include <AbstractAxis.h>
#include <FullyClosedGear.h>
#include <GearSettings.h>

class AbstractServo;

class Axis : public AbstractAxis
{
public:
    AbstractServo *mover = {};
    uint8_t last_status = {};
    int opstatus_changer_count = 0;
    uint32_t m_defaultSpeed_rpm = {};
    uint32_t m_defaultAccel_ms = {};
    uint32_t m_min_accel_ms = {};
    bool gearSettingsMode = false;
    GearSettings gearSettings = {};
    FullyClosedGear fullyClosedGear = {};
    rxcpp::composite_subscription mover_status_subscription = {};
    bool m_needread_load_gain = false;
    double m_loadGain = 1;

    std::chrono::system_clock::time_point last_operation_start_time;

public:
    Axis(const Axis &) = delete;
    Axis &operator=(const Axis &) = delete;
    Axis(const std::string &name, AbstractServo *_mover);
    Axis(const std::string &name,
         AbstractServo *_mover,
         std::vector<Device *> vect);

    long milliseconds_from_last_operation_start();

    GearSettings *gear_settings() override
    {
        return &gearSettings;
    }

    double units_to_pulses(double dist) const;
    double pulses_to_units(double dist) const;

    FullyClosedGear *fully_closed_gear() override
    {
        return &fullyClosedGear;
    }

    void on_ready_status_changed(bool ready) override;

    uint32_t minAccel_ms() const override
    {
        return m_min_accel_ms;
    }
    uint32_t defaultSpeed_rpm() const override
    {
        return m_defaultSpeed_rpm;
    }
    uint32_t defaultAccel_ms() const override
    {
        return m_defaultAccel_ms;
    }

    void maxSpeed_ups(double arg) override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void maxAccel_ups2(double arg) override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    double maxSpeed_ups() const override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    double maxAccel_ups2() const override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void defaultSpeed_ups(double arg) override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void defaultAccel_ups2(double arg) override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    double defaultSpeed_ups() const override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    double defaultAccel_ups2() const override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void setSpeed_ups(double arg) override
    {
        return setSpeed_unitPerSecond(arg);
    }
    void setAccel_ups2(double arg) override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    double getSpeed_ups() const override
    {
        return getSpeed_unitPerSecond();
    }

    uint32_t maxSpeed_rpm() const override;
    void maxSpeed_rpm(uint32_t maxspd) override;

    int64_t backwardLimit()
    {
        return unitBackwardLimit() * unitRatio();
    }
    int64_t forwardLimit()
    {
        return unitForwardLimit() * unitRatio();
    }
    int64_t backwardLimit() const
    {
        return unitBackwardLimit() * unitRatio();
    }
    int64_t forwardLimit() const
    {
        return unitForwardLimit() * unitRatio();
    }
    void backwardLimit(int64_t dist)
    {
        unitBackwardLimit((double)dist / unitRatio());
    }
    void forwardLimit(int64_t dist)
    {
        unitForwardLimit((double)dist / unitRatio());
    }

    double unitRatio() const override;
    double unitRatio_by_fully_closed() const;
    double unitRatio_by_gear() const;

    uint32_t protect_accel(uint32_t val);
    void protect_speed_exception(uint32_t val);
    void protect_accel_exception(uint32_t val);
    virtual void onReadyHandler() override;
    virtual void onMoverReady(Device *) override;
    virtual void onCriticalErrorHandler(Device *dev) override;
    virtual void preset() override;
    virtual void generate_trigger() override;
    virtual void manual_trigger_mode(int32_t en) override;
    virtual void no_trigger_mode(int32_t en) override
    {
        throw FunctionIsNotSupported();
        ;
    }
    virtual void bitrigmode(int32_t en) override
    {
        throw FunctionIsNotSupported();
        ;
    }

    virtual void stopLastOperation(StopType arg) override;
    // void relative_move_impl(double arg, bool ctrl);
    // void relativeUnitMove_impl(double arg, bool ctrl);
    void absolute_move_impl(double arg, bool ctrl);
    void absoluteUnitMove_impl(double arg, bool ctrl) override;
    void absoluteUnitMove_over_unit(double arg, bool ctrl);
    void absoluteUnitMove_over_pulse(double arg, bool ctrl);
    bool servo_absolute_moving_compat();

    void setSpeed_rpm(int32_t arg) override;
    void setSpeed_unitPerSecond(double arg);
    void setSpeed_unitPerMinute(double arg);
    void setAccel_ms(int32_t arg) override;
    int32_t getSpeed_rpm() const override;
    double getSpeed_unitPerSecond() const;
    double getSpeed_unitPerMinute() const;
    int32_t getAccel_ms() const override;
    int getLimitSwitchStatus() override;
    void setNullPosition() override;
    void stop_impl() override;
    bool haveScan() override;
    bool haveAbsoluteMove() override;
    bool haveSetReference() override;
    bool hasInterpolation() override;

    // Scaner API
    void scanMove(int32_t arg) override;
    void scanUnitMove(double arg) override;
    void scanForwardZone(int32_t arg) override;
    void scanUnitForwardZone(double arg) override;
    void scanBackwardZone(int32_t arg) override;
    void scanUnitBackwardZone(double arg) override;
    void scanStart() override;
    void scanCompareStart() override;
    void scanPoints(int32_t arg) override;

    int32_t scanMove() override;
    double scanUnitMove() override;
    int32_t scanForwardZone() override;
    double scanUnitForwardZone() override;
    int32_t scanBackwardZone() override;
    double scanUnitBackwardZone() override;
    int32_t scanPoints() override;
    int32_t trigreturn_time() override
    {
        throw FunctionIsNotSupported();
    }

    virtual int32_t synchroSpeedTest() override;

    void servo_on() override;
    void servo_off() override;

    int32_t clamp_to_limits(int32_t tgt);
    bool inlimits(int32_t tgt);

    virtual void operationStatusSlot(uint8_t arg);

    void set_speed_procent(double proc) override
    {
        setSpeed_rpm(maxSpeed_rpm() * (abs(proc) / 100.0));
    }

    double getAccel_ups2() const override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    // double getAccel_ups2() override {FUNCTION_IS_NOT_RELEASED}

    double getSpeed_upm() const override
    {
        FUNCTION_IS_NOT_RELEASED
    }
    void defaultSpeed_rpm(uint32_t) final
    {
        FUNCTION_IS_NOT_RELEASED
    }

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
    void scan_subsystem_reset() final{FUNCTION_IS_NOT_RELEASED}

    std::string declared_type() const override
    {
        return "simple_axis";
    }

    void command(const nos::argv &args, nos::ostream &out) override;
    std::string command_help() override;
    std::string info();

    void notifyCurrentPosition(int32_t arg);
    void notifyCurrentPositionUnit(double arg);

    std::vector<double>
    scan_points_position(int offset, int size, bool from_end) override
    {
        return std::vector<double>();
    }

private:
    void operationCorrectStart(bool ctrl = true);
    void operationCorrectFinish();

protected:
    bool scanStop = true;
    bool lastPoint = true;
};

#endif
