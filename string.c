#include "config.h"
#include "string.h"
#include "util.h"

/* create a new string */
DD_(string_t)*
DD_(new_string)(void)
{
	DD_(string_t)* str = xmalloc(sizeof(DD_(string_t)));
	str->used = 0;
	str->len = 128;
	str->str = xmalloc(str->len);
	str->str[0] = '\x00';
	return str;
}

/* append len bytes from source to dest */
void
DD_(append_n)(dest, source, len)
	DD_(string_t)* dest; const char* source; size_t len;
{
	size_t new_len;
	new_len = dest->used + len + 1;
	if(new_len > dest->len){
		dest->len = (size_t)(new_len * 1.5);
		dest->str = xrealloc(dest->str, dest->len);
	}
	xmemcpy(dest->str + dest->used, source, len);
	dest->used += len;
	dest->str[dest->used] = '\x00';
}

void
DD_(append_c)(dest, source)
	DD_(string_t)* dest; int source;
{
	size_t new_len = dest->used + 2;
	if(new_len > dest->len){
		dest->len = (size_t)(new_len * 1.5);
		dest->str = xrealloc(dest->str, dest->len);
	}
	dest->str[dest->used++] = (char)source;
	dest->str[dest->used] = '\x00';
}

void
DD_(append)(dest, source)
	DD_(string_t)* dest; const char* source;
{
	DD_(append_n)(dest, source, xstrlen(source));
}

void
DD_(prepend_n)(dest, source, len)
	DD_(string_t)* dest; const char* source; size_t len;
{
	size_t new_len = dest->used + len + 1;
	if(new_len > dest->len){
		dest->len = (size_t)(new_len * 1.5);
		dest->str = xrealloc(dest->str, dest->len);
	}
	if(dest->used){
		xmemmove(dest->str + len, dest->str, dest->used);
	}
	xmemcpy(dest->str, source, len);
	dest->used += len;
	dest->str[dest->used] = '\x00';
}

void
DD_(prepend)(dest, source)
	DD_(string_t)* dest; const char* source;
{
	DD_(prepend_n)(dest, source, xstrlen(source));
}

void
DD_(nestpend_n)(dest, source, len, is_nested)
	DD_(string_t)* dest; const char* source; size_t len; int is_nested;
{
	if(is_nested){
		DD_(append_n)(dest, source, len);
	}else{
		DD_(prepend)(dest, " ");
		DD_(prepend_n)(dest, source, len);
	}
}

void
DD_(nestpend)(dest, source, is_nested)
	DD_(string_t)* dest; const char* source; int is_nested;
{
	DD_(nestpend_n)(dest, source, xstrlen(source), is_nested);
}
