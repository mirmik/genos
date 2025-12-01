#ifndef GXX_DATASTRUCT_TRENT_H
#define GXX_DATASTRUCT_TRENT_H

/**
    Pure C trent implementation.
    trent - is script-language like object.
    It can be integer, numerical, string, dictionary, list or nil type.
*/

#include <gxx/datastruct/buf.h>
#include <gxx/datastruct/dlist.h>
#include <gxx/panic.h>

#include <stdbool.h>
#include <cstring>

enum TrentType
{
    TRENT_NIL = 0,
    TRENT_NUM = 1,
    TRENT_STR = 2,
    TRENT_PTR = 3,

    TRENT_TABL = 8,
    TRENT_LIST = 9,  // TABL type
    TRENT_DICT = 10, // TABL type
};

#define TRENT_TABL_MASK 0b1000

struct __trent_buffer
{
    char *data;
    size_t size;
}

struct trent_node
{
    struct dlist_head lnk;
    // struct trent_node* 	parent;
    struct cbuf_s name;
    uint8_t type;

    union
    {
        void *m_ptr;
        long double m_num;
        struct __trent_buffer m_str;
        struct dlist_head m_childs;
    };
};

__BEGIN_DECLS

static inline void trent_init(struct trent_node *tr, struct trent_node *parent)
{
    dlist_init(&tr->lnk);
    tr->parent = parent;
}

static inline void trent_init_nil(struct trent_node *tr,
                                  struct trent_node *parent)
{
    trent_init(tr, parent);
    tr->type = TRENT_NIL;
}

static inline void trent_init_bool(struct trent_node *tr,
                                   struct trent_node *parent, bool val)
{
    trent_init(tr, parent);
    tr->type = TRENT_BOOL;
    tr->m_int = val;
}

static inline void trent_init_int(struct trent_node *tr,
                                  struct trent_node *parent, long long val)
{
    trent_init(tr, parent);
    tr->type = TRENT_INT;
    tr->m_int = val;
}

static inline void trent_init_num(struct trent_node *tr,
                                  struct trent_node *parent, long double val)
{
    trent_init(tr, parent);
    tr->type = TRENT_NUM;
    tr->m_num = val;
}

static inline void trent_init_str(struct trent_node *tr,
                                  struct trent_node *parent, const char *data,
                                  unsigned int size)
{
    trent_init(tr, parent);
    tr->type = TRENT_STR;
    tr->m_str.data = data;
    tr->m_str.size = size;
}

static inline void trent_init_tabl(struct trent_node *tr,
                                   struct trent_node *parent, uint8_t tableType)
{
    assert(tableType & TRENT_TABL_MASK);
    trent_init(tr, parent);
    tr->type = tableType;
    dlist_init(&tr->m_childs);
}

static inline void trent_setname(struct trent_node *tr, const char *data,
                                 unsigned int size)
{
    tr->name.data = data;
    tr->name.size = size;
}

static inline int trent_addchild(struct trent_node *parent,
                                 struct trent_node *child)
{
    assert(parent->type & TRENT_TABL_MASK);
    dlist_add_next(&child->lnk, &parent->m_childs);
}

static inline int trent_equal(struct trent_node *a, struct trent_node *b)
{
    if (a->type != b->type)
        return 0;

    switch (a->type)
    {
    case TRENT_NIL:
        return true;
    case TRENT_INT:
    case TRENT_BOOL:
        return a->m_int == b->m_int;
    case TRENT_NUM:
        return a->m_num == b->m_num;
    case TRENT_STR:
        return memcmp(a->m_str.data, b->m_str.data,
                      a->m_str.size < b->m_str.size ? a->m_str.size
                                                    : b->m_str.size);
    default:
        PANIC_TRACED("undefined operation");
    }
}

void trent_unlink(struct trent_node *node);
void trent_dealloc(struct trent_node *node, void (*free)(void *));

__END_DECLS

#endif
