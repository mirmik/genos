#ifndef GXX_GSTUFF_CAUTOMATE_H
#define GXX_GSTUFF_CAUTOMATE_H

#include <igris/compiler.h>

#include <igris/protocols/gstuff_v1/autorecv.h>
#include <igris/protocols/gstuff_v1/gstuff.h>

#include <igris/datastruct/sline.h>
#include <igris/util/crc.h>

#define GSTUFF_CRC_ERROR_V1 -1
#define GSTUFF_OVERFLOW_V1 -2
#define GSTUFF_DATA_ERROR_V1 -3
#define GSTUFF_NEWPACKAGE_V1 1
#define GSTUFF_CONTINUE_V1 0

struct gstuff_autorecv_v1
{
    struct sline line;
    uint8_t crc;
    uint8_t state;
    // void(*callback)(void*,int,char*,int);
    // void* callback_argument;
};

__BEGIN_DECLS

void gstuff_autorecv_reset_v1(struct gstuff_autorecv_v1 *autom);

void gstuff_autorecv_setbuf_v1(struct gstuff_autorecv_v1 *autom, void *buf,
                               int len);

void gstuff_autorecv_init_v1(struct gstuff_autorecv_v1 *autom,
                             void (*callback)(void *, int sts, char *dat,
                                              int len),
                             void *arg);

int gstuff_autorecv_newchar_v1(struct gstuff_autorecv_v1 *autom, char c);

__END_DECLS

#endif
