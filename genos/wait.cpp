#include <asm/irq.h>
#include <genos/schedee.h>
#include <genos/schedee_api.h>
#include <genos/wait.h>

#include <igris/sync/syslock.h>
#include <igris/util/bug.h>
#include <igris/util/macro.h>

int unwait_schedee_waiter(struct waiter *w)
{
    genos::schedee *sch = mcast_out(w, genos::schedee, waiter);
    schedee_start(sch);
    return 0;
}

void _unwait_schedee(void *priv)
{
    genos::schedee *sch = (genos::schedee *)priv;
    unwait_schedee_waiter(&sch->waiter);
}

/**
 * Подписывает текущую единицу исполнения на очередь ожидания @head.
 *
 * Если единица исполнения не имеет флага can_displace, метод
 * current_schedee_displace вернёт -1 вместо вытеснения контекста. Функция
 * завершится немедленно, а завершение исполнения возлагается на
 * пользовательский код.
 *
 * @head - очередь ожидания
 * @priority - приоритетность - добавляет токен в начало или конец очереди
 * @future - Указатель на слово для приёма возвращаемого значения.
 * */
int wait_current_schedee(igris::dlist_base *head, int priority, void **future)
{
    (void)future;
    genos::schedee *sch;
    sch = genos::current_schedee();

    if (sch == NULL)
    {
        return -1;
    }

    sch->sch_state = genos::schedee_state::wait;

    // Для лока используем апи прерываний, чтобы не нарушать консистентность
    // подсистемы syslock
    irqstate_t save = irqs_save();
    sch->control_lnk.unlink();

    waiter_delegate_init(&sch->waiter, _unwait_schedee, (void *)sch);
    if (priority)
        head->move_front(sch->waiter.lnk);
    else
        head->move_back(sch->waiter.lnk);
    irqs_restore(save);

    return 0;
}

int waitchild()
{
    genos::schedee *sch;
    sch = genos::current_schedee();

    sch->sch_state = genos::schedee_state::wait_schedee;

    system_lock();
    sch->control_lnk.unlink();
    system_unlock();

    genos::current_schedee_displace();

    return 0;
}

/*int waitqueue_no_displace_wait(struct waitqueue *queue)
{
    int priority = 0;
    igris::dlist_node *head = &queue->lst;
    genos::schedee *sch;
    sch = genos::current_schedee();

    sch->ctr.type = CTROBJ_WAITER_SCHEDEE;
    sch->sch_state = genos::schedee_state::wait;

    // Для лока используем апи прерываний, чтобы не нарушать консистентность
    // подсистемы syslock
    irqstate_t save = irqs_save();
    sch->ctr.lnk.unlink();

    if (priority)
        sch->ctr.lnk.move_next_than(head);
    else
        sch->ctr.lnk.move_prev_than(head);
    irqs_restore(save);

    return 0;
}
*/