#ifndef GENOS_STRHASH_H
#define GENOS_STRHASH_H

static inline unsigned int HashH37(const char * str)
{
	unsigned int hash = 2139062143;
	for(; *str; str++)
		hash = 37 * hash + *str;
	return hash;
}

#endif