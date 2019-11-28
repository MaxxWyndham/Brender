#include "compiler.h"
#include "brfont.h"

#ifndef _BRENDER_PIXELMAP_H_

#define BR_PMAF_NORMAL			(1 << 0)
#define BR_PMAF_INVERTED		(1 << 1)
#define BR_PMAF_ROW_WHOLEPIXELS	(1 << 2)
#define BR_PMAF_ALLOCATED		(1 << 4)
#define BR_PMAF_NO_ACCESS		(1 << 5)
#define BR_PMAF_DEPTHBUFFER		(1 << 6)
#define BR_PMAF_OFFSCREEN		(1 << 7)

#define BR_PMF_NO_ACCESS  		BR_PMAF_NO_ACCESS
#define BR_PMF_ROW_WHOLEPIXELS  BR_PMAF_ROW_WHOLEPIXELS

#define BR_MAPU_ALL			(1 << 0)
#define BR_TABU_ALL			(1 << 0)

typedef struct br_pixelmap br_pixelmap;

enum {
	BR_PMT_INDEX_1,
	BR_PMT_INDEX_2,
	BR_PMT_INDEX_4,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_555,
	BR_PMT_RGB_565,
	BR_PMT_RGB_888,
	BR_PMT_RGBX_888,
	BR_PMT_RGBA_8888,
	BR_PMT_DEPTH_16,
	BR_PMT_DEPTH_32
};

enum {
	BR_PMMATCH_OFFSCREEN,
	BR_PMMATCH_DEPTH_16
};

struct br_pixelmap
{
	// br_list expects identifier to be the first member
	char *identifier;

	br_uint_8 type;
	br_uint_8 flags;

	br_int_16 origin_x;
	br_int_16 origin_y;

	br_uint_16 width;
	br_uint_16 height;

	/* these are 1/2 pre-calculated dimensions for some reason */
	br_uint_16 width12;
	br_uint_16 height12;

	void *pixels;
	br_int_16 row_bytes;
	br_uint_16 base_x;
	br_uint_16 base_y;
	br_pixelmap *map;

	void *user;
	void *device;

	/* 2D render backing texture */
	br_uint_32 *tex32;

	/* for sub pixelmaps, can only hold one for now */
	br_pixelmap *sub;
};

br_pixelmap* BrPixelmapAllocate(br_uint_8 type, br_uint_16 w, br_uint_16 h, void* pixels, int flags);
br_pixelmap* BrPixelmapAllocateSub(br_pixelmap* pm, br_uint_16 x, br_uint_16 y, br_uint_16 w, br_uint_16 h);
br_uint_32 BrPixelmapFileCount(const char* filename, br_uint_16* num);
br_pixelmap *BrPixelmapLoad(const char* filename);
br_uint_32 BrPixelmapLoadMany(const char* filename, br_pixelmap** pixelmaps, br_uint_16 num);
br_pixelmap *BrPixelmapMatch(const br_pixelmap* src, int match_type);
void BrPixelmapLine(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour);
/* plot is not documented */
void BrPixelmapPlot(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_32 colour);
void BrPixelmapCopy(br_pixelmap* dst, const br_pixelmap* src);
void BrPixelmapRectangleCopy (br_pixelmap* dst, br_int_16 dx, br_int_16 dy, const br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h);
void BrPixelmapFill(br_pixelmap* dat, br_uint_32 colour);
void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_16 w, br_uint_16 h, br_uint_32 colour);
void BrPixelmapText(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_32 colour, const br_font* font, const char* text);
void BrPixelmapTextF(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_32 colour, const br_font* font, const char* fmt, ...);
br_uint_16 BrPixelmapTextWidth(const br_pixelmap* dst, const br_font* font, const char* text);
void BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src);
void BrPixelmapFree(br_pixelmap* pmap);
br_uint_32 BrPixelmapSave(const char* filename, const br_pixelmap* pixelmap);

br_pixelmap* BrMapAdd(br_pixelmap* pixelmap);
br_uint_32 BrMapAddMany(br_pixelmap* const* pixelmaps, int n);
br_pixelmap* BrMapFind(const char* pattern);
void BrMapUpdate(br_pixelmap* pixelmap, br_uint_16 flags);
br_pixelmap* BrMapRemove(br_pixelmap* pixelmap);

typedef br_uint_32 BR_CALLBACK br_table_enum_cbfn(br_pixelmap*, void*);

br_pixelmap* BrTableAdd(br_pixelmap* pixelmap);
br_uint_32 BrTableAddMany(br_pixelmap* const* pixelmaps, int n);
void BrTableUpdate(br_pixelmap* pixelmap, br_uint_16 flags);
br_pixelmap* BrTableFind(const char* pattern);
br_pixelmap* BrTableRemove(br_pixelmap* pixelmap);
br_uint_32 BrTableEnum(const char* pattern, br_table_enum_cbfn* callback, void* arg);

#define _BRENDER_PIXELMAP_H_
#endif
