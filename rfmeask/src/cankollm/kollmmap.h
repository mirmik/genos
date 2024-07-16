#ifndef KOLLMMAP_H
#define KOLLMMAP_H

#include <map>
#include <string>

enum class KollmType
{
    INVALID = 0,
    INTEGER8 = 1,
    INTEGER16 = 2,
    INTEGER32 = 3,
    UNSIGNED8 = 4,
    UNSIGNED16 = 5,
    UNSIGNED32 = 6,
    ARRAY = 7,
};

enum KollmAccess
{
    ro,
    rw,
    w
};

struct kollm_param
{
    const char *mnem;
    KollmType type;
    KollmAccess access;
    uint16_t index;
    uint8_t subindex;
};

extern std::map<std::string, kollm_param> kollmmap;
void kollmmap_init();
uint8_t kollm_wcode(KollmType tp);

#endif
