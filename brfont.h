#include "compiler.h"

#ifndef _BRENDER_BRFONT_H_

#define BR_FONTF_PROPORTIONAL (1 << 0)

#pragma pack(push, 1)
typedef struct
{
	br_uint_32	flags;
	br_uint_16	glyph_x;
	br_uint_16	glyph_y;
	br_int_16	spacing_x;
	br_int_16	spacing_y;
	br_int_8	*width;
	br_uint_16	*encoding;
	br_uint_8	*glyphs;
} br_font;
#pragma pack(pop)

// preloaded fonts I presume
extern br_font *BrFontProp4x6;
extern br_font *BrFontProp7x9;

#define _BRENDER_BRFONT_H_
#endif
