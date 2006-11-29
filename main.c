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

#ifdef DEMANGLE_D_STANDALONE
#include "parser.h"
#include "util.h"

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
#endif /* DEMANGLE_D_STANDALONE */
