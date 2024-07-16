#ifndef IGRIS_SLIST_H
#define IGRIS_SLIST_H

#include <igris/datastruct/slist.h>
#include <igris/util/member.h>
#include <igris/util/memberxx.h>

namespace igris
{

    template <typename type, slist_head type::*member> class slist
    {
        slist_head head;

    public:
        slist()
        {
            head.next = &head;
        }

        bool empty()
        {
            return head.next == &head;
        }

        void add_first(type &obj)
        {
            slist_add(&(obj.*member), &head);
        }

        void move_front(type &obj)
        {
            (obj.*member).next = head.next;
            head.next = &(obj.*member);
        }

        class iterator
        {
        public:
            // using iterator_category = std::bidirectional_iterator_tag;
            using value_type = type;
            using difference_type = ptrdiff_t;
            using pointer = type *;
            using reference = type &;

        public:
            slist_head *current;

        public:
            iterator() : current(nullptr){};
            iterator(slist_head *head) : current(head){};
            iterator(const iterator &other) : current(other.current){};

            iterator operator++(int)
            {
                iterator i = *this;
                current = current->next;
                return i;
            }
            iterator operator++()
            {
                current = current->next;
                return *this;
            }
            bool operator!=(const iterator &b)
            {
                return current != b.current;
            }
            bool operator==(const iterator &b)
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

        class const_iterator
        {
        public:
            // using iterator_category = std::bidirectional_iterator_tag;
            using value_type = type;
            using difference_type = ptrdiff_t;
            using pointer = type *;
            using reference = type &;

        public:
            slist_head *current;

        public:
            const_iterator() : current(nullptr){};
            const_iterator(slist_head *head) : current(head){};
            const_iterator(const const_iterator &other)
                : current(other.current){};

            const_iterator operator++(int)
            {
                iterator i = *this;
                current = current->next;
                return i;
            }
            const_iterator operator++()
            {
                current = current->next;
                return *this;
            }
            bool operator!=(const const_iterator &b)
            {
                return current != b.current;
            }
            bool operator==(const const_iterator &b)
            {
                return current == b.current;
            }

            const type &operator*()
            {
                return *member_container(current, member);
            }
            const type *operator->()
            {
                return member_container(current, member);
            }
        };

        iterator begin()
        {
            return iterator(head.next);
        }
        iterator end()
        {
            return iterator(&head);
        }

        const_iterator begin() const
        {
            return const_iterator(head.next);
        }
        const_iterator end() const
        {
            return const_iterator(&head);
        }
    };
}

#endif
