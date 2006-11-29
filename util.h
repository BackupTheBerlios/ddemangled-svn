#ifndef DEMANGLE_D_UTIL_H
#define DEMANGLE_D_UTIL_H 1

#include "config.h"

#ifdef DEMANGLE_D_REQUIRE_strndup
char* DD_(strndup)(const char*, size_t);
#endif

#ifdef DEMANGLE_D_REQUIRE_strtol_10
long int DD_(strtol_10)(char*, char**);
#endif

#ifdef DEMANGLE_D_REQUIRE_malloc
void* DD_(malloc)(size_t);
#endif

#ifdef DEMANGLE_D_REQUIRE_realloc
void* DD_(realloc)(void*, size_t);
#endif

#ifdef DEMANGLE_D_REQUIRE_memmove
void * DD_(memmove)(void*, const void *, size_t );
#endif

#ifdef DEMANGLE_D_REQUIRE_error
void DD_(error)(const char*);
#endif

#endif /* DEMANGLE_D_UTIL_H */
