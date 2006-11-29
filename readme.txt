 pluggable D de-mangler
=======================

Author:
	Thomas Kuehne <thomas@kuehne.cn>

License:
	Eiffel Forum License, version 1 (see license.txt)


Usage: stand alone
---------------------

1) compile:
	cc -DDEMANGLE_D_STANDALONE -o demangle_d demangle_d.c
	
2) run:
	./demangle_d _D3std3utf6toUTF8FG4awZAa _D3std6string7sformatFAaYAa
	> _D3std3utf6toUTF8FG4awZAa       char[] std.utf.toUTF8(char[4], dchar)
	> _D3std6string7sformatFAaYAa     char[] std.string.sformat(char[], ...)


Usage: plugin
---------------------

1) adapt demangle_d_conf.h to your system

2) include demangle_d.c in your build system

3) demangle:
	#include "demangle_d.h"

	char* mangled;
	char* demangled;
	
	mangled = "_D1b5outerFeZf";
	demangled = DD_(demangle_d)(mangled);

	// ... process demangled ...

	if(demangled){
		free(demangled);
	}

optional:
	adapt the DD_(str) macro in "demangle_d.h" to resolve potential
	name clashes in your object code

	
TODO
---------------------

	* better output for symbols in nested types:
		_staticCtorFZv3Ali5_ctorFiZC_D1b11_staticCtorFZv3Ali5innerFiZi
	
	* handle "special" names:
		_init__D1b11_staticCtorFZv3Al
		_Class__D1b11_staticCtorFZv3Ali
		_vtbl__D1b11_staticCtorFZv3Ali
		_modctor_1b
		_assert_1b

