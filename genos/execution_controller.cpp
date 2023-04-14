#include <genos/execution_controller.h>

__attribute__((init_priority(101)))
igris::dlist<genos::execution_controller, &genos::execution_controller::_lnk>
    genos::execution_controller_manager::_list;

genos::execution_controller::execution_controller(const char *mnemo)
{
    _mnemo = mnemo;
    genos::execution_controller_manager::register_controller(this);
}
