/**
    @file
    Абстрактный класс объекта оси.
    Реализует базовые абстракции, такие как масштабирования,
    обработка статуса операции и т.д.
*/

#ifndef ABSTRACT_AXIS_H
#define ABSTRACT_AXIS_H

#include <FullyClosedGear.h>
#include <GearSettings.h>
#include <LogicalDevice.h>
#include <moveapi/MoveApiClient.h>
#include <ScanApiClient.h>
#include <comm/notifymap.h>
#include <defs.h>
#include <exception.h>
#include <igris/event/multiple_delegate.h>
#include <include_helper.h>
#include <modes.h>
#include <ndmath/named_cartesian_correction.h>
#include <nos/shell/argv.h>
#include <nos/trent/binder.h>
#include <nos/trent/settings.h>
#include <set>
#include <status.h>
#include <vector>

class SyncScanApiClient;
class MoveApiClient;
class InterpolationGroup_v2;

class AbstractAxis : public LogicalDevice
{
public:
    // Этих переменных не должно быть в классе.
    std::vector<int> for_modes = {};
    // Этих переменных не должно быть в классе.

    std::unique_ptr<ScanApiClient> scan_client = {};

private:
    NotifyTheme operationStatusNotify = {};
    Operation operation = Operation::NONE;

protected:
    NotifyTheme operationStopTypeNotify = {};
    // Этих переменных не должно быть в классе.
    InterpolationGroup_v2 *igcontroller = nullptr;
    int _number = {}; // номер в списке осей сервера
    // Этих переменных не должно быть в классе.

    bool _has_correction = false;
    bool _correction_enabled = true;
    ndmath::cartesian_correction one_axis_correction_table = {};
    std::unique_ptr<MoveApiClient> move_client = {};
    igris::multiple_delegate<int> scanPointSignal = {};
    nos::trent_binder<double> *unitBackwardLimit_syncer = nullptr;
    nos::trent_binder<double> *unitForwardLimit_syncer = nullptr;
    NotifyTheme torqueNotifyTheme = {};
    NotifyTheme limitSwitchStatusNotify = {};
    NotifyTheme scanPointNotify = {};
    NotifyTheme scanTrigErrorNotify = {};

    std::mutex last_data_mutex = {};

    bool m_reversed = false;

private:
    NotifySmoothTheme positionNotify = {};
    NotifySmoothTheme positionUnitNotify = {};
    NotifySmoothTheme positionUnitNotifyNotCorrected = {};
    double _last_position_unit = 0;
    double _last_position_unit_not_corrected = 0;

private:
    void update_current_position_handler(double unitpos, bool force);

public:
    bool axis_inited = false;
    NotifyTheme isTandemSynchronizedNotifyTheme = {};

public:
    Operation operation_status()
    {
        return operation;
    }

public:
    void operation_finish_callback();
    void start_operation_notify();
    void
    send_operation_status_notify_for_interpolation_mode(Operation operation);

    int scpi_api_index() const
    {
        return _number;
    }

    double reverse_corrected_unit_position(double val)
    {
        if (correction_table_used())
        {
            auto corrected_point =
                one_axis_correction_table.inverted_corrected_point({val});
            return corrected_point[0];
        }
        else
            return val;
    }

    void update_current_position_handler_with_reverse_if_need(
        double noncorrected_unitpos, bool force);

    virtual GearSettings *gear_settings() = 0;
    virtual FullyClosedGear *fully_closed_gear() = 0;

    void scan_point_handler(int num)
    {
        scanPointNotify.notify(num);
        scanPointSignal(num);
    }

    void bind_scan_point_handler(igris::delegate<void, int> handler)
    {
        scanPointSignal += handler;
    }

    void unbind_scan_point_handler(igris::delegate<void, int> handler)
    {
        scanPointSignal -= handler;
    }

    void wrong_scan_point_handler()
    {
        nos::println("ScanApiClient_Syncro::wrongScanPointHandler");
        scanTrigErrorNotify.notify();
    }

    void set_igcontroller(InterpolationGroup_v2 *ig)
    {
        igcontroller = ig;
    }

    void set_number(size_t num)
    {
        this->_number = num;
    }

    size_t number() const
    {
        return _number;
    }

    void set_reverse(bool reverse)
    {
        m_reversed = reverse;
    }

    bool is_reversed()
    {
        return m_reversed;
    }

public:
    bool in_moving_state()
    {
        return operation == Operation::USERMOVE;
    }

    void enable_power_off_position_keeping();

    virtual double unitRatio() const = 0;
    double unitRatio_forScpiApi()
    {
        return const_cast<const AbstractAxis *>(this)->unitRatio();
    }

    void save_one_axis_correction_to_runtime();
    void load_one_axis_correction_from_runtime();

    const ndmath::cartesian_correction &get_one_axis_correction() const
    {
        return one_axis_correction_table;
    }

    Operation operation_status() const
    {
        return operation;
    }

    // TODO: дублирование
    bool has_correction() const
    {
        return _has_correction;
    }

    // Эта функция проверяет наличие таблицы, но не её активированность.
    // Для проверки активности используется correction_table_used
    bool has_correction_table() const
    {
        return _has_correction;
    }

    nos::trent one_axis_correction_as_trent() const;

    void set_one_axis_correction(const ndmath::cartesian_correction &cor,
                                 bool correction_enabled)
    {
        one_axis_correction_table = cor;
        _has_correction = true;
        _correction_enabled = correction_enabled;
        save_one_axis_correction_to_runtime();
    }

    void clear_one_axis_correction()
    {
        one_axis_correction_table = {};
        _has_correction = false;
        _correction_enabled = true; // default state is true
        save_one_axis_correction_to_runtime();
    }

    bool is_correction_enabled() const
    {
        return _correction_enabled;
    }

    virtual ~AbstractAxis();
    AbstractAxis(const AbstractAxis &) = delete;
    AbstractAxis &operator=(const AbstractAxis &) = delete;

    AbstractAxis(const std::string &name);

    //bool isActive(); // < for HelixAxis only. deprecated

    double unitBackwardLimit();
    double unitForwardLimit();
    double unitBackwardLimit() const;
    double unitForwardLimit() const;
    void unitBackwardLimit(double arg);
    void unitForwardLimit(double arg);
    void unitLimits(double back, double forw);

    /// Обработчик для учёта обновления пределов в логике потомка.
    virtual void limits_update() {}

    // Extended limit protectors for polygon compatible.
    virtual double unitBackwardLimit_protected() const
    {
        return unitBackwardLimit();
    }
    virtual double unitForwardLimit_protected() const
    {
        return unitForwardLimit();
    }

    // Velocity settings:
    virtual void setSpeed_rpm(int32_t rpm) = 0;
    virtual void setSpeed_ups(double arg) = 0;
    virtual int32_t getSpeed_rpm() const = 0;
    virtual double getSpeed_ups() const = 0;
    virtual double getSpeed_upm() const = 0;

    virtual void maxSpeed_rpm(uint32_t) = 0;
    virtual void maxSpeed_ups(double arg) = 0;
    virtual uint32_t maxSpeed_rpm() const = 0;
    virtual double maxSpeed_ups() const = 0;

    virtual void defaultSpeed_rpm(uint32_t) = 0;
    virtual void defaultSpeed_ups(double arg) = 0;
    virtual uint32_t defaultSpeed_rpm() const = 0;
    virtual double defaultSpeed_ups() const = 0;

    virtual void set_speed_procent(double proc) = 0;

    // Acceleration settings:
    virtual void setAccel_ups2(double arg) = 0;
    virtual void setAccel_ms(int32_t ms);
    virtual double getAccel_ups2() const = 0;
    virtual int32_t getAccel_ms() const = 0;

    virtual void minAccel_ms(uint32_t);
    virtual void maxAccel_ups2(double arg) = 0;
    virtual uint32_t minAccel_ms() const = 0;
    virtual double maxAccel_ups2() const = 0;

    virtual void defaultAccel_ups2(double arg) = 0;
    virtual void defaultAccel_ms(uint32_t);
    virtual uint32_t defaultAccel_ms() const = 0;
    virtual double defaultAccel_ups2() const = 0;

    // HANDLERS:
    virtual void onReadyHandler();
    virtual void onMoverReady(Device *);
    virtual void onCriticalErrorHandler(Device *dev);

    virtual void preset();
    virtual void generate_trigger();
    virtual void manual_trigger_mode(int32_t en);
    virtual void no_trigger_mode(int32_t en);
    virtual void bitrigmode(int32_t en);

    virtual void stopLastOperation(StopType arg);
    virtual void absoluteUnitMove_impl(double arg, bool lock_mode) = 0;

    void relativeUnitMove(double arg);
    void absoluteUnitMove(double arg);

    virtual void interpolationRelativeUnitMove(
        double arg, const std::vector<std::pair<int, double>> &subax);
    virtual void interpolationAbsoluteUnitMove(
        double arg, const std::vector<std::pair<int, double>> &subax);

    double last_unit_position();
    double last_unit_position_not_corrected()
    {
        return _last_position_unit_not_corrected;
    }

    virtual int getLimitSwitchStatus();

    int32_t getStatus();

    virtual std::string status_string();

    virtual void setNullPosition();

    void jog(Direction dir);
    void stop();
    virtual void stop_impl() = 0;

    virtual bool haveScan();
    virtual bool haveAbsoluteMove();
    virtual bool haveSetReference();
    virtual bool hasInterpolation();

    // Scaner API
    virtual void scanMove(int32_t arg);
    virtual void scanUnitMove(double arg);
    virtual void scanForwardZone(int32_t arg);
    virtual void scanUnitForwardZone(double arg);
    virtual void scanBackwardZone(int32_t arg);
    virtual void scanUnitBackwardZone(double arg);
    virtual void scanStart();
    virtual void scanCompareStart();
    virtual void scanPoints(int32_t arg);
    virtual int32_t scanMove();
    virtual double scanUnitMove();
    virtual int32_t scanForwardZone();
    virtual double scanUnitForwardZone();
    virtual int32_t scanBackwardZone();
    virtual double scanUnitBackwardZone();
    virtual void scanEnableExternalTriggerNotify();
    virtual int32_t scanPoints();
    virtual int32_t scan_get_encoder_position();

    virtual std::vector<double>
    scan_points_position(int offset, int size, bool from_end) = 0;

    virtual int32_t scan_discrete_output_set_state(int num, int state);
    virtual int32_t scan_discrete_output_set_unical(int num);
    virtual int32_t scan_discrete_output_disable();

    virtual int32_t synchroSpeedTest();

    virtual void servo_on();
    virtual void servo_off();

    void notifiesRegistry();
    void init_runtime_settings();

    //    void singleTorqueNotify(int32_t arg);
    //    void multiTorqueNotify(std::vector<int32_t> arg);

    void limitSwitchStatusSlot(uint8_t arg);

    int32_t getOperationCode();
    void checkAxisIsReady();
    // virtual void operationCorrectFinish() = 0;

    virtual void compare_table_enable(bool en) = 0;
    virtual void compare_table_add_point(int32_t pos) = 0;
    virtual void compare_table_clear() = 0;
    virtual void scan_subsystem_reset() = 0;

    virtual void command(const nos::argv &args, nos::ostream &out);
    virtual std::string info() = 0;
    virtual std::string command_help();

    bool correction_table_used();
    bool is_need_to_prevent_moving_to_position(double tgtpos,
                                               bool notify_if_prevented);

    void absoluteUnitMove_delegateToIGroup(double tgtpos);

    virtual int32_t trigreturn_time();
    virtual void com_trigger_duration(int d) {}
    virtual void com_trigger_polarity(int d) {}
};

#endif
