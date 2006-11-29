#if !(DEMANGLE_D_CONF_H)
#define DEMANGLE_D_CONF_H 1

#include "demangle_d.h"

#if (DEMANGLE_D_IN_VALGRIND)

/* valgrind - http://www.valgrind.org */

#include <stddef.h> /* size_t */

#define	xmemcpy		VG_(memcpy)
#define xmemmove	MEMMOVE
#define	xstrlen		VG_(strlen)
#define	xstrncmp	VG_(strncmp)
#define xmalloc		VG_(malloc)
#define DEMANGLE_D_REQUIRE_malloc 0
#define xrealloc	VG_(realloc)
#define DEMANGLE_D_REQUIRE_realloc 0
#define xfree		VG_(free)
#define xsnprintf	VG_(snprintf)
#define	xisdigit	ISDIGIT
#define xisxdigit	ISXDIGIT
#define xasci2hex	ASCI2HEX

#define DEMANGLE_D_REQUIRE_strtol_10 1
#define xstrtol_10	DD_(strtol_10)

#else

/* "normal" libc */
#define DEMANGLE_D_IN_VALGRIND 0

#include <string.h>
#define	xmemcpy		memcpy
#define xmemmove	memmove
#define	xstrlen		strlen
#define	xstrncmp	strncmp

#include <stdlib.h>
#define xmalloc		DD_(malloc)
#define DEMANGLE_D_REQUIRE_malloc 1
#define xrealloc	DD_(realloc)
#define DEMANGLE_D_REQUIRE_realloc 1
#define xfree		free
#define xabort		abort
#define xstrtol_10(n,p)	strtol((n), (p), 10)
#define DEMANGLE_D_REQUIRE_strtol_10 0

#include <stdio.h>
#define xsnprintf	snprintf

#include <ctype.h>
#define	xisdigit	isdigit
#define xisxdigit	isxdigit

#define xasci2hex	ASCI2HEX

#if (DEMANGLE_D_STANDALONE)
#define xprintf		printf
#define xperror		perror
#define xfprintf	fprintf
#else
#define DEMANGLE_D_STANDALONE 0
#endif

#endif

/* helper macros */

#define MEMMOVE(dest, src, len) \
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

#define ISDIGIT(c) (('0' <= (c)) && ((c) <= '9'))

#define ISXDIGIT(c) ( \
		(('0' <= (c)) && ((c) <= '9')) \
		|| (('a' <= (c)) && ((c) <= 'f')) \
		|| (('A' <= (c)) && ((c) <= 'F')) \
		)

#define ASCI2HEX(c) \
	( \
	 	('a' <= (c) && (c) <= 'f') \
		? \
		((c) - 'a' + 10) \
		: \
		( \
		 	('A' <= (c) && (c) <= 'F') \
			? \
			((c) - 'A' + 10) \
			: \
			( \
			 	('0' <= (c) && (c) <= '9') \
				? \
				((c) - '0') \
				: \
				0 \
			) \
		) \
	)

#endif /* DEMANGLE_D_CONF_H */
