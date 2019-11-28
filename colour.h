#include <stdint.h>

#ifndef _BRENDER_COLOUR_H_

typedef uint32_t br_colour;

#define BR_COLOUR_RGB(r, g, b) (br_colour)( r | (g << 8) | (b << 16) )
#define BR_COLOUR_RGBA(r,g,b,a) (br_colour)( r | (g << 8) | (b << 16) | (a << 24) )

#define BR_RED(c) ((c >> 16) & 0xFF)
#define BR_GRN(c) ((c >> 8) & 0xFF)
#define BR_BLU(c) ((c) & 0xFF)
#define BR_ALPHA(c) ((c >> 24) & 0xFF)

#define _BRENDER_COLOUR_H_
#endif
