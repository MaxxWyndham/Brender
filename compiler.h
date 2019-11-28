#include <stdint.h>
#include <string.h>

#ifndef _BRENDER_COMPILER_H_

#define BR_TRUE		1
#define BR_FALSE	0

/* guessing stdcall */
#define BR_CALLBACK __stdcall
#define BR_ASM_CALL __stdcall
#define BR_PUBLIC_ENTRY __stdcall

#define BR_ASM_DATA

// x64 has only one calling convention
#if defined(__x86_64__) || defined(linux)
	#ifndef __stdcall
		#define __stdcall
	#endif
	#ifndef __cdecl
		#define __cdecl
	#endif
#endif

typedef int8_t		br_int_8;
typedef int16_t		br_int_16;
typedef int32_t		br_int_32;

typedef uint8_t		br_uint_8;
typedef uint16_t	br_uint_16;
typedef uint32_t	br_uint_32;

typedef float		br_float;

typedef size_t		br_size_t;

// this is a custom type I added to fix 64 bit issues
typedef intptr_t	br_intptr_t;

// custom type for opengl related activities -errol
typedef double      br_double;

#define _BRENDER_COMPILER_H_
#endif
