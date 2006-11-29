#ifndef DEMANGLE_D_STRING_H
#define DEMANGLE_D_STRING_H 1

#include "config.h"

typedef struct{
	size_t	used;
	char*	str;
	size_t	len;
} DD_(string_t);

DD_(string_t)* DD_(new_string)(void);


void DD_(append_n)(DD_(string_t)* dest, const char* source, size_t len);

void DD_(append_c)(DD_(string_t)* dest, int source);

void DD_(append)(DD_(string_t)* dest, const char* source);


void DD_(prepend_n)(DD_(string_t)* dest, const char* source, size_t len);

void DD_(prepend)(DD_(string_t)* dest, const char* source);


void DD_(nestpend_n)(DD_(string_t)* dest, const char* source, size_t len,
		int is_nested);

void DD_(nestpend)(DD_(string_t)* dest, const char* source, int is_nested);

#endif /* DEMANGLE_D_STRING_H */
