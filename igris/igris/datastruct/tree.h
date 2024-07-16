#ifndef IGRIS_DATASTRUCT_TREE_H
#define IGRIS_DATASTRUCT_TREE_H

#include <igris/datastruct/dlist.h>
#include <igris/util/macro.h>

struct tree_head
{
    struct dlist_head lnk;
    struct dlist_head childs;
    struct tree_head *parent;
};

#define TREE_HEAD_INIT(name)                                                   \
    {                                                                          \
        DLIST_HEAD_INIT(name.lnk), DLIST_HEAD_INIT(name.childs), NULL          \
    }

__BEGIN_DECLS

static inline void tree_init(struct tree_head *tree)
{
    dlist_init(&tree->lnk);
    dlist_init(&tree->childs);
    tree->parent = NULL;
}

static inline void tree_add_child(struct tree_head *node,
                                  struct tree_head *parent)
{
    dlist_add(&node->lnk, &parent->childs);
    node->parent = parent;
}

static inline void tree_add_child_tail(struct tree_head *node,
                                       struct tree_head *parent)
{
    dlist_add_tail(&node->lnk, &parent->childs);
    node->parent = parent;
}

__END_DECLS

#define tree_for_each_child(it, lst)                                           \
    for (it = dlist_first_entry(&lst->childs, struct tree_head, lnk),          \
        it != lst, it = dlist_next_entry(it, lnk))

#endif
