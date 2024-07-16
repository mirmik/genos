/**
    @file slist.h
    Заголовок односвязного списка.
*/

#ifndef IGRIS_DATASTRUCT_SLIST_H
#define IGRIS_DATASTRUCT_SLIST_H

#include <igris/compiler.h>
#include <igris/dprint.h>
#include <igris/util/member.h>

struct slist_head
{
    struct slist_head *next;
};

#define SLIST_HEAD_INIT(name)                                                  \
    {                                                                          \
        &(name)                                                                \
    }

#define SLIST_HEAD(name) struct slist_head name = SLIST_HEAD_INIT(name)

__BEGIN_DECLS

/// Инициализация заголовка односвязного списка.
static inline void slist_init(struct slist_head *head) { head->next = head; }

/// Возвращает истину, если за текущим элементом списка ничего нет.
static inline int slist_empty(struct slist_head *head)
{
    return head->next == head;
}

/**
 * Insert a new entry after the specified head.
 *
 * @param new: new entry to be added
 * @param head: list head to add it after
 */
static inline void slist_add(struct slist_head *link, struct slist_head *head)
{
    link->next = head->next;
    head->next = link;
}

///Отлинковать и вернуть первый элемент списка.
static inline struct slist_head *slist_pop_first(struct slist_head *head)
{
    struct slist_head *ret = head->next;
    if (ret == head)
        return NULL;
    head->next = ret->next;
    return ret;
}

__END_DECLS

#define slist_entry(ptr, type, member) mcast_out(ptr, type, member)

#define slist_first_entry(ptr, type, member)                                   \
    slist_entry((ptr)->next, type, member)

//Отлинковать и вернуть первый элемент списка как entry.
#define slist_pop_first_entry(ptr, type, member)                               \
    slist_entry(slist_pop_first(ptr), type, member)

#define slist_next_entry(pos, member)                                          \
    slist_entry((pos)->member.next, __typeof__(*(pos)), member)

#define slist_for_each(pos, head)                                              \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define slist_for_each_entry(pos, head, member)                                \
    for (pos = slist_first_entry(head, __typeof__(*pos), member);              \
         &pos->member != (head); pos = slist_next_entry(pos, member))

__BEGIN_DECLS

/// Вернуть количество элементов после текущего заголовка.
static inline int slist_size(const struct slist_head *head)
{
    int i = 0;
    struct slist_head *it;
    slist_for_each(it, head) { i++; }
    return i;
}

/// Выяснить, находится ли заголовок finded за head.
static inline int slist_in(const struct slist_head *head,
                           const struct slist_head *finded)
{
    struct slist_head *it;
    slist_for_each(it, head)
    {
        if (it == finded)
            return true;
    }
    return false;
}

/// Вывести информацию об элементах списка через систему dprint.
static inline void debug_print_slist_head(const struct slist_head *head)
{
    DPRINTPTR(head->next);
    DPRINT(slist_size(head));
}

__END_DECLS

#endif
