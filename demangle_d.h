#if !(DEMANGLE_D_H)
#define DEMANGEL_D_H 1

#define DD_(str) demangle_d_##str

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
char* DD_(demangle_d)(char* source);

#endif /* DEMANGEL_D_H */
