#include "main.h"
#include <nos/argparse.h>
#include <fstream>



using namespace std;

#define FILE_PATH "/home/rfmeas/HMI_IP.txt"
#define DEFAULT_HMI_IP "192.168.2.253"
#define DEFAULT_SCPI_PORT 5025
#define DEFAULT_NCPI_PORT 5026

std::recursive_mutex global_mutex;

ModbusHMI *hmi = NULL;
Scpi *scpi = NULL;
string hmiIp = DEFAULT_HMI_IP;
const char *modbusPort = "502";
uint16_t scpiPort = DEFAULT_SCPI_PORT;
uint16_t ncpiPort = DEFAULT_NCPI_PORT;
bool lastConnectionStatus = false;
bool jogBackwardDir = false;
uint32_t selectedAxis = 0;
float targetPos = 0;
float oldSlValue = 0;
float slValue = 0;

void init_scpi_subsystem()
{
    scpi->updateFullSystemInfo();
    for (int i = 0; i < std::min<int>((int)scpi->getAxesCount(), 15); i++)
        scpi->setNotifications(Scpi::NotificationType::Position, i, true);
    scpi->setNotifications(Scpi::NotificationType::SystemStatus, 0, true);
}

void setSliderValueForSelectedAxis();

void selectAxisCallback(int selectedAxis);
void activateFistAxis()
{
    selectedAxis = 0;
    selectAxisCallback(selectedAxis);
}

void modbus_init_part()
{
    hmi = new ModbusHMI(hmiIp.data(), modbusPort);
    hmi->connect(HMI_SLAVE_ADDR);
    hmi->setSelectedAxisAddr(7);
    hmi->setSelectedDevAddr(8);
    hmi->setMoveSpeedAddr(9);
    hmi->setTargetPosAddr(4);
    hmi->setCurrentModeAddr(10);
    hmi->setSystemReadyStatusAddr(20);
    hmi->addLeds(LED1_COIL, 2);
    hmi->addIndicationLabel(CURR_MODE_COIL);
    hmi->addIndicationLabel(JOG_DIR_COIL);
    hmi->addButtons(A1_COIL, 21, ButtonCallback);
    hmi->addGaugeVariable(
        GAUGE_LEFT_ADDR, GAUGE_LEFT_MIN_ADDR, GAUGE_LEFT_MAX_ADDR);
    hmi->addSlider(SLIDER_SPEED_ADDR);
    hmi->addSlider(SLIDER_POS_ADDR);
    hmi->addValueLabel(CURR_AXIS_ADDR);
    hmi->addValueLabel(LABEL_TARGET_POS_ADDR, true);
    hmi->addValueLabel(CURRENT_POS, true);
    hmi->setContinuousDataScanning(HMI_ALL, 10);
    hmi->setSliderValue(0, (int16_t)oldSlValue);
}

void position_updated_slot(int axisno, float pos)
{

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (axisno == (int)selectedAxis)
    {
        nos::println("UPDATED:", (int)selectedAxis, pos);
        hmi->setLabelValue(2, pos);
    }
}

// MODBUS-TCP to SCPI bridge for remote control HMI devices
int main(int argc, char *argv[])
{
    if (checkRunArgs(argc, argv, hmiIp, scpiPort, ncpiPort))
        return 0;
    cout << "HMI Control v1.0, Radioline 2021" << endl;
    cout << "For using with HMI model:Kinco SZ7ES" << endl;
    cout << "Startup parameters are: HMI IP:" << hmiIp
         << ", SCPI port:" << scpiPort << ", NCPI port:" << ncpiPort
         << ", MODBUS TCP port:" << modbusPort << endl;

    modbus_init_part();
    scpi = new Scpi("127.0.0.1", scpiPort, ncpiPort);
    scpi->set_position_updated_callback(position_updated_slot);

    while (1)
    {
        try
        {
            if (!hmi->isConnected())
            {
                std::this_thread::sleep_for(100ms);
                continue;
            }

            if (!scpi->isConnected())
            {
                scpi->reconnect();
                init_scpi_subsystem();
                activateFistAxis();
                setSliderValueForSelectedAxis();
                std::this_thread::sleep_for(100ms);
                continue;
            }

            if (lastConnectionStatus == false)
                nos::println("Scpi connection established.");
            lastConnectionStatus = true;

            if (scpi->axesTotalCount() == 0)
            {
                std::this_thread::sleep_for(100ms);
                continue;
                
            }

            uint32_t axis = selectedAxis;
            hmi->setGaugeValue(0, scpi->getAxisPos(axis));
            hmi->setGaugeLimits(0,
                                scpi->getAxisBackwardLimit(axis),
                                scpi->getAxisForwardLimit(axis));
            hmi->setLed(LED_GREEN, scpi->getSystemReadyStatus());
            hmi->setLed(LED_RED, scpi->getDevAlarmStatus());
            hmi->setIndicationLabel(1, !jogBackwardDir);

            slValue = hmi->readSliderValue(0);
            if (hmi->lastCmdStatus == -1)
                continue;
            float minPos = scpi->getAxisBackwardLimit(selectedAxis);
            float maxPos = scpi->getAxisForwardLimit(selectedAxis);
            targetPos =
                (hmi->readSliderValue(1) / 1000.0f) * (maxPos - minPos) +
                scpi->getAxisBackwardLimit(selectedAxis);
            hmi->setLabelValue(1, targetPos);
            if (oldSlValue != slValue)
            {
                oldSlValue = slValue;
                auto maxspeed = scpi->getAxisMaxSpeed(axis);
                float speed = (slValue / 1000.0f) * maxspeed;
                scpi->axisSetSpeed(axis, speed);
            }
            usleep((1000 / HMI_FRAMERATE) * 1000);
        }
        catch (const scpi_connection_error &)
        {
            if (lastConnectionStatus == true)
                nos::println("Lost scpi connection.");
            lastConnectionStatus = false;
            scpi->disconnect();
        }
    }
    return 0;
}

void setSliderValueForSelectedAxis()
{
    oldSlValue = slValue = scpi->getAxisSpeed(selectedAxis) * 1000 /
                           scpi->getAxisMaxSpeed(selectedAxis);
    hmi->setSliderValue(0, (int16_t)slValue);
}

void selectAxisCallback(int axno)
{
//    nos::println("Select axis:", axno);
    hmi->setLabelValue(0, axno + 1);
//    nos::println("Set current position:", scpi->axis_list[axno].pos);
    hmi->setLabelValue(2, scpi->axis_list[axno].pos);
    setSliderValueForSelectedAxis();
}

void ButtonCallbackPressed(uint16_t buttonId)
{
    if (buttonId == JOG_DIR_COIL)
    {
        return;
    }

    cout << "Button pressed: " << buttonId << endl;
    // if (buttonId <= 6 && buttonId >= 3)
    // {
    //     if (buttonId <= scpi->getAxesCount())
    //     {
    //         selectedAxis = buttonId - 1;
    //         selectAxisCallback(selectedAxis);
    //     }
    // }

    if (buttonId == A1_COIL)
    {
        if (selectedAxis == 0)
            selectedAxis = scpi->axesTotalCount() - 1;
        else
            selectedAxis--;
        selectAxisCallback(selectedAxis);
    }
    if (buttonId == A2_COIL)
    {
        if ((int)selectedAxis == scpi->axesTotalCount() - 1)
            selectedAxis = 0;
        else
            selectedAxis++;
        selectAxisCallback(selectedAxis);
    }
    else if ((buttonId >= JOG_00_1_COIL && buttonId <= JOG_100_COIL) ||
             buttonId == JOG_000_1_COIL)
    {
        double delta = 0.01 * pow(10, buttonId - JOG_00_1_COIL);
        if (buttonId == JOG_000_1_COIL)
            delta = 0.001;
        if (jogBackwardDir)
            scpi->axisMove(selectedAxis, -(float)delta);
        else
            scpi->axisMove(selectedAxis, (float)delta);
    }
    else if (buttonId == MOVE_TO_ZERO)
    {
        scpi->axisMoveAbs(selectedAxis, 0.0);
    }
    else
    {
        switch (buttonId)
        {
        case STOP_COIL:
            scpi->stopAll();
            break;
        case RUN_COIL:
            scpi->axisMoveAbs(selectedAxis, targetPos);
            break;
        case HAND0_COIL:
            jogBackwardDir = !jogBackwardDir;
            break;
        case HAND1_COIL:
            break;
        case DEC_COIL:
            scpi->axisJog(selectedAxis, false);
            break;
        case INC_COIL:
            scpi->axisJog(selectedAxis, true);
            break;
        default:
            break;
        }
    }
}

void ButtonCallbackReleased(uint16_t buttonId)
{
    cout << "Button released: " << buttonId << endl;
    switch (buttonId)
    {
    case DEC_COIL:
        scpi->axisStop(selectedAxis);
        break;
    case INC_COIL:
        scpi->axisStop(selectedAxis);
        break;
    default:
        break;
    }
}

void ButtonCallback(uint16_t buttonId, bool state)
{
    if (scpi)
    {
        if (!scpi->isConnected())
            return;
    }
    else
        return;

    if (state)
        ButtonCallbackPressed(buttonId);
    else
        ButtonCallbackReleased(buttonId);
}

bool checkRunArgs(int argc,
                  char *argv[],
                  string &hmiIp,
                  uint16_t &scpiPort,
                  uint16_t &ncpiPort)
{
    nos::argparse parser("ModbusRemoteControl", "Modbus HMI server");
    parser.add_argument("hmi_ip", 'i', "HMI IP address", DEFAULT_HMI_IP, false);
    parser.add_argument("help", 'h', "Show this help message and exit");
    parser.add_argument("scpi_port",
                        's',
                        "SCPI port",
                        std::to_string(DEFAULT_SCPI_PORT),
                        false);
    parser.add_argument("ncpi_port",
                        'n',
                        "NCPI port",
                        std::to_string(DEFAULT_NCPI_PORT),
                        false);
    parser.add_argument(
        "version", 'v', "Show program's version number and exit");

    auto args = parser.parse_args(argc, argv);
    scpiPort = std::stoul(args.value("scpi_port"));
    ncpiPort = std::stoul(args.value("ncpi_port"));
    
    ////////////////////////////////////////////////////////////////////

        std::ifstream tmpfile(FILE_PATH);
        if(tmpfile.is_open())
            {
            tmpfile >> hmiIp;
            tmpfile.close();
            }
        else hmiIp = args.value("hmi_ip");
            
    ////////////////////////////////////////////////////////////////////

    if (args.flag("version"))
    {
        nos::println(VERSION);
        exit(0);
    }

    if (!args || args.flag("help"))
    {
        parser.print_help();
        exit(0);
    }

    return false;
}
