#ifndef DEMANGLE_D_PARSER_H
#define DEMANGLE_D_PARSER_H 1

#include "config.h"
#include "string.h"

char* DD_(nextType)(DD_(string_t)* dest, char* raw, int is_nested);

void DD_(interpreteTemplate)(DD_(string_t)* dest, char* raw);

char* DD_(parseReal)(DD_(string_t)* dest, char* raw);

char* DD_(parseFunction)(DD_(string_t)* dest, char* raw, char* name, int is_nested);

#endif /* DEMANGLE_D_PARSER_H */
