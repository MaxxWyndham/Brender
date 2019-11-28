#include "brender.h"
#include "dosio.h"
#include "sdlio.h"

br_pixelmap *DOSGfxBegin(const char *mode)
{
	WARN("(\"%s\"): only default MCGA and VESA are supported", mode);

	/* this is enough for now */
	if (strncmp(mode, "VESA", 4) == 0)
		return SDLGfxBegin(640, 480, 8, 0);

	/* default to standard 320x200x8 */
	/* Errol:  Changed default to 640x480 due to hardcoded glViewport value in brender.c */
	return SDLGfxBegin(640, 480, 8, 0);
}

void DOSGfxPaletteSet(br_pixelmap *palette)
{
	SDLGfxPaletteSet(palette);
}

void DOSGfxEnd()
{
	SDLGfxEnd();
}
