#include "brender.h"
#include "sdlio.h"
#include "pixelmap.h"

#include <SDL.h>
#include <SDL_opengl.h>

br_pixelmap *SDLGfxBegin(int width, int height, int bpp, int fullscreen)
{
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_InitSubSystem(SDL_INIT_VIDEO);
	}

	WARN("Assuming 8 bit surface");
	br_pixelmap *screen = BrPixelmapAllocate(BR_PMT_INDEX_8, width, height, NULL, BR_PMAF_NORMAL);
	screen->flags = BR_PMAF_OFFSCREEN;

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	screen->device = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));

	if (screen->device) {
		INFO("Mode: %dx%dx%d", width, height, bpp);
		SDLGfxTitleSet("SDL BRender");
	}

	// hide cursor by default, app can show it if necessary
	SDL_ShowCursor(SDL_DISABLE);

	return screen;
}

void SDLGfxPaletteSet(br_pixelmap *palette)
{
	FIXME("(%p): stub", palette);
}

void SDLGfxTitleSet(const char *title)
{
	SDL_WM_SetCaption(title, title);
}

void SDLGfxEnd()
{
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
