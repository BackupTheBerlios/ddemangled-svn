#ifndef DEMANGLE_D_DEMANGLE_H
#define DEMANGLE_D_DEMANGLE_H 1

#ifdef __cplusplus
extern "C" {
#endif

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
char* DD_(demangle_d)(char*);

#ifdef __cplusplus
}
#endif

#endif /* DEMANGLE_D_DEMANGLE_H */
