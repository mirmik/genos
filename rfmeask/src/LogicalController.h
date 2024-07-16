/**
    @file
*/

#ifndef LOGICALCONTROLLER_H
#define LOGICALCONTROLLER_H

/**
    Интерфейс для объекта логического устройства управляющего другими
   логическими устройствами.
*/
class LogicalController
{
public:
    virtual void stop() = 0;
    virtual ~LogicalController() = default;
};

#endif // LOGICALCONTROLLER_H
