#include <igris/dprint.h>
#include <igris/osutil/realtime.h>
#include <pthread.h>

int this_thread_set_realtime_priority(void)
{
#ifndef __WIN32__
    int ret;

    pthread_t this_thread = pthread_self();
    struct sched_param params;

    params.sched_priority = 30;

    if ((ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params)))
    {
        dprln("realtime: setshedparam error");
        return ret;
    }

    int policy = 0;
    if ((ret = pthread_getschedparam(this_thread, &policy, &params)))
    {
        dprln("realtime: getshedparam error");
        return ret;
    }

    if (policy != SCHED_FIFO)
    {
        dprln("realtime: policy is not setted");
        return -1;
    }

    return 0;
#else
    return -1;
#endif
}
