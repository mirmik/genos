#ifndef IGRIS_SCHMIDT_H
#define IGRIS_SCHMIDT_H

struct incremental_schmidt
{
    int lo;
    int hi;
    int maxim;
    int cur;
    uint8_t state;

    incremental_schmidt(int lo, int hi, int maxim, bool start_state)
        : lo(lo), hi(hi), maxim(maxim), cur(start_state ? maxim : 0),
          state(start_state)
    {
    }

    int check(int val)
    {
        cur += val;

        if (cur < 0)
            cur = 0;
        if (cur > maxim)
            cur = maxim;

        if (state == 0)
        {
            if (cur < hi)
                return 0;

            else
            {
                state = 1;
                return 1;
            }
        }
        else
        {
            if (cur > lo)
                return 1;

            else
            {
                state = 0;
                return 0;
            }
        }
    };
};

#endif
