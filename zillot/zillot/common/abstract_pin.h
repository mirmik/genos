#ifndef ZILLOT_COMMON_ABSTRACT_PIN_H
#define ZILLOT_COMMON_ABSTRACT_PIN_H

namespace zillot
{
    class abstract_pin
    {
    public:
        virtual void write(int val) const = 0;
        virtual int read() const = 0;
        virtual void toggle() const = 0;
        virtual void pull_up(int lvl) const = 0;
        virtual int setup(uint32_t mode) const = 0;

        void set(int val) const
        {
            write(val);
        }
        int get() const
        {
            return read();
        }
    };
}

#endif