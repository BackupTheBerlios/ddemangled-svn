#ifndef DEMANGLE_D_CONFIG_H
#define DEMANGLE_D_CONFIG_H 1

#include "demangle.h"

#undef DEMANGLE_D_REQUIRE_error
#undef DEMANGLE_D_REQUIRE_malloc
#undef DEMANGLE_D_REQUIRE_memmove
#undef DEMANGLE_D_REQUIRE_realloc
#undef DEMANGLE_D_REQUIRE_strndup
#undef DEMANGLE_D_REQUIRE_strtol_10
#undef DEMANGLE_D_REQUIRE_ISDIGIT
#undef DEMANGLE_D_REQUIRE_ISXDIGIT
#undef DEMANGLE_D_REQUIRE_ASCI2HEX

#ifdef DEMANGLE_D_IN_VALGRIND

/* valgrind - http://www.valgrind.org */

#include <stddef.h> /* size_t */

#define	xmemcpy		VG_(memcpy)
#define xmemmove	DD_(memmove)
#define DEMANGLE_D_REQUIRE_memmove 1
#define	xstrlen		VG_(strlen)
#define	xstrncmp	VG_(strncmp)
#define xstrndup	DD_(strndup)
#define DEMANGLE_D_REQUIRE_strndup 1
#define xmalloc		VG_(malloc)
#define xrealloc	VG_(realloc)
#define xfree		VG_(free)
#define xsnprintf	VG_(snprintf)
#define	xisdigit	ISDIGIT
#define DEMANGLE_D_REQUIRE_ISDIGIT 1
#define xisxdigit	ISXDIGIT
#define DEMANGLE_D_REQUIRE_ISXDIGIT 1
#define xasci2hex	ASCI2HEX
#define DEMANGLE_D_REQUIRE_ASCI2HEX 1

#define xstrtol_10	DD_(strtol_10)
#define DEMANGLE_D_REQUIRE_strtol_10 1

#elif defined(DEMANGLE_D_IN_GDB) /* not DEMANGLE_D_IN_VALGRIND */

/* gdb - http://www.gnu.org/software/gdb/ */

#include <stddef.h> /* size_t */

#include <string.h>
#define xmemcpy		memcpy
#define xmemmove	memmove
#define xstrlen		strlen
#define xstrncmp	strncmp
#if defined(__USE_GNU) || defined(_GNU_SOURCE)
#define xstrndup	strndup
#else
#define xstrndup	DD_(strndup)
#define DEMANGLE_D_REQUIRE_strndup 1
#endif

#include "../libiberty.h"
/* xmalloc */
/* xrealloc */

#include "../defs.h"
/* xfree */
/* xsnprintf */

#include "../../include/safe-ctype.h"
#define xisdigit	ISDIGIT
#define xisxdigit	ISXDIGIT

#define xasci2hex	ASCI2HEX
#define DEMANGLE_D_REQUIRE_ASCI2HEX 1

#define xstrtol_10	DD_(strtol_10)
#define DEMANGLE_D_REQUIRE_strtol_10 1

#else /* not DEMANGLE_D_IN_GDB */

/* "normal" libc */

#include <stddef.h> /* size_t */

#include <string.h>
#define	xmemcpy		memcpy
#define xmemmove	memmove
#define	xstrlen		strlen
#define	xstrncmp	strncmp
#if defined(__USE_GNU) || defined(_GNU_SOURCE)
#define xstrndup	strndup
#else
#define xstrndup	DD_(strndup)
#define DEMANGLE_D_REQUIRE_strndup 1
#endif

#include <stdlib.h>
#define xmalloc		DD_(malloc)
#define DEMANGLE_D_REQUIRE_malloc 1
#define xrealloc	DD_(realloc)
#define DEMANGLE_D_REQUIRE_realloc 1
#define xfree		free
#define xabort		abort
#define xstrtol_10(n,p)	strtol((n), (p), 10)

#include <stdio.h>
#define xsnprintf	snprintf

#include <ctype.h>
#define	xisdigit	isdigit
#define xisxdigit	isxdigit

#define xasci2hex	ASCI2HEX
#define DEMANGLE_D_REQUIRE_ASCI2HEX 1

#ifdef DEMANGLE_D_STANDALONE
#define DEMANGLE_D_REQUIRE_error 1
#define xprintf		printf
#define xperror		perror
#define xfprintf	fprintf
#endif

#endif /* not DEMANGLE_D_IN_VALGRIND && not DEMANGLE_D_IN_GDB */

/* helper macros */

#ifdef DEMANGLE_D_REQUIRE_ISDIGIT
#define ISDIGIT(c) (('0' <= (c)) && ((c) <= '9'))
#endif

#ifdef DEMANGLE_D_REQUIRE_ISXDIGIT
#define ISXDIGIT(c) ( \
		(('0' <= (c)) && ((c) <= '9')) \
		|| (('a' <= (c)) && ((c) <= 'f')) \
		|| (('A' <= (c)) && ((c) <= 'F')) \
		)
#endif

#ifdef DEMANGLE_D_REQUIRE_ASCI2HEX
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
#endif

#endif /* DEMANGLE_D_CONFIG_H */
