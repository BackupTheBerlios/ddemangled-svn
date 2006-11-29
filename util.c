#include "util.h"

#ifdef DEMANGLE_D_REQUIRE_memmove
/* Copy len bytes from memory area src to memory area dest. */
void * DD_(memmove)(dest, src, len)
	void *dest; const void *src; size_t len;
{
	void* tmp;
	char* a;
	const char* b;

	a = (char*) dest;
	b = (const char*) src;
	
	if(((a < b) && (a + len < b))
		|| (((b < a) && (b + len < a))))
	{
		return xmemcpy(dest, src, len);
	}else{
		tmp = xmalloc(len);
		xmemcpy(tmp, src, len);
		xmemcpy(dest, tmp, len);
		xfree(tmp);
		return dest;
	}
}
#endif

#ifdef DEMANGLE_D_REQUIRE_strndup
/* Copy len first bytes into a newly allocated buffer. */
char*
DD_(strndup)(source, len)
	const char* source; size_t len;
{
	char* dest;
	dest = xmalloc(len+1);
	xmemcpy(dest, source, len);
	dest[len] = '\x00';
	return dest;
}
#endif

#ifdef DEMANGLE_D_REQUIRE_strtol_10
/* Convert a string to a long integer using base 10. */
long int
DD_(strtol_10)(src, endptr)
	char* src; char** endptr;
{
	long int value;
	int sign;

	if(src[0] == '-'){
		sign = -1;
		src++;
	}else if(src[0] == '+'){
		sign = 1;
		src++;
	}else{
		sign = 1;
	}

	value = 0;

	while(xisdigit(src[0])){
		value = (value * 10) + (src[0] - '0');
		src++;
	}

	if(endptr){
		*endptr = src;
	}

	return sign * value;
}
#endif

#ifdef DEMANGLE_D_REQUIRE_malloc
void*
DD_(malloc)(len)
	size_t len;
{
	void* ptr;
	ptr = malloc(len);
	if(!ptr){
		error(NULL);
	}
	return ptr;
}
#endif

#ifdef DEMANGLE_D_REQUIRE_realloc
void*
DD_(realloc)(ptr, len)
	void* ptr; size_t len;
{
	ptr = realloc(ptr, len);
	if(!ptr){
		error(NULL);
	}
	return ptr;
}
#endif

#ifdef DEMANGLE_D_REQUIRE_error
void
DD_(error)(message)
	const char* message;
{
	if(message){
		xfprintf(stderr, message);
	}else{
		xperror("demangle_d");
	}
	xabort();
}
#endif
