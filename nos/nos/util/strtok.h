#ifndef NOS_STRTOK_H
#define NOS_STRTOK_H

#include <string.h>

static inline 
char *nos_strtok_r(char *str, const char *delim, char **saveptr) {
	char ch;

	if (str == NULL && (NULL == (str = *saveptr))) {
		return NULL;
	}

	/* search first not delimiting character */
	do {
		if ('\0' == (ch = *str++)) {
			return NULL;
		}
	} while(strchr(delim, ch));

	/* skip not delimiting characters */
	*saveptr = str + strcspn(str, delim);

	if (**saveptr != '\0') { /* if string is not finished */
		**saveptr = 0; /* we must insert zero instead of separator */
		(*saveptr)++;
	}

	return --str;
}

#endif
