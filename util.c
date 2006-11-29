/*
 * demangle_d - pluggable D de-mangler
 * Copyright (C) 2006 Thomas Kuehne <thomas@kuehne.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent modules,
 * and to copy and distribute the resulting executable under terms of your
 * choice, provided that you also meet, for each linked independent module,
 * the terms and conditions of the license of that module. An independent
 * module is a module which is not derived from or based on this library. If
 * you modify this library, you may extend this exception to your version of
 * the library, but you are not obligated to do so. If you do not wish to do
 * so, delete this exception statement from your version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "util.h"

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
		DD_(error)(NULL);
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
		DD_(error)(NULL);
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
