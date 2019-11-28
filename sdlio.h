#include "pixelmap.h"

br_pixelmap *SDLGfxBegin(int width, int height, int bpp, int fullscreen);
void SDLGfxPaletteSet(br_pixelmap *);
void SDLGfxTitleSet(const char *);
void SDLGfxEnd();
