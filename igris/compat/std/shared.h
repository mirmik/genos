#ifndef STD_SHARED_PTR_H
#define STD_SHARED_PTR_H

#include <memory>
#include <utility>

namespace std
{

    template <class T> class shared_ptr
    {
        struct aux
        {
            unsigned count;
            aux() : count(1) {}
            virtual void destroy() = 0;
            virtual ~aux() {} // must be polymorphic
        };

        template <class Deleter> struct auximpl : public aux
        {
            T *p;
            Deleter d;
            auximpl(T *pu, Deleter x) : p(pu), d(x) {}
            virtual void destroy()
            {
                d(p);
            }
        };

        struct auximpl_integrated : public aux
        {
            T obj;
            virtual void destroy() {}
        };

        struct default_deleter
        {
            void operator()(T *p) const
            {
                delete p;
            }
        };

        aux *pa;
        T *pt;

        void inc()
        {
            if (pa)
                ++pa->count;
        }

        void dec()
        {
            if (pa && !--pa->count)
            {
                pa->destroy();
                delete pa;
            }
        }

    public:
        shared_ptr() : pa(), pt() {}

        template <class U, class Deleter>
        shared_ptr(U *pu, Deleter d) : pa(new auximpl<Deleter>(pu, d)), pt(pu)
        {
        }

        template <class U>
        explicit shared_ptr(U *pu)
            : pa(new auximpl<default_deleter>(pu, default_deleter())), pt(pu)
        {
        }

        shared_ptr(const shared_ptr &s) : pa(s.pa), pt(s.pt)
        {
            inc();
        }

        template <class U>
        shared_ptr(const shared_ptr<U> &s) : pa(s.pa), pt(s.pt)
        {
            inc();
        }

        ~shared_ptr()
        {
            dec();
        }

        shared_ptr &operator=(const shared_ptr &s)
        {
            if (this != &s)
            {
                dec();
                pa = s.pa;
                pt = s.pt;
                inc();
            }
            return *this;
        }

        T *operator->() const
        {
            return pt;
        }
        T &operator*() const
        {
            return *pt;
        }

        template <class Alloc, class... Args>
        friend shared_ptr<T> allocate_shared(const Alloc &alloc,
                                             Args &&... args);
        template <class... Args>
        friend shared_ptr<T> make_shared(Args &&... args);
    };

    template <class T, class Alloc, class... Args>
    shared_ptr<T> allocate_shared(const Alloc &alloc, Args &&... args)
    {
    }

    template <class T, class... Args> shared_ptr<T> make_shared(Args &&... args)
    {
        shared_ptr<T> ptr;
        auto *zip = std::allocator<typename shared_ptr<T>::auximpl_integrated>()
                        .allocate(1);
        new (&zip->obj) T(std::forward<Args>(args)...);
        ptr->pa = zip;
        ptr->pa = &zip->obj;
        return ptr;
    }
}

#endif
