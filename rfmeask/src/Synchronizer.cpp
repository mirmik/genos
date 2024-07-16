/**
    @file
*/

#include <Synchronizer.h>
//#include <igris/util/numconvert.h>

bool Synchronizer::scanmode = false;
std::mutex Synchronizer::mtx;

Synchronizer::Synchronizer(const char *name, RS232Listener *com, uint8_t addr) :
    AbstractSyncro(name)
{
    drv = com;
    rs232_addr = addr;
}

void Synchronizer::updaterThreadFunc()
{
    while (1)
    {
        mtx.lock();

        try
        {
            if (scanmode == false && connected == false)
                test();

            connected = true;
        }
        catch (const DeviceRefuseException &)
        {
            connected = false;
            goto end;
        }
        catch (const std::exception &ex)
        {
            nos::println("Exception in update thread: ", ex.what());
        }

    end:
        mtx.unlock();
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_updaterTimeout));
    }
}

void Synchronizer::preset()
{
    api_version = scanApiVersion();
    stop();

    initial_encoder_position = drv->get_encoder_position(rs232_addr);
}

int Synchronizer::getAlarmStatus()
{
    return 0;
}

void Synchronizer::setScanZone(int32_t arg)
{
    invokeDecorator([this, arg]() { drv->incpos_move(rs232_addr, arg); });
}

void Synchronizer::setScanPoints(int32_t arg)
{
    invokeDecorator([this, arg]() { drv->incpos_points(rs232_addr, arg); });
}

void Synchronizer::setScanStartZone(int32_t arg)
{
    invokeDecorator([this, arg]() { drv->incpos_strt_zone(rs232_addr, arg); });
}

void Synchronizer::setScanStopZone(int32_t arg)
{
    invokeDecorator([this, arg]() { drv->incpos_stop_zone(rs232_addr, arg); });
}

void Synchronizer::setLoadGain(double gain)
{
    int32_t numer = 1000;
    int32_t denum = (double)numer / gain;
    invokeDecorator([this, numer, denum]() {
        drv->spdmul_numer(rs232_addr, numer);
        drv->spdmul_denum(rs232_addr, denum);
    });
}

double Synchronizer::encoder_multiplier()
{
    nos::println("encoder_numerator:", encoder_numerator);
    nos::println("encoder_denumerator:", encoder_denumerator);
    double mul = (double)encoder_numerator / (double)encoder_denumerator;
    return mul;
}

void Synchronizer::push_point_information(int64_t point)
{
    double encmul = encoder_multiplier();
    if (std::isnan(encmul))
    {
        encmul = 50;
    }

    double val = point * encmul;

    nos::println("push_point_information:", point, encoder_multiplier());
    nos::println("push_point_information:", val);
    point_information_ring.push(val);
}

void Synchronizer::setSpeed(int32_t spd)
{
    invokeDecorator([this, spd]() { drv->speed(rs232_addr, spd); });
}

void Synchronizer::stop()
{
    invokeDecorator([this]() { drv->stop(rs232_addr); });
}

void Synchronizer::test()
{
    invokeDecorator([this]() { drv->test(rs232_addr); });
}

void Synchronizer::cmpscanStart()
{
    drv->point_information_listener = this;
    invokeDecorator([this]() { drv->cmpscan_start(rs232_addr); });
}

void Synchronizer::set_enc_numer(uint32_t arg)
{
    encoder_numerator = arg;
    invokeDecorator([this, arg]() { drv->enc_numer(rs232_addr, arg); });
}

void Synchronizer::set_gen_numer(uint32_t arg)
{
    invokeDecorator([this, arg]() { drv->gen_numer(rs232_addr, arg); });
}

void Synchronizer::set_enc_denum(uint32_t arg)
{
    encoder_denumerator = arg;
    invokeDecorator([this, arg]() { drv->enc_denum(rs232_addr, arg); });
}

void Synchronizer::set_gen_denum(uint32_t arg)
{
    invokeDecorator([this, arg]() { drv->gen_denum(rs232_addr, arg); });
}

void Synchronizer::generate_trigger()
{
    invokeDecorator([this]() { drv->generate_trigger(rs232_addr); });
}

void Synchronizer::manual_trigger_mode(int32_t en)
{
    invokeDecorator(
        [this, en]() { drv->manual_trigger_mode(rs232_addr, !!en); });
}

void Synchronizer::wait_trigger_mode(int en)
{
    invokeDecorator([this, en]() { drv->wait_trigger_mode(rs232_addr, en); });
}

void Synchronizer::bitrigmode(bool en)
{
    invokeDecorator(igris::delegate<void>(
        [this, en]() { drv->bitrigmode(rs232_addr, en); }));
}

int32_t Synchronizer::trigreturn_time()
{
    return drv->get_return_time(rs232_addr, 0);
}

int32_t Synchronizer::synchro_speed_test()
{
    invokeDecorator(igris::delegate<void>([&]() { drv->test(rs232_addr); }));
    return 0;
}

int32_t Synchronizer::get_encoder_position()
{
    return drv->get_encoder_position(rs232_addr);
}

int32_t Synchronizer::discrete_output_set_state(int num, int state)
{
    uint32_t arg = (num << 16 != 0) && state;
    auto mes = drv->Form(74, arg);
    drv->Query(mes.to_buf());
    return 0;
}

int32_t Synchronizer::discrete_output_set_unical(int num)
{
    auto mes = drv->Form(75, num);
    drv->Query(mes.to_buf());
    return 0;
}

int32_t Synchronizer::discrete_output_disable()
{
    auto mes = drv->Form(76, 0);
    drv->Query(mes.to_buf());
    return 0;
}

int32_t Synchronizer::scanApiVersion()
{
    auto mes = drv->Form(64, 0);
    auto ret = drv->Query(mes.to_buf());
    std::string str = ret.substr(3, 8);
    int value = strtol(str.c_str(), nullptr, 16);
    return 0xABCD == value ? 0 : value;
}

void Synchronizer::start_comparing_scan_with_distance_table(
    const std::vector<double> &points)
{
    if (api_version < 210)
        throw std::runtime_error(
            "Scanning with distance table is not supported");

    drv->clear_compare_table(rs232_addr);
    drv->add_compare_table(rs232_addr, points);
    drv->enable_compare_table(rs232_addr, 1);
    cmpscanStart();
}

int32_t Synchronizer::enable_external_trigger_mode()
{
    nos::println("Set Point Listener");
    drv->point_information_listener = this;
    drv->enable_external_trigger_mode(rs232_addr);
    return 0;
}

void Synchronizer::start_comparing_scan_classic_mode()
{
    if (api_version >= 210)
        drv->enable_compare_table(rs232_addr, 0);
    cmpscanStart();
}

std::vector<int64_t>
Synchronizer::scan_points_position(int offset, int size, bool from_end)
{
    nos::println("Synchronizer::scan_points_position");
    return point_information_ring.get_last(offset, size, from_end);
}

void Synchronizer::command(const nos::argv &args, nos::ostream &os)
{
    if (args.size() == 0)
    {
        nos::println_to(os, "No command");
        return;
    }

    if (args[0] == "encoder")
    {
        // int channel = std::stoi(args[1]);
        int value = drv->get_encoder_position(rs232_addr);
        nos::fprintln_to(os, "Encoder value: {}", value);
    }
}

std::string Synchronizer::info()
{
    return "info";
}

std::string Synchronizer::command_help()
{
    return "command_help";
}