#ifndef GSTUFF_V1_BASE_H
#define GSTUFF_V1_BASE_H

#include <igris/compiler.h>

#define GSTUFF_START_V1 ((char)0xAC)
#define GSTUFF_STUB_V1 ((char)0xAD)
#define GSTUFF_STUB_START_V1 ((char)0xAE)
#define GSTUFF_STUB_STUB_V1 ((char)0xAF)

__BEGIN_DECLS

/**
    Собрать пакет gstuff.

    @param data - входной буффер
    @param size - длина входного буффер
    @param outdata - выходной буффер (рекомендованная длина 2*size+2)
    @return результирующая длина пакета.
 */
int gstuffing_v1(char *data, int size, char *outdata);

__END_DECLS

#endif
