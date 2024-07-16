#include <servos/MRJ5Servo.h>


    MRJ5Servo::MRJ5Servo(const char *name,
                    MitsubishiCommunicator *mrs,
                    uint8_t rs485addr) 
                    : MitsubishiServo(name, mrs, rs485addr)
                    
                    {}