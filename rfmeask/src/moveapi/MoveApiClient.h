#ifndef MOVING_API_CLIENT_H
#define MOVING_API_CLIENT_H

#include <GearSettings.h>
#include <PolymorphAccel.h>
#include <PolymorphSpeed.h>
#include <ScaleProtector.h>
#include <devices/Device.h>
#include <igris/util/bug.h>
#include <nos/io/ostream.h>
#include <nos/shell/argv.h>
#include <ralgo/util/clamper.h>
#include <ralgo/util/scaller.h>
#include <set>

class CommonAxis;
/**
 * О единицах измерения:
 * External - unit. величина в которой клиенты осуществляют командное
 * управление. Internal - Величина для внутреннего пользования. Уровень
 * MoveApiClient не может её знать. Он определяет поля и логику работы. А за
 * конкретные преоб разования отвечают наследникики.
 * */
class MoveApiClient
{
    ralgo::clamper<double> extclamp = {};
    ralgo::clamper<double> intclamp = {};

protected:
    CommonAxis *_axis;
    ralgo::scaller<double> scaller = {};

    PolymorphSpeed _default_speed = {};
    PolymorphAccel _default_accel = {};
    PolymorphSpeed _maximum_speed = {};
    PolymorphAccel _maximum_accel = {};
    PolymorphSpeed _current_speed = {};
    PolymorphAccel _current_accel = {};

    std::vector<PolymorphSpeed *> all_speeds = {
        &_default_speed, &_current_speed, &_maximum_speed};
    std::vector<PolymorphAccel *> all_accels = {
        &_default_accel, &_current_accel, &_maximum_accel};

public:
    virtual GearSettings *gear_settings() = 0;
    virtual FullyClosedGear *fully_closed_gear() = 0;

    MoveApiClient(CommonAxis *axis) : _axis(axis) {}
    MoveApiClient() = default;

    void set_common_axis(CommonAxis *axis)
    {
        _axis = axis;
    }

    void operation_finish_callback();
    void operation_start_callback();

    virtual std::string typestr()
    {
        return "undefined moveclient";
    }

    PolymorphSpeed current_speed()
    {
        return _current_speed;
    }

    PolymorphAccel current_accel()
    {
        return _current_accel;
    }

    double gain()
    {
        assert(scaller.gain() != 0);
        return scaller.gain();
    }

    bool limits_inited()
    {
        return extclamp.inited() && intclamp.inited();
    }

    virtual std::vector<Device *> get_devices() = 0;

    virtual void preset();
    virtual void init(CommonAxis *parent);

    double current_external_position()
    {
        if (!multipliers_inited())
            throw std::runtime_error("multipliers not inited");
        return scaller.int2ext(get_position_iu());
    }

    double current_internal_position()
    {
        return get_position_iu();
    }

    bool multipliers_inited()
    {
        if (!scaller.inited() || !_current_speed.inited() ||
            !_default_speed.inited() || !_maximum_speed.inited() ||
            !_current_accel.inited() || !_default_accel.inited() ||
            !_maximum_accel.inited())
            return false;
        return true;
    }

    bool unready_for_moving(bool send_warning)
    {
        bool unready = !multipliers_inited() || !limits_inited();
        if (unready && send_warning)
            nos::log::warn("Moving prevented because a limits is not inited. "
                           "multipliers:{} limits:{}",
                           multipliers_inited(),
                           limits_inited());
        return unready;
    }

    virtual int relative_external_move(double extdist)
    {
        if (unready_for_moving(true))
            return 0;

        double current_extpos = current_external_position();
        double exttgt = current_extpos + extdist;
        double protected_exttgt = extclamp(exttgt);
        double protected_extdist = protected_exttgt - current_extpos;
        double protected_intdist = scaller.ext2int(protected_extdist);
        return unsafe_relative_move_iu(
            protected_intdist, _current_speed, _current_accel);
    };

    virtual int relative_internal_move(double intdist)
    {
        if (unready_for_moving(true))
            return 0;

        double current_intpos = current_internal_position();
        double inttgt = current_intpos + intdist;
        double protected_inttgt = intclamp(inttgt);
        double protected_intdist = protected_inttgt - current_intpos;
        return unsafe_relative_move_iu(
            protected_intdist, _current_speed, _current_accel);
    };

    bool is_reversed();

    virtual int absolute_external_move(double exttgt);
    virtual int absolute_internal_move(double inttgt)
    {
        // if (unready_for_moving(true))
        //     return 0;

        // double protected_inttgt = intclamp(inttgt);
        // return unsafe_absolute_move_iu(
        //     protected_inttgt, _current_speed, _current_accel);
        abort();
    }

    void set_limits_external(double ext_lo, double ext_hi)
    {
        if (!multipliers_inited())
            return;

        extclamp.set_limits(ext_lo, ext_hi);

        double int_lo = scaller.ext2int(ext_lo);
        double int_hi = scaller.ext2int(ext_hi);
        scaller.swap_if_reverse(int_lo, int_hi);
        intclamp.set_limits(int_lo, int_hi);
    }

    void limits_reevaluate()
    {
        if (!multipliers_inited())
            return;

        set_limits_external(extclamp.lo, extclamp.hi);
    }

    void set_limits_internal(double int_lo, double int_hi)
    {
        if (!multipliers_inited())
            return;

        double ext_lo = scaller.int2ext(int_lo);
        double ext_hi = scaller.int2ext(int_hi);
        scaller.swap_if_reverse(ext_lo, ext_hi);
        intclamp.set_limits(int_lo, int_hi);
        extclamp.set_limits(ext_lo, ext_hi);
    }

    virtual bool is_ready() = 0;
    virtual bool is_unready() = 0;

public:
    virtual int unsafe_absolute_move_iu(double tgt,
                                        PolymorphSpeed speed,
                                        PolymorphAccel accel) = 0;
    virtual int unsafe_relative_move_iu(double dist,
                                        PolymorphSpeed speed,
                                        PolymorphAccel accel) = 0;
    virtual int unsafe_absolute_move_units(double dist,
                                           PolymorphSpeed speed,
                                           PolymorphAccel accel) = 0;
    virtual double get_position_iu() = 0;
    virtual void nullify_position() = 0;

    void set_default_speed_ups(double ups)
    {
        _default_speed.set(PositionUnit::EXTERNAL, TimeUnit::SECONDS, ups);
    }

    void set_default_speed_rpm(double rpm)
    {
        _default_speed.set(PositionUnit::ROTATION, TimeUnit::MINUTES, rpm);
    }

    void set_default_accel_rpmm(double rpmm)
    {
        _default_accel.set(PositionUnit::ROTATION, TimeUnit::MINUTES, rpmm);
    }

    void set_default_accel_upss(double upss)
    {
        _default_accel.set(PositionUnit::EXTERNAL, TimeUnit::SECONDS, upss);
    }

    void set_maximum_speed_ups(double ups)
    {
        _maximum_speed.set(PositionUnit::EXTERNAL, TimeUnit::SECONDS, ups);
    }

    void set_maximum_speed_rpm(double rpm)
    {
        _maximum_speed.set(PositionUnit::ROTATION, TimeUnit::MINUTES, rpm);
    }

    void set_maximum_accel_rpmm(double rpmm)
    {
        _maximum_accel.set(PositionUnit::ROTATION, TimeUnit::MINUTES, rpmm);
    }

    void set_current_accel_rpss(double rpss)
    {
        _current_accel.set(PositionUnit::ROTATION, TimeUnit::SECONDS, rpss);
    }

    void set_maximum_accel_rpss(double rpss)
    {
        _maximum_accel.set(PositionUnit::ROTATION, TimeUnit::SECONDS, rpss);
    }

    void set_default_accel_rpss(double rpss)
    {
        _default_accel.set(PositionUnit::ROTATION, TimeUnit::SECONDS, rpss);
    }

    void set_current_accel_upss(double upss)
    {
        _current_accel.set(PositionUnit::EXTERNAL, TimeUnit::SECONDS, upss);
    }

    double get_current_accel_upss()
    {
        return _current_accel.unit_per_second_sqr();
    }

    void set_maximum_accel_upss(double upss)
    {
        _maximum_accel.set(PositionUnit::EXTERNAL, TimeUnit::SECONDS, upss);
    }

    void set_current_speed_ups(double ups)
    {
        _current_speed.set(PositionUnit::EXTERNAL, TimeUnit::SECONDS, ups);
    }

    void set_current_speed_rpm(double rpm)
    {
        _current_speed.set(PositionUnit::ROTATION, TimeUnit::MINUTES, rpm);
    }

    double get_current_accel_ms() const
    {
        return _current_accel.get_milliseconds_for_nominal();
    }

    void set_maximum_accel_ms(double ms)
    {
        _maximum_accel.set_milliseconds_for_nominal(ms);
    }

    void set_default_accel_ms(double ms)
    {
        _default_accel.set_milliseconds_for_nominal(ms);
    }

    void set_current_accel_ms(double ms)
    {
        _current_accel.set_milliseconds_for_nominal(ms);
    }

    double get_current_speed_ups() const
    {
        return _current_speed.unit_per_second();
    }

    double get_current_speed_rpm() const
    {
        return _current_speed.getvalue(PositionUnit::ROTATION,
                                       TimeUnit::MINUTES);
    }

    double get_default_accel_upss() const
    {
        return _default_accel.getvalue(PositionUnit::EXTERNAL,
                                       TimeUnit::SECONDS);
    }

    double get_default_speed_ups() const
    {
        return _default_speed.getvalue(PositionUnit::EXTERNAL,
                                       TimeUnit::SECONDS);
    }

    double get_current_speed_upm() const
    {
        return _current_speed.getvalue(PositionUnit::EXTERNAL,
                                       TimeUnit::MINUTES);
    }

    double get_maximum_speed_rpm() const
    {
        return _maximum_speed.getvalue(PositionUnit::ROTATION,
                                       TimeUnit::MINUTES);
    }

    double get_default_speed_rpm() const
    {
        return _default_speed.getvalue(PositionUnit::ROTATION,
                                       TimeUnit::MINUTES);
    }

    double get_default_accel_ms() const
    {
        return _default_accel.get_milliseconds_for_nominal();
    }

    virtual void stop() = 0;

    virtual ~MoveApiClient() = default;

    virtual std::string info();
    // @return true if command founded. false outerwise.
    virtual bool command(const nos::argv &args, nos::ostream &os) = 0;
    virtual std::string command_help() = 0;
};

#endif