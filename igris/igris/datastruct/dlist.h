#ifndef IGRIS_DLIST_HEAD_H
#define IGRIS_DLIST_HEAD_H

/**
    @file
*/

#include <igris/compiler.h>
#include <igris/dprint.h>
#include <igris/util/member.h>

#include <igris/util/decltypeof.h>

/**
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define DLIST_POISON1 ((struct dlist_head *)(uintptr_t)0xDEADC0DE)
#define DLIST_POISON2 ((struct dlist_head *)(uintptr_t)0xDEADC9DE)

/** Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */
struct dlist_head
{
    struct dlist_head *next, *prev;
};

#ifdef __cplusplus
static_assert(sizeof(struct dlist_head) == 2 * sizeof(void *),
              "dlist_head size is not 2 * sizeof(void *)");
#else
_Static_assert(sizeof(struct dlist_head) == 2 * sizeof(void *),
               "dlist_head size is not 2 * sizeof(void *)");
#endif

#define DLIST_HEAD_INIT(name)                                                  \
    {                                                                          \
        &(name), &(name)                                                       \
    }

#define DLIST_HEAD(name) struct dlist_head name = DLIST_HEAD_INIT(name)

__BEGIN_DECLS

static inline int dlist_is_linked(struct dlist_head *head)
{
    return head->next != head;
}

/**
 * Init dlist head.
 *
 * It should be used before all operations with head
 * except dlist_add*,
 */
static inline void dlist_init(struct dlist_head *head)
{
    head->next = head->prev = head;
}

/**
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __dlist_add(struct dlist_head *lnk,
                               struct dlist_head *next,
                               struct dlist_head *prev)
{
    lnk->prev = prev;
    lnk->next = next;
    next->prev = lnk;
    prev->next = lnk;
}

/**
 * Insert a new entry after the specified head.
 * @param lnk: new entry to be added
 * @param head: list head to add it after
 */
static inline void dlist_add_next(struct dlist_head *lnk,
                                  struct dlist_head *head)
{
    __dlist_add(lnk, head->next, head);
}
#define dlist_add(a, b) dlist_add_next(a, b)

/**
 * Insert a new entry before the specified head.
 * @param lnk: new entry to be before
 * @param head: list head to add it after
 */
static inline void dlist_add_prev(struct dlist_head *lnk,
                                  struct dlist_head *head)
{
    __dlist_add(lnk, head, head->prev);
}
#define dlist_add_tail(a, b) dlist_add_prev(a, b)

/**
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __dlist_del(struct dlist_head *prev, struct dlist_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * Deletes entry from list.
 * @param entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void dlist_del(struct dlist_head *entry)
{
    __dlist_del(entry->prev, entry->next);
    entry->next = DLIST_POISON1;
    entry->prev = DLIST_POISON2;
}

/**
 * Deletes entry from list and reinitialize it.
 * @param entry: the element to delete from the list.
 */
static inline void dlist_del_init(struct dlist_head *entry)
{
    __dlist_del(entry->prev, entry->next);
    dlist_init(entry);
}

/**
 * Delete from one list and add as another's head
 * @param list: the entry to move
 * @param head: the head that will precede our entry
 */
static inline void dlist_move(struct dlist_head *list, struct dlist_head *head)
{
    __dlist_del(list->prev, list->next);
    dlist_add(list, head);
}

/**
 * Delete from one list and add as another's tail
 * @param list: the entry to move
 * @param head: the head that will follow our entry
 */
static inline void dlist_move_tail(struct dlist_head *list,
                                   struct dlist_head *head)
{
    __dlist_del(list->prev, list->next);
    dlist_add_tail(list, head);
}
#define dlist_move_prev(a, b) dlist_move_tail(a, b)

/**
    Добавляет элемент added перед элементом, который первым вернёт true на вызов
    comparator(added, pos).
 */
#define dlist_move_sorted(added, head, member, comparator)                     \
    {                                                                          \
        decltypeof(added) pos;                                                 \
        dlist_for_each_entry(pos, (head), member)                              \
        {                                                                      \
            if (comparator((added), pos))                                      \
                break;                                                         \
        }                                                                      \
        dlist_add_prev(&(added)->member, &pos->member);                        \
    }

/**
 * Tests whether a list is empty
 * @param head: the list to test.
 */
static inline int dlist_empty(const struct dlist_head *head)
{
    return head->next == head;
}

static inline void dlist_insert_instead(struct dlist_head *iter,
                                        struct dlist_head *instead)
{
    dlist_add_prev(iter, instead);
    dlist_del_init(instead);
}

__END_DECLS

#define dlist_entry(ptr, type, member) mcast_out(ptr, type, member)

#define dlist_next_entry(pos, member)                                          \
    dlist_entry((pos)->member.next, decltypeof(*(pos)), member)

#define dlist_prev_entry(pos, member)                                          \
    dlist_entry((pos)->member.prev, decltypeof(*(pos)), member)

#define dlist_first_entry(ptr, type, member)                                   \
    dlist_entry((ptr)->next, type, member)

#define dlist_last_entry(ptr, type, member)                                    \
    dlist_entry((ptr)->prev, type, member)

#define dlist_for_each(pos, head)                                              \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define dlist_for_each_reverse(pos, head)                                      \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define dlist_for_each_safe(pos, n, head)                                      \
    for (pos = (head)->next, n = pos->next; pos != (head);                     \
         pos = n, n = pos->next)

#define dlist_for_each_entry(pos, head, member)                                \
    for (pos = dlist_first_entry(head, decltypeof(*pos), member);              \
         &pos->member != (head);                                               \
         pos = dlist_next_entry(pos, member))

#define dlist_for_each_entry_reverse(pos, head, member)                        \
    for (pos = dlist_last_entry(head, decltypeof(*pos), member);               \
         &pos->member != (head);                                               \
         pos = dlist_prev_entry(pos, member))

#define dlist_for_each_entry_safe(pos, n, head, member)                        \
    for (pos = dlist_first_entry(head, decltypeof(*pos), member),              \
        n = dlist_next_entry(pos, member);                                     \
         &pos->member != (head);                                               \
         pos = n, n = dlist_next_entry(n, member))

__BEGIN_DECLS

static inline int dlist_in(struct dlist_head *fnd, struct dlist_head *head)
{
    struct dlist_head *it;
    dlist_for_each(it, head)
    {
        if (it == fnd)
            return 1;
    }
    return 0;
}

static inline int dlist_check(struct dlist_head *fnd, int count)
{
    int steps = 0;
    struct dlist_head *it = fnd;

    while (count--)
    {
        struct dlist_head *next = it->next;

        if (fnd == next)
            return steps;

        it = next;
        steps++;
    }

    return -1;
}

static inline int dlist_check_reversed(struct dlist_head *fnd, int count)
{
    int steps = 0;
    struct dlist_head *it = fnd;

    while (count--)
    {
        struct dlist_head *prev = it->prev;

        if (fnd == prev)
            return steps;

        it = prev;
        steps++;
    }

    return -1;
}

static inline void dlist_debug_print_node(const char *prefix,
                                          struct dlist_head *node)
{
    debug_print(prefix);
    debug_printhex_ptr(node);
    debug_print(" (next: ");
    debug_printhex_ptr(node->next);
    debug_print(", prev: ");
    debug_printhex_ptr(node->prev);
    debug_print_line(")");
}

static inline void dlist_debug_print(struct dlist_head *head)
{
    struct dlist_head *it;

    dlist_debug_print_node("head: ", head);
    dlist_for_each(it, head)
    {
        dlist_debug_print_node("node: ", it);
    }
}

/// Выясняет количество нодов в кольце за исключением этого.
static inline int dlist_size(struct dlist_head *head)
{
    struct dlist_head *it;
    int sz = 0;

    dlist_for_each(it, head)
    {
        ++sz;
    }

    return sz;
}

/// Выясняет количество нодов в кольце за исключением этого,
/// осуществляя проход в обратном направлении.
static inline int dlist_size_reversed(struct dlist_head *head)
{
    struct dlist_head *it;
    int sz = 0;

    dlist_for_each_reverse(it, head)
    {
        ++sz;
    }

    return sz;
}

static inline bool dlist_is_correct(struct dlist_head *head)
{
    int check = dlist_check(head, 1000);
    if (check < 0)
        return false;
    int rcheck = dlist_check_reversed(head, 1000);
    if (rcheck < 0)
        return false;
    return check == rcheck;
}

__END_DECLS

#endif
