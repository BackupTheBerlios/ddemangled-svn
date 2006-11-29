/*
 * demangle_d.c - pluggable D de-mangler
 *
 * Copyright 2006 Thomas Kuehne <thomas@kuehne.cn>
 *
 *
 * Eiffel Forum License, version 1
 *
 * Permission is hereby granted to use, copy, modify and/or distribute this
 * package, provided that:
 *
 *  - copyright notices are retained unchanged
 *
 *  - any distribution of this package, whether modified or not, includes
 *    this file
 *
 * Permission is hereby also granted to distribute binary programs which
 * depend on this package, provided that:
 *
 *  - if the binary program depends on a modified version of this package,
 *    you must publicly release the modified version of this package
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT WARRANTY. ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE TO ANY PARTY FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ARISING IN ANY WAY
 * OUT OF THE USE OF THIS PACKAGE.
 *
 * 2006-04-19
 *	added GDB support
 * 
 * 2006-04-17
 * 	added Valgrind support
 *
 * 2006-04-16
 * 	inital public release
 *
 */

#include "demangle_d_conf.h"
#include "demangle_d_internal.h"

/* copy len first bytes into a newly allocated buffer */
char*
DD_(strndup)(const char* source, size_t len)
{
	char* dest;
	dest = xmalloc(len+1);
	xmemcpy(dest, source, len);
	dest[len] = '\x00';
	return dest;
}

#if (DEMANGLE_D_REQUIRE_strtol_10)
/* convert a string to a long integer using base 10 */
long int
xstrtol_10(char* src, char** endptr)
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

#if (DEMANGLE_D_REQUIRE_malloc)
void*
DD_(malloc)(size_t len)
{
	void* ptr;
	ptr = malloc(len);
	if(!ptr){
		perror("DD_(malloc)");
		xabort();
	}
	return ptr;
}
#endif

#if (DEMANGLE_D_REQUIRE_realloc)
void*
DD_(realloc)(void* ptr, size_t len)
{
	ptr = realloc(ptr, len);
	if(!ptr){
		perror("DD_(realloc)");
		xabort();
	}
	return ptr;
}
#endif

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

void
DD_(append_n)(DD_(string_t)* dest, const char* source, size_t len)
{
	size_t new_len = dest->used + len + 1;
	if(new_len > dest->len){
		dest->len = (size_t)(new_len * 1.5);
		dest->str = xrealloc(dest->str, dest->len);
	}
	xmemcpy(dest->str + dest->used, source, len);
	dest->used += len;
	dest->str[dest->used] = '\x00';
}

void
DD_(append_c)(DD_(string_t)* dest, char source)
{
	size_t new_len = dest->used + 2;
	if(new_len > dest->len){
		dest->len = (size_t)(new_len * 1.5);
		dest->str = xrealloc(dest->str, dest->len);
	}
	dest->str[dest->used++] = source;
	dest->str[dest->used] = '\x00';
}

void
DD_(append)(DD_(string_t)* dest, const char* source)
{
	DD_(append_n)(dest, source, xstrlen(source));
}

void
DD_(prepend_n)(DD_(string_t)* dest, const char* source, size_t len)
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
DD_(prepend)(DD_(string_t)* dest, const char* source)
{
	DD_(prepend_n)(dest, source, xstrlen(source));
}

void
DD_(nestpend_n)(DD_(string_t)* dest, const char* source, size_t len,
		int is_nested)
{
	if(is_nested){
		DD_(append_n)(dest, source, len);
	}else{
		DD_(prepend)(dest, " ");
		DD_(prepend_n)(dest, source, len);
	}
}

void
DD_(nestpend)(DD_(string_t)* dest, const char* source, int is_nested)
{
	DD_(nestpend_n)(dest, source, xstrlen(source), is_nested);
}

/* parse until the end of the next type */
char*
DD_(nextType)(DD_(string_t)* dest, char* source, int is_nested)
{
	if(!source || !source[0]){
		return NULL;
	}

	if((source[0] == '_') && (source[1] == 'D')
			&& xisdigit(source[2])
			&& (source[2] != '0'))
	{
		DD_(string_t)* tmp;
		tmp = DD_(new_string)();
		source = DD_(nextType)(tmp, source + 2, 0);
		if(dest->used && (dest->str[dest->used] != '.')){
			DD_(append_c)(dest, '.');
		}
		DD_(append_n)(dest, tmp->str, tmp->used);
		xfree(tmp->str);
		xfree(tmp);
		return source;
	}

	switch(source[0]){
		case 'v':
			DD_(nestpend)(dest, "void", is_nested);
			source += 1;
			break;
		case 'b': /* deprecated since DMD-0.148 (2006-02-25) */
			DD_(nestpend)(dest, "bit", is_nested);
			source += 1;
			break;
		case 'x':
			DD_(nestpend)(dest, "bool", is_nested);
			source += 1;
			break;
		case 'g':
			DD_(nestpend)(dest, "byte", is_nested);
			source += 1;
			break;
		case 'h':
			DD_(nestpend)(dest, "ubyte", is_nested);
			source += 1;
			break;
		case 's':
			DD_(nestpend)(dest, "short", is_nested);
			source += 1;
			break;
		case 't':
			DD_(nestpend)(dest, "ushort", is_nested);
			source += 1;
			break;
		case 'i':
			DD_(nestpend)(dest, "int", is_nested);
			source += 1;
			break;
		case 'k':
			DD_(nestpend)(dest, "uint", is_nested);
			source += 1;
			break;
		case 'l':
			DD_(nestpend)(dest, "long", is_nested);
			source += 1;
			break;
		case 'm':
			DD_(nestpend)(dest, "ulong", is_nested);
			source += 1;
			break;
		case 'f':
			DD_(nestpend)(dest, "float", is_nested);
			source += 1;
			break;
		case 'd':
			DD_(nestpend)(dest, "double", is_nested);
			source += 1;
			break;
		case 'e':
			DD_(nestpend)(dest, "real", is_nested);
			source += 1;
			break;
		case 'o':
			DD_(nestpend)(dest, "ifloat", is_nested);
			source += 1;
			break;
		case 'p':
			DD_(nestpend)(dest, "idouble", is_nested);
			source += 1;
			break;
		case 'j':
			DD_(nestpend)(dest, "ireal", is_nested);
			source += 1;
			break;
		case 'q':
			DD_(nestpend)(dest, "cfloat", is_nested);
			source += 1;
			break;
		case 'r':
			DD_(nestpend)(dest, "cdouble", is_nested);
			source += 1;
			break;
		case 'c':
			DD_(nestpend)(dest, "creal", is_nested);
			source += 1;
			break;
		case 'a':
			DD_(nestpend)(dest, "char", is_nested);
			source += 1;
			break;
		case 'u':
			DD_(nestpend)(dest, "wchar", is_nested);
			source += 1;
			break;
		case 'w':
			DD_(nestpend)(dest, "dchar", is_nested);
			source += 1;
			break;

		case 'A': /* dynamic array */
			if(!is_nested){
				DD_(prepend)(dest, "[] ");
			}
			source = DD_(nextType)(dest, source+1, is_nested);
			if(is_nested){
				DD_(append)(dest, "[]");
			}
			break;

		case 'G': { /* static array */
			char* start = ++source;
			char* end = start;
			while(xisdigit(*end)){
				end++;
			}
			if(!is_nested){
				DD_(prepend)(dest, "] ");
				DD_(prepend_n)(dest, start, end-start);
				DD_(prepend)(dest, "[");
			}
			source = DD_(nextType)(dest, end, is_nested);
			if(is_nested){
				DD_(append)(dest, "[");
				DD_(append_n)(dest, start, end-start);
				DD_(append)(dest, "]");
			}
			break;
		}

		case 'H': { /* associative array */
			DD_(string_t)* aa;
			aa = DD_(new_string)();
			source = DD_(nextType)(aa, source+1, 1);
			DD_(prepend)(aa, "[");
			DD_(append)(aa, "]");
			source = DD_(nextType)(aa, source, 0);

			DD_(nestpend)(dest, aa->str, is_nested);
			xfree(aa->str);
			xfree(aa);
			break;
		}

		case 'D': { /* delegate */
			DD_(string_t)* sig;
			sig = DD_(new_string)();
			source = DD_(parseFunction)(sig, source+1, NULL, 0);
			DD_(nestpend_n)(dest, sig->str, sig->used, is_nested);
			xfree(sig->str);
			xfree(sig);
			break;
		}

		case 'P': /* pointer */
			if((source[1] == 'F') || (source[1]=='U')
					|| (source[1]=='W') || (source[1]=='V')
					|| (source[1]=='R'))
			{
				/* function */
				DD_(string_t)* sig;
				sig = DD_(new_string)();
				source = DD_(parseFunction)(sig, source+1,
						"", 0);
				DD_(nestpend_n)(dest, sig->str, sig->used,
						is_nested);
				xfree(sig->str);
				xfree(sig);
			}else{
				/* 'normal' type */
				if(!is_nested){
					DD_(prepend)(dest, "* ");
				}
				source = DD_(nextType)(dest, source+1,
						is_nested);
				if(is_nested){
					DD_(append)(dest, " *");
				}
			}
			break;

		case 'J': /* out */
			DD_(append)(dest, "out ");
			source = DD_(nextType)(dest, source+1, 1);
			break;

		case 'K': /* inout */
			DD_(append)(dest, "inout ");
			source = DD_(nextType)(dest, source+1, 1);
			break;

		case 'C': /* class */
		case 'S': /* struct */
		case 'E': /* enum */
		case 'T': /* typedef */
		{
#if (DEMANGLE_D_VERBOSE)
			char tmp = source[0];
#endif /* DEMANGLE_D_VERBOSE */
			if(!is_nested){
				DD_(string_t)* sig;
				sig = DD_(new_string)();
				source = DD_(nextType)(sig, source+1, 0);
				DD_(append)(sig, " ");
#if (DEMANGLE_D_VERBOSE)
				switch(tmp){
					case 'C':
						DD_(prepend)(sig, "class ");
						break;
					case 'S':
						DD_(prepend)(sig, "struct ");
						break;
					case 'E':
						DD_(prepend)(sig, "enum ");
						break;
					case 'T':
						DD_(prepend)(sig, "typedef ");
						break;
				}
#endif /* DEMANGLE_D_VERBOSE */
				DD_(prepend_n)(dest, sig->str, sig->used);
				xfree(sig->str);
				xfree(sig);
			}else{
#if (DEMANGLE_D_VERBOSE)
				switch(tmp){
					case 'C':
						DD_(append)(dest, "class ");
						break;
					case 'S':
						DD_(append)(dest, "struct ");
						break;
					case 'E':
						DD_(append)(dest, "enum ");
						break;
					case 'T':
						DD_(append)(dest, "typedef ");
						break;
				}
#endif /* DEMANGLE_D_VERBOSE */
				source = DD_(nextType)(dest, source+1, 1);
			}
			break;
		}

		case '1': /* qualified name */
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			long int len;
			len = xstrtol_10(source, &source);
			if(len >= 5 && (source[0] == '_') && (source[1] == '_')
					&& (source[2] == 'T')
					&& xisdigit(source[3])
					&& (source[3] != '0'))
			{
				/* template */
				char* template = DD_(strndup)(source + 3, len-3);
				DD_(interpreteTemplate)(dest, template);
				xfree(template);
			}else{
				DD_(append_n)(dest, source, len);
			}
			source += len;
			while(source && xisdigit(source[0])
					&& (source[0] != '0'))
			{
				len = xstrtol_10(source, &source);
				DD_(append_c)(dest, '.');
				if(len >= 5 && (source[0] == '_')
						&& (source[1] == '_')
						&& (source[2] == 'T')
						&& xisdigit(source[3])
						&& (source[3] != '0'))
				{
					/* template */
					char* template = DD_(strndup)(source + 3, len-3);
					DD_(interpreteTemplate)(dest, template);
					xfree(template);
				}else if((len == 5) && (xstrncmp(source, "_ctor", len) == 0)){
					DD_(append)(dest, "this");
				}else if((len == 5) && (xstrncmp(source, "_dtor", len) == 0)){
					DD_(append)(dest, "~this");
				}else if((len == 11) && (xstrncmp(source, "_staticCtorFZv", len + 3) == 0)){
					DD_(prepend)(dest, "static void ");
					DD_(append)(dest, "this()");
					source = source + 11 + 3;
					break;
				}else if((len == 11) && (xstrncmp(source, "_staticDtorFZv", len + 3) == 0)){
					DD_(prepend)(dest, "static void ");
					DD_(append)(dest, "~this()");
					source = source + 11 + 3;
					break;
				}else{
					DD_(append_n)(dest, source, len);
				}
				source += len;
			}
			if(!is_nested){
				source = DD_(nextType)(dest, source, 0);
			}
			break;
		}

		case 'F': /* D function */
		case 'U': /* C function */
		case 'W': /* Windows function */
		case 'V': /* Pascal function */
		case 'R': /* C++ function */
			if(!is_nested){
				DD_(string_t)* id;
				id = DD_(new_string)();
				DD_(append_n)(id, dest->str, dest->used);
				dest->used = 0;
				dest->str[0] = '\x00';
				source = DD_(parseFunction)(dest, source, id->str, 0);
				xfree(id->str);
				xfree(id);
			}else{
				source = DD_(parseFunction)(dest, source, "", 1);
			}
			break;

		default:
			DD_(append)(dest, " @bug@[2]{");
			DD_(append)(dest, source);
			DD_(append)(dest, "}");
			source = NULL;
			break;
	}

	return source;
}

/* parse a "real" template parameter */
char*
DD_(parseReal)(DD_(string_t)* dest, char* source)
{
	/* @BUG@ architecture dependent */
	long double f;
	size_t i;
	int tmp;
	unsigned char* c = (unsigned char*) &f;
	char* buffer;

	for(i = 0; i < 10; i++){
		if(!xisxdigit(source[i * 2]) || !xisxdigit(source[i * 2 + 1])){
format_error:
			DD_(append)(dest, "0x");
			DD_(append_n)(dest, source, 20);
			return source + 20;
		}
		c[i] = (xasci2hex(source[i * 2]) << 4);
		c[i] |= xasci2hex(source[i * 2 + 1]);
	}

	buffer = xmalloc(64);
	tmp = xsnprintf(buffer, 64, "%Lf", f);
	if(tmp < 1){
		xfree(buffer);
		goto format_error;
	}
	DD_(append_n)(dest, buffer, tmp);
	xfree(buffer);
	return source + 20;
}

/* parse a function (including arguments and return type) */
char*
DD_(parseFunction)(DD_(string_t)* dest, char* source, char* name,
		int is_nested)
{
	DD_(string_t)* fn_return;
	DD_(string_t)* fn_param;

	fn_return = DD_(new_string)();
	fn_param = DD_(new_string)();

	source++;

	/* params */
	if(source[0] != 'Z'){
		if(source[0] == 'Y'){
			DD_(append)(fn_param, "...");
			goto var_arg_param;
		}
		source = DD_(nextType)(fn_param, source, 1);
		while(source && source[0] && source[0]!='Z'){
			if(source[0] == 'Y'){
				DD_(append)(fn_param, ", ...");
				goto var_arg_param;
			}else if(source[0] == 'X'){
				DD_(append)(fn_param, " ...");
				goto var_arg_param;
			}
			DD_(append)(fn_param, ", ");
			source = DD_(nextType)(fn_param, source, 1);
		}
	}

	/* return type */
	if(source && source[0] == 'Z'){
var_arg_param:
		source = DD_(nextType)(fn_return, source + 1, 1);
	}

	/* output */
	if(name && name[0]){
		if(! is_nested){
			DD_(prepend)(dest, " ");
			DD_(prepend_n)(dest, fn_return->str, fn_return->used);
			DD_(append)(dest, name);
		}else{
			DD_(append_n)(dest, fn_return->str, fn_return->used);
			DD_(append)(dest, " ");
			DD_(append)(dest, name);
		}
	}else if(name){
		DD_(append_n)(dest, fn_return->str, fn_return->used);
		DD_(append)(dest, " function");
	}else{
		DD_(append_n)(dest, fn_return->str, fn_return->used);
		DD_(append)(dest, " delegate");
	}

	if(fn_param->used){
		DD_(append)(dest, "(");
		DD_(append_n)(dest, fn_param->str, fn_param->used);
		DD_(append)(dest, ")");
	}else{
		DD_(append)(dest, "()");
	}

	xfree(fn_return->str);
	xfree(fn_return);
	xfree(fn_param->str);
	xfree(fn_param);

	return source;
}

/* interprete a NULL terminated template symbol */
void
DD_(interpreteTemplate)(DD_(string_t)* dest, char* raw)
{
	char* tmp;
	int first_arg = 1;
	long int dataLen;

	/* id */
	while(xisdigit(raw[0]) && (raw[0] != '0')){
		long int len;
		len = xstrtol_10(raw, &raw);
		DD_(append_n)(dest, raw, len);
		raw += len;
	}
	DD_(append)(dest, "!(");

	/* arguments */
	while(raw && raw[0]){
		if(raw[0] == 'T'){
			/* type parameter */
			raw++;
			if(!first_arg){
				DD_(append)(dest, ", ");
			}else{
				first_arg = 0;
			}
			raw = DD_(nextType)(dest, raw, 1);
		}else if(raw[0] == 'V'){
			/* value parameter */
			if(!first_arg){
				DD_(append)(dest, ", ");
			}else{
				first_arg = 0;
			}
			raw = DD_(nextType)(dest, raw + 1, 1);
			DD_(append)(dest, " ");
			if(xisdigit(raw[0])){
				/* integer */
integer_arg:
				tmp = raw;
				while(xisdigit(raw[0])){
					raw++;
				}
				DD_(append_n)(dest, tmp, raw-tmp);
			}else if(raw[0] == 'N'){
				/* negative integer */
				raw++;
				DD_(append)(dest, "-");
				goto integer_arg;
			}else if(raw[0] == 'e'){
				/* float */
				raw = DD_(parseReal)(dest, raw+1);
			}else if(raw[0] == 'c'){
				/* complex float */
				raw = DD_(parseReal)(dest, raw+1);
				DD_(append)(dest, " + ");
				raw = DD_(parseReal)(dest, raw);
				DD_(append)(dest, "i");
			}else if(raw[0] == 'n'){
				DD_(append)(dest, "null");
				raw++;
			}else if((raw[0] == 'a') || (raw[0] == 'w') || (raw[0] == 'd')){
				/* character literal */
				raw++;
				if(!xisdigit(raw[0])){
					goto bug;
				}
				dataLen = xstrtol_10(raw, &raw);
				if(raw[0] != '_'){
					goto bug;
				}
				raw++;
				DD_(append)(dest, "\"");
				while(dataLen--){
					if(xisxdigit(raw[0]) && xisxdigit(raw[1])){
						DD_(append_c)(dest, (xasci2hex(raw[0]) << 4)
								+ xasci2hex(raw[1]));
					}else{
						DD_(append_c)(dest, '?');
					}
					raw += 2;
				}
				DD_(append)(dest, "\"");
			}else{
				goto bug;
			}
		}else if(raw[0] == 'Z'){
			/* end of parameter list */
			break;
		}else{
bug:
			DD_(append)(dest, " @bug@[1]{");
			DD_(append)(dest, raw);
			DD_(append)(dest, "}");
			break;
		}
	}
	DD_(append)(dest, ")");
}

/* demangle a D symbol
 *
 * input:
 * 	a NULL terminated mangled symbol
 *
 * output:
 * 	UTF-8 encoded demangled symbol
 * 	or NULL if unable to demangle
 *
 * memory:
 * 	the caller is responsible to
 * 	free input and output
 */
static char*
DD_(demangle_d)(char* source)
{
	DD_(string_t)* dest;
	DD_(string_t)* nested;
	char* back;

	if((source[0] != '_') || (source[1] != 'D') || (!xisdigit(source[2]))
			|| (source[2] == '0'))
	{
		/* %% @BUG@ might be mangled with 'D' but hasn't 'D' linkage
		 * samples:
		 * _aaApply10treewalkerFPS3aaA3aaAZi
		 * _aaKeys9_aaKeys_xFPS3aaA3aaAZv
		 */
		return NULL;
	}else{
		source += 2;
	}

	dest = DD_(new_string)();

	source = DD_(nextType)(dest, source, 0);

	while(source && source[0]){
		/* nested symbols */
		nested = DD_(new_string)();
		DD_(append)(dest, ".");
		source = DD_(nextType)(nested, source, 0);
		DD_(append_n)(dest, nested->str, nested->used);
		xfree(nested->str);
		xfree(nested);
	}

	back = DD_(strndup)(dest->str, dest->used+1);
	xfree(dest->str);
	xfree(dest);

	return back;
}

#if (DEMANGLE_D_STANDALONE)
int
main(int argc, char** argv)
{
	int i;
	if(argc < 2){
		xfprintf(stderr,
			"pluggable D d-demangler by Thomas Kuehne <thomas@kuehne.cn> "
			"($Date$)\n");
		if(argc > 0){
			xfprintf(stderr, "%s <mangledSymbol_1> [<mangledSymbol_2> ...]\n", argv[0]);
		}else{
			xfprintf(stderr, "dmangle_d <mangledSymbol_1> [<mangledSymbol_2> ...]\n");
		}
		return (EXIT_FAILURE);
	}
	for(i = 1; i < argc; i++){
		char* demangled = DD_(demangle_d)(argv[i]);
		if(4 > xprintf("%s\t%s\n", argv[i], demangled)){
			xperror("main");
		}
		if(demangled){
			xfree(demangled);
		}
	}
	return (EXIT_SUCCESS);
}
#endif
