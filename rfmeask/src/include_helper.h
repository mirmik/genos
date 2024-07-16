/**
    @file
    Объявления часто используемых классов вынесены в этот файл,
    для использования в качестве forward declaration.
*/

#ifndef INCLUDE_HELPER_H
#define INCLUDE_HELPER_H

#include <memory>

namespace nos
{
    class ostream;
}

namespace igris
{
    template <template <class T> class Alloc> class trent_basic;
    template <class T> class trent_binder;
    class trent_syncer_slice;
    using trent = trent_basic<std::allocator>;
}

class NotifyBrocker;
class NotifyTheme;
class NotifySmoothTheme;
class DeviceChecker;
class AbstractServo;

#endif