/**
    @file

    Некоторое количество глобальных объектов, которых на самом деле не должно
   быть.
    TODO.
*/

#ifndef STUFF_H
#define STUFF_H

#include <igris/event/multiple_delegate.h>

extern int scanPointNumber;

extern igris::multiple_delegate<int> g_scanPointSignal;
extern igris::multiple_delegate<> descanSignal;
extern igris::multiple_delegate<> wrongScanPointSignal;

#endif
