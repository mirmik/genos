#ifndef IGRIS_DLIST_H
#define IGRIS_DLIST_H

#include <igris/util/member.h>
#include <igris/util/memberxx.h>
#include <iterator>

namespace igris
{
    class dlist_base;

    class dlist_node
    {
        friend class dlist_base;

    public:
        dlist_node *next = nullptr;
        dlist_node *prev = nullptr;

    public:
        dlist_node *next_node() const
        {
            return next;
        }

        dlist_node *prev_node() const
        {
            return prev;
        }

    public:
        dlist_node() : next(this), prev(this) {}
        dlist_node(const dlist_node &) = delete;
        dlist_node &operator=(const dlist_node &) = delete;

        ~dlist_node()
        {
            unlink();
        }

        bool empty() const
        {
            return next == this;
        }

        template <class T, dlist_node T::*ptr> T &cast_out() const
        {
            return *member_container(this, ptr);
        }

        void move_prev_than(dlist_node *node)
        {
            unlink();
            auto *old_prev = node->prev;
            node->prev = this;
            old_prev->next = this;
            prev = old_prev;
            next = node;
        }

        void move_next_than(dlist_node *node)
        {
            unlink();
            auto *old_next = node->next;
            node->next = this;
            old_next->prev = this;
            next = old_next;
            prev = node;
        }

        bool is_linked() const
        {
            return next != this;
        }

        bool is_unlinked() const
        {
            return next == this;
        }

        void unlink();

        size_t circular_size() const
        {
            size_t sz = 0;
            const dlist_node *n = this;
            do
            {
                ++sz;
                n = n->next;
            } while (n != this);

            return sz;
        }

        size_t reverse_circular_size() const
        {
            size_t sz = 0;
            const dlist_node *n = this;
            do
            {
                ++sz;
                n = n->prev;
            } while (n != this);

            return sz;
        }
    };
    static_assert(sizeof(dlist_node) == 2 * sizeof(void *),
                  "dlist_node size is not equal to 2 * sizeof(void *)");

    class dlist_base
    {
        friend class dlist_node;

    protected:
        dlist_node list = {};

    public:
        dlist_base() = default;

    public:
        // move all nodes from other list to this list.
        void unlink_and_move_all_nodes_from_other(dlist_base &&oth)
        {
            list.unlink();
            list.next = oth.list.next;
            list.prev = oth.list.prev;
            list.next->prev = &list;
            list.prev->next = &list;
            oth.list.next = &oth.list;
            oth.list.prev = &oth.list;
        }

        dlist_node *first_node() const
        {
            return list.next_node();
        }

        dlist_node *last_node() const
        {
            return list.prev_node();
        }

        template <class T, dlist_node T::*ptr> T &first_entry() const
        {
            return list.next_node()->cast_out<T, ptr>();
        }

        template <class T, dlist_node T::*ptr> T &last_entry() const
        {
            return list.prev_node()->cast_out<T, ptr>();
        }

        size_t size() const
        {
            return list.circular_size() - 1;
        }

        void pop_node(dlist_node *node)
        {
            node->unlink();
        }

        void pop_front()
        {
            auto *node = list.next_node();
            pop_node(node);
        }

        void pop_back()
        {
            auto *node = list.prev_node();
            pop_node(node);
        }

        bool empty() const
        {
            return !list.is_linked();
        }

        ~dlist_base()
        {
            while (!empty())
                pop_front();
        }

        void move_next(dlist_node *node, dlist_node *head_node)
        {
            node->move_next_than(head_node);
        }

        void move_prev(dlist_node *node, dlist_node *head_node)
        {
            node->move_prev_than(head_node);
        }

        void move_front(dlist_node &node)
        {
            move_next(&node, &list);
        }

        void move_back(dlist_node &node)
        {
            move_prev(&node, &list);
        }
    };
    static_assert(sizeof(dlist_base) == sizeof(dlist_node),
                  "dlist_base size is not equal to dlist_node size");

    template <typename type, dlist_node type::*member>
    class dlist : public dlist_base
    {
    public:
        class iterator
        {
        public:
            using value_type = type;
            using difference_type = ptrdiff_t;
            using pointer = type *;
            using reference = type &;
            using iterator_category = std::bidirectional_iterator_tag;

        public:
            dlist_node *current;

        public:
            iterator() : current(nullptr){};
            iterator(dlist_node *head) : current(head){};
            iterator(const iterator &other) : current(other.current){};

            iterator operator++(int)
            {
                iterator i = *this;
                current = current->next_node();
                return i;
            }
            iterator &operator++()
            {
                current = current->next_node();
                return *this;
            }
            iterator operator--(int)
            {
                iterator i = *this;
                current = current->prev_node();
                return i;
            }
            iterator &operator--()
            {
                current = current->prev_node();
                return *this;
            }
            bool operator!=(const iterator &b) const
            {
                return current != b.current;
            }
            bool operator==(const iterator &b) const
            {
                return current == b.current;
            }

            type &operator*()
            {
                return *member_container(current, member);
            }
            type *operator->()
            {
                return member_container(current, member);
            }
        };

        class reverse_iterator
        {
        public:
            using value_type = type;
            using difference_type = ptrdiff_t;
            using pointer = type *;
            using reference = type &;
            using iterator_category = std::bidirectional_iterator_tag;

        private:
            dlist_node *current = nullptr;

        public:
            reverse_iterator(dlist_node *head) : current(head) {}

            reverse_iterator operator++(int)
            {
                reverse_iterator i = *this;
                current = current->prev;
                return i;
            }
            reverse_iterator &operator++()
            {
                current = current->prev;
                return *this;
            }
            reverse_iterator operator--(int)
            {
                reverse_iterator i = *this;
                current = current->next;
                return i;
            }
            reverse_iterator &operator--()
            {
                current = current->next;
                return *this;
            }
            bool operator!=(const reverse_iterator &b) const
            {
                return current != b.current;
            }
            bool operator==(const reverse_iterator &b) const
            {
                return current == b.current;
            }

            type &operator*()
            {
                return *member_container(current, member);
            }
            type *operator->()
            {
                return member_container(current, member);
            }
        };

        // METHODS:
        dlist() = default;

        void clear()
        {
            while (!empty())
                pop_front();
        }

        bool is_correct()
        {
            size_t circsize = list.circular_size();
            size_t revcircsize = list.reverse_circular_size();
            return circsize == revcircsize;
        }

        type &first()
        {
            return *member_container(list.next_node(), member);
        }

        type &front()
        {
            return *member_container(list.next_node(), member);
        }

        type &back()
        {
            return *member_container(list.prev_node(), member);
        }

        void move_next(type &obj, dlist_node *head_node)
        {
            auto *node = &(obj.*member);
            dlist_base::move_next(node, head_node);
        }

        void move_prev(type &obj, dlist_node *head_node)
        {
            auto *node = &(obj.*member);
            dlist_base::move_prev(node, head_node);
        }

        void move_next(type &obj, type &head)
        {
            auto head_node = &(head.*member);
            move_next(obj, head_node);
        }

        void move_prev(type &obj, type &head)
        {
            auto head_node = &(head.*member);
            move_prev(obj, head_node);
        }

        void move_next(type &obj, iterator head)
        {
            move_next(obj, *head);
        }

        void move_prev(type &obj, iterator head)
        {
            move_prev(obj, *head);
        }

        void move_front(type &obj)
        {
            move_next(obj, &list);
        }

        void move_back(type &obj)
        {
            move_prev(obj, &list);
        }

        void pop(type &obj)
        {
            auto *node = &(obj.*member);
            node->unlink();
        }

        void round_left()
        {
            auto *node = list.next;
            move_back(node);
        }

        iterator begin()
        {
            return iterator(list.next_node());
        }
        iterator end()
        {
            return iterator(&list);
        }

        iterator begin() const
        {
            return iterator((dlist_node *)list.next_node());
        }
        iterator end() const
        {
            return iterator((dlist_node *)&list);
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(list.prev);
        }
        reverse_iterator rend()
        {
            return reverse_iterator(&list);
        }
    };
}

#endif
