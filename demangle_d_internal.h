#if !(DEMANGLE_D_INTERNAL_H)
#define DEMANGLE_D_INTERNAL_H 1

#include "demangle_d.h"

typedef struct{
	size_t	used;
	char*	str;
	size_t	len;
} DD_(string_t);

DD_(string_t)* DD_(new_string)(void);

void DD_(append_n)(DD_(string_t)* dest, const char* source, size_t len);
void DD_(append_c)(DD_(string_t)* dest, char source);
void DD_(append)(DD_(string_t)* dest, const char* source);

void DD_(prepend_n)(DD_(string_t)* dest, const char* source, size_t len);
void DD_(prepend)(DD_(string_t)* dest, const char* source);

void DD_(nestpend_n)(DD_(string_t)* dest, const char* source, size_t len, int is_nested);
void DD_(nestpend)(DD_(string_t)* dest, const char* source, int is_nested);

char* DD_(nextType)(DD_(string_t)* dest, char* raw, int is_nested);

void DD_(interpreteTemplate)(DD_(string_t)* dest, char* raw);

char* DD_(parseReal)(DD_(string_t)* dest, char* raw);
char* DD_(parseFunction)(DD_(string_t)* dest, char* raw, char* name, int is_nested);

char* DD_(strndup)(const char* source, size_t len);

#if (DEMANGLE_D_REQUIRE_strtol_10)
long int DD_(strtol_10)(char* src, char** endptr);
#endif

#if (DEMANGLE_D_REQUIRE_malloc)
void* DD_(malloc)(size_t len);
#endif

#if (DEMANGLE_D_REQUIRE_realloc)
void* DD_(realloc)(void* ptr, size_t len);
#endif

#endif /* DEMANGLE_D_INTERNAL_H */
