#ifndef RALGO_CYNEMATIC_COORDS_READER_H
#define RALGO_CYNEMATIC_COORDS_READER_H

/**
    Т.к. Библиотека кинематических цепей используется
    для управления реальными устройствами, удобно подтягить
    текущие координаты по прямым указателям.

    Для этого существует интерфейс read_coord, которые следует реализовывать
    на объекте кинематической пары.
*/

namespace ralgo
{
    class coord_reader
    {
        virtual float read_coord(float multiplier);
    };
}

#endif
