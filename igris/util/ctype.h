#ifndef IGRIS_CTYPE_H
#define IGRIS_CTYPE_H

#include <igris/compiler.h>

static inline int igris_isxdigit_helper(int c) { 
	return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

static inline int igris_isblank(int c) { return c == ' ' || c == '\t'; }
static inline int igris_isspace(int c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v'; }
static inline int igris_isdigit(int c) { return c >= '0' && c <= '9'; }
static inline int igris_isxdigit(int c) { return igris_isdigit(c) || igris_isxdigit_helper(c); }
static inline int igris_isupper(int c) { return c >= 'A' && c <= 'Z'; }
static inline int igris_islower(int c) { return c >= 'a' && c <= 'z'; }
static inline int igris_isalpha(int c) { 
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static inline int igris_isalnum(int c) { 
	return igris_isalpha(c) || igris_isdigit(c); }


// TODO
static inline int igris_isprint(int c) 
{ 
	return igris_isalpha(c) || igris_isdigit(c) || 
		(c >= ' ' && c <= '~');
}

static inline int igris_toupper(int c) { return igris_islower(c) ? c + ('A'-'a') : c; }
static inline int igris_tolower(int c) { return igris_isupper(c) ? c + ('a'-'A') : c; }

#endif
