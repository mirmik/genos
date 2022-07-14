#ifndef GENOS_RESOURCE_NAMESPACE_H
#define GENOS_RESOURCE_NAMESPACE_H

#include <igris/compiler.h>
#include <igris/datastruct/dlist.h>

struct namespace_head;
struct file_head;

struct namespace_operations
{
    int (*lookup)(struct namespace_head *ns,
                  struct file_head **filpp,
                  const char *path);
};

struct namespace_head
{
    const char *path;
    struct dlist_head lnk_namespace_list;
    const struct namespace_operations *ns_ops;
};

__BEGIN_DECLS

void namespace_init(struct namespace_head *head,
                    const char *path,
                    const struct namespace_operations *ops);

void namespace_deinit(struct namespace_head *head);

struct namespace_head *namespace_lookup(const char *path,
                                        const char **internal_path);

__END_DECLS

#endif