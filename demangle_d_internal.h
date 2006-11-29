#if !(DEMANGLE_D_INTERNAL_H)
#define DEMANGLE_D_INTERNAL_H 1

#include "demangle_d.h"

typedef struct{
	size_t	used;
	char*	str;
	size_t	len;
} DD_(String);

DD_(String)* DD_(new_String)(void);


void DD_(append_n)(DD_(String)* dest, const char* source, size_t len);
void DD_(append_c)(DD_(String)* dest, char source);
void DD_(append)(DD_(String)* dest, const char* source);

void DD_(prepend_n)(DD_(String)* dest, const char* source, size_t len);
void DD_(prepend)(DD_(String)* dest, const char* source);

void DD_(nestpend_n)(DD_(String)* dest, const char* source, size_t len, int is_nested);
void DD_(nestpend)(DD_(String)* dest, const char* source, int is_nested);

char* DD_(nextType)(DD_(String)* dest, char* raw, int is_nested);

unsigned char DD_(ascii2hex)(char c);

void DD_(interpreteTemplate)(DD_(String)* dest, char* raw);

char* DD_(parseReal)(DD_(String)* dest, char* raw);
char* DD_(parseFunction)(DD_(String)* dest, char* raw, char* name, int is_nested);

char* DD_(strndup)(const char* source, size_t len);
long int DD_(strtol_10)(char* src, char** endptr);

void* DD_(malloc)(size_t len);
void* DD_(realloc)(void* ptr, size_t len);

#endif /* DEMANGLE_D_INTERNAL_H */
