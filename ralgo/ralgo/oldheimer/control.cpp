#include <igris/sync/syslock.h>
#include <ralgo/oldheimer/control.h>

dlist_head heimer::control_node_list = DLIST_HEAD_INIT(control_node_list);

int heimer::control_node::activate()
{
    int sts;

    if (flags & HEIM_IS_ACTIVE)
        return 0;

    if ((sts = take_control()))
        return sts;

    if ((sts = on_activate()))
    {
        release_control();
        return sts;
    }

    flags |= HEIM_IS_ACTIVE;
    return 0;
}

int heimer::control_node::deactivate()
{
    int sts;

    if (is_controlled())
        return HEIM_ERR_IS_BUSY;

    if (!(flags & HEIM_IS_ACTIVE))
        return 0;

    if ((sts = on_deactivate()))
        return sts;

    release_control();

    flags &= ~HEIM_IS_ACTIVE;
    return 0;
}

int heimer::control_node::take_control()
{
    int sts = 0;

    control_node *it = nullptr;
    while ((it = iterate(it)))
    {
        if (it->is_active() == false ||
            (!it->is_multicontrolled() && it->is_controlled()))
        {
            sts = HEIM_ERR_IS_CONTROL_FAULT;
            break;
        }

        sts = it->on_external_take(this);
        if (sts)
            break;
        it->flags |= HEIM_IS_CONTROLLED;
        it->controller = this;
    }

    if (sts)
        release_control();

    return sts;
}

int heimer::control_node::release_control()
{
    int sts = 0;

    control_node *it = nullptr;
    while ((it = iterate(it)))
    {
        if (it->is_active())
        {
            if (!it->is_multicontrolled())
            {
                sts = it->on_external_release(this);
                it->flags &= ~HEIM_IS_CONTROLLED;
                it->controller = nullptr;
                (void)sts;
            }
            else
            {
                sts = it->on_external_release(this);
                if (controllers_iterate(nullptr) == nullptr)
                    it->flags &= ~HEIM_IS_CONTROLLED;
                (void)sts;
            }
        }
    }

    return 0;
}

void heimer::control_node::on_interrupt_common(
    control_node *slave, // источник, переславший сигнал
    control_node *source, // изначальный источник сигнала
    interrupt_args *interrupt)
{
    bool prevent = on_interrupt(slave, source, interrupt);
    if (prevent)
        return;

    control_node *it = nullptr;
    while ((it = controllers_iterate(it)))
    {
        it->on_interrupt_common(this, source, interrupt);
    }
}

void heimer::control_node::throw_interrupt(interrupt_args *interrupt)
{
    on_interrupt_common(this, this, interrupt);
}

heimer::control_node *
heimer::control_node::vector_iterate(heimer::control_node **bit,
                                     heimer::control_node **eit,
                                     heimer::control_node *curit)
{
    heimer::control_node **it = bit;
    for (; it != eit; ++it)
    {
        if (*it == curit)
        {
            ++it;
            if (it == eit)
                return nullptr;
            return *it;
        }
    }

    return nullptr;
}

void heimer::control_node::serve()
{
    if (!is_active())
        return;

    serve_impl();
}
