#ifndef DEMANGLE_D_CONF_H
#define DEMANGLE_D_CONF_H 1

#ifdef DEMANGLE_D_IN_VALGRIND
/* valgrind - http://www.valgrind.org */

#include <stddef.h> /* size_t */

#define	xmemcpy		VG_(memcpy)
#define xmemmove	y_memmove
#define	xstrlen		VG_(strlen)
#define	xstrncmp	VG_(strncmp)
#define xmalloc		VG_(malloc)
#define xrealloc	VG_(realloc)
#define xfree		VG_(free)
#define xsnprintf	VG_(snprintf)
#define	xisdigit	y_isdigit
#define xisxdigit	y_isxdigit

#else

/* "normal" libc */

#include <string.h>
#define	xmemcpy		memcpy
#define xmemmove	memmove
#define	xstrlen		strlen
#define	xstrncmp	strncmp

#include <stdlib.h>
#define xmalloc		malloc
#define xrealloc	realloc
#define xfree		free

#include <stdio.h>
#define xsnprintf	snprintf

#include <ctype.h>
#define	xisdigit	isdigit
#define xisxdigit	isxdigit

#ifdef DEMANGLE_D_STANDALONE
#define xprintf		printf
#endif

#endif

/* helpers for odd system */

#define y_memmove(dest, src, len) \
{ \
	if(((dest < src) && (dest + len < src)) \
		|| (((src < dest) && (src + len < dest)))) \
	{ \
		xmemcpy(dest, src, len); \
	}else{ \
		void* tmp; \
		tmp = xmalloc(len); \
		xmemcpy(tmp, src, len); \
		xmemcpy(dest, tmp, len); \
		xfree(tmp); \
	} \
}

#define y_isdigit(c) (('0' <= (c)) && ((c) <= '9'))

#define y_isxdigit(c) ( (('0' <= (c)) && ((c) <= '9')) \
		|| (('a' <= (c)) && ((c) <= 'f')) \
		|| (('A' <= (c)) && ((c) <= 'F')))


#endif /* DEMANGLE_D_CONF_H */
