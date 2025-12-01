#ifndef IGRIS_PROTOCOL_NUMCMD_H
#define IGRIS_PROTOCOL_NUMCMD_H

#include <igris/compiler.h>
#include <stdint.h>

struct numcmd_record
{
    uint8_t num;
    uint32_t (*func)(uint32_t);
};

#define NUMCMD_TBLFIN                                                          \
    {                                                                          \
        0, NULL                                                                \
    }

__BEGIN_DECLS

int numcmd_checksum_check(const char *str);
int numcmd_do(const char *str, struct numcmd_record *table, uint32_t *ret);

void numcmd_form_checksum(char *str);

__END_DECLS

#endif
