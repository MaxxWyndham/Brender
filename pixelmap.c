#include "brender.h"
#include "brfont.h"
#include "pixelmap.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <math.h>

br_pixelmap *BrPixelmapAllocate(br_uint_8 type, br_uint_16 w, br_uint_16 h, void* pixels, int flags)
{
	br_pixelmap *ret = BrResAllocate(NULL, sizeof(br_pixelmap), BR_MEMORY_PIXELMAP);

	ret->flags = flags;
	ret->type = type;
	ret->width = w;
	ret->height = h;
	ret->width12 = BR_DIV(w, 2);
	ret->height12 = BR_DIV(h, 2);
	ret->origin_x = 0;
	ret->origin_y = 0;
	ret->base_x = 0;
	ret->base_y = 0;

	if (ret->width % 2 == 0 && ret->width12 % 2 == 0)
	{
		ret->row_bytes = w;
	}
	else
	{
		ret->row_bytes = (ret->width12 + (ret->width12 % 2 == 0 ? 2 : 1)) * 2;
	}

	switch (ret->type)
	{
	case BR_PMT_INDEX_1:
	case BR_PMT_INDEX_2:
	case BR_PMT_INDEX_4:
	case BR_PMT_INDEX_8:
		ret->row_bytes *= 1;
		break;

	case BR_PMT_RGBX_888:
		ret->row_bytes *= 4;
		break;

	default:
		BR_FATAL1("Unhandled pixel format %d", ret->type);
	}

	if (pixels == NULL)
	{
		ret->pixels = BrResAllocate(ret, (ret->row_bytes * h), BR_MEMORY_PIXELS);
		ret->flags |= BR_PMAF_ALLOCATED;
	}
	else
	{
		ret->pixels = pixels;
	}

	return ret;
}

br_uint_32 BrPixelmapFileCount(const char* filename, br_uint_16* num)
{
	*num = 0;
	br_uint_32 s = BrFileGetLength(filename);
	br_uint_32 ret = 1;

	if (s > 16)
	{
		FILE *fp;
		fp = BrFileOpenRead(filename, 0, NULL, 0);
		char *buffer_start = BrMemCalloc(s, sizeof(char), BR_MEMORY_SCRATCH);
		char *buffer = buffer_start;
		BrFileRead(buffer, sizeof(char), s, fp);
		BrFileClose(fp);

		/* magic numbers are magic */
		if (BrMemReadBU32(&buffer) == 18 &&
			BrMemReadBU32(&buffer) == 8 &&
			BrMemReadBU32(&buffer) == 2 &&
			BrMemReadBU32(&buffer) == 2)
		{
			br_uint_32 pn = (br_uint_32)buffer + s - 16;
			br_uint_32 blocktype;
			br_uint_32 blocksize;

			while ((br_uint_32)buffer < pn)
			{
				blocktype = BrMemReadBU32(&buffer);
				blocksize = BrMemReadBU32(&buffer);

				switch (blocktype)
				{
				case 3:
					//*num += 1;
					BrMemSkipBytes(&buffer, 11);
					BrMemReadString(&buffer);
					break;

				case 61:
					//*num += 1;
					BrMemSkipBytes(&buffer, 13);
					BrMemReadString(&buffer);
					break;

				case 0:
					*num += 1;
				case 33:
					BrMemSkipBytes(&buffer, blocksize);
					break;

				default:
					INFO("%s:", filename);
					INFO("unknown block %u (len %u) @ %u", blocktype, blocksize, (br_uint_32)buffer);
					break;
				}
			}

			ret = 0;
		}

		BrMemFree(buffer_start);
		buffer_start = NULL;
		buffer = NULL;
	}

	return ret;
}

br_pixelmap *BrPixelmapLoad(const char* filename)
{
	br_pixelmap *pmap;

	if (BrPixelmapLoadMany(filename, &pmap, 1))
	{
		return pmap;
	}
	else
	{
		//WARN("%s missing or corrupt", filename);
		return NULL;
	}
}

br_pixelmap *BrPixelmapMatch(const br_pixelmap* src, int match_type)
{
	br_pixelmap *ret = BrPixelmapAllocate(src->type, src->width, src->height, NULL, BR_PMAF_NORMAL);

	if (match_type == BR_PMMATCH_DEPTH_16)
	{
		ret->type = BR_PMT_DEPTH_16;
		ret->flags |= BR_PMAF_DEPTHBUFFER;
	}
	else
	{
		ret->flags |= BR_PMAF_OFFSCREEN;
	}

	return ret;
}

void BrPixelmapFill(br_pixelmap* dat, br_uint_32 colour)
{
	BrBlockFill(dat->pixels, colour, (dat->row_bytes * dat->height));

	if (dat->flags & BR_PMAF_OFFSCREEN)
	{
		//FIXME("Use BR_RED/GRN/BLU to set glClearColor");
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	else if (dat->flags & BR_PMAF_DEPTHBUFFER)
	{
		//FIXME("Set GL_DEPTH_BUFFER_BIT");
		glClearDepth(1);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

void BrPixelmapDoubleBuffer(br_pixelmap* dst, br_pixelmap* src)
{
	if (!dst->device || !dst->map)
		return;

	/* FIXME : Correct perspective matrix fucks this up */
	uint8_t		*data = src->pixels;
	uint32_t	*palette = dst->map->pixels;

	float		src_ar = (float)src->width / src->height,
		dst_ar = (float)dst->width / dst->height;

	int			real_w = src->width,
		real_h = src->height,
		tex_w = 1024,
		tex_h = 1024;

	float		scale_w = 1.0f,
		scale_h = 1.0f,
		off_h = 0.0f,
		off_w = 0.0f;

	if (dst_ar < src_ar)
	{
		real_w = (float)src->width;
		real_h = (float)src->width / dst_ar;

	}
	else if (dst_ar > src_ar)
	{
		real_w = (float)src->height * dst_ar;
		real_h = (float)src->height;
	}

	scale_w = (float)real_w / tex_w;
	scale_h = (float)real_h / tex_h;

	off_h = (scale_h - scale_h * ((float)src->height / real_h)) / 2;
	off_w = (scale_w - scale_w * ((float)src->width / real_w)) / 2;

	if (dst->tex32 == NULL)
	{
		// allocate 2D rendering backing texture
		dst->tex32 = BrResAllocate(dst, tex_w * tex_h * sizeof(br_uint_32), BR_MEMORY_PIXELS);
	}

	if (src->width > tex_w || src->height > tex_h)
	{
		WARN("source image bigger than texture, ignoring");
		return;
	}

	if (!palette)
	{
		WARN("no palette, can't render");
		return;
	}

	for (int x = 0; x < src->width; x++)
	{
		for (int y = 0; y < src->height; y++)
		{
			br_uint_32 p = palette[data[y * src->width + x]];

			// FIXME: this is incorrect and causes menus to leak
			if (p == 0)
				dst->tex32[y * tex_w + x] = 0;
			else
				dst->tex32[y * tex_w + x] = p | 0xFF000000;
		}
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, src->width, src->height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst->tex32);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(1024, 1024, 0);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1024, 0, 0);
	glTexCoord2f(0.0f, 0.0f);	glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(0, 1024, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	SDL_GL_SwapBuffers();
}

void BrPixelmapFree(br_pixelmap* pmap)
{
	//INFO("%p->%s", pmap, pmap->identifier);
	if (pmap->identifier)
		BrMemFree(pmap->identifier);
	if (pmap->pixels && pmap->flags & BR_PMAF_ALLOCATED)
		BrMemFree(pmap->pixels);
	if (pmap->sub)
		BrPixelmapFree(pmap->sub);
	BrMemFree(pmap);
	//	if (pmap->tex32)
	//	{
	//		BrResFree(pmap->tex32);
	//	}
	//
	//	if (pmap->pixels && pmap->free_pixels)
	//	{
	//		BrResFree(pmap->pixels);
	//	}
	//
	//	BrResFree(pmap);
}

br_pixelmap* BrPixelmapAllocateSub(br_pixelmap* pm, br_uint_16 x, br_uint_16 y, br_uint_16 w, br_uint_16 h)
{
	if (pm->sub)
		FIXME("this pixelmap already has a sub! Implement this properly!");
	return (pm->sub = BrPixelmapAllocate(pm->type, w, h, NULL, pm->flags));
}

br_uint_32 BrPixelmapLoadMany(const char* filename, br_pixelmap** pixelmaps, br_uint_16 num)
{
	if (BrPixelmapFileCount(filename, &num) > 0) { return 0; }

	if (num > 0)
	{
		br_uint_32 s = BrFileGetLength(filename);

		if (s)
		{
			FILE *fp;
			fp = BrFileOpenRead(filename, 0, NULL, 0);
			BrFileAdvance(16, fp); // First 16 bytes are magic number
			s -= 16;

			char *buffer_start = BrMemCalloc(s, sizeof(char), BR_MEMORY_DATAFILE);
			char *buffer = buffer_start;
			BrFileRead(buffer, sizeof(char), s, fp);
			BrFileClose(fp);

			br_uint_32 pn = (br_uint_32)buffer + s;
			br_uint_32 blocktype;
			br_uint_32 blocksize;
			br_uint_32 i = 0;

			br_uint_8   type;
			br_uint_16  rowsize;
			br_uint_16  width;
			br_uint_16  height;
			br_uint_16  halfwidth;
			br_uint_16  halfheight;

			br_uint_32 pixelcount;
			br_uint_32 pixelsize;

			br_pixelmap *parent = 0;

			while ((br_uint_32)buffer < pn)
			{
				blocktype = BrMemReadBU32(&buffer);
				blocksize = BrMemReadBU32(&buffer);

				switch (blocktype)
				{
				case 3:
					type = BrMemReadU8(&buffer);
					rowsize = BrMemReadBU16(&buffer);
					width = BrMemReadBU16(&buffer);
					height = BrMemReadBU16(&buffer);
					halfwidth = BrMemReadBU16(&buffer);
					halfheight = BrMemReadBU16(&buffer);

					if (parent) {
						parent->sub = BrPixelmapAllocate(type, width, height, NULL, 0);
						parent->sub->row_bytes = rowsize;
						parent->sub->width12 = halfwidth;
						parent->sub->height12 = halfheight;
						parent->sub->identifier = BrResStrDup(parent->sub, BrMemReadString(&buffer));
					}
					else {
						pixelmaps[i] = BrPixelmapAllocate(type, width, height, NULL, 0);
						pixelmaps[i]->row_bytes = rowsize;
						pixelmaps[i]->width12 = halfwidth;
						pixelmaps[i]->height12 = halfheight;
						pixelmaps[i]->identifier = BrResStrDup(pixelmaps[i], BrMemReadString(&buffer));

						parent = &pixelmaps[i];
					}
					break;

				case 33:
					pixelcount = BrMemReadBU32(&buffer);
					pixelsize = BrMemReadBU32(&buffer);
					BrBlockCopy(pixelmaps[i]->pixels, buffer, blocksize - 8);
					BrMemSkipBytes(&buffer, blocksize - 8);

					if (type == 7)
					{
						br_uint_32 *pal = (br_uint_32 *)pixelmaps[i]->pixels;

						for (int j = 0; j < 256; j++)
						{
							pal[j] = pal[j] >> 8;
						}
					}
					break;

				case 61:
					type = BrMemReadU8(&buffer);
					rowsize = BrMemReadBU16(&buffer);
					width = BrMemReadBU16(&buffer);
					height = BrMemReadBU16(&buffer);
					BrMemSkipBytes(&buffer, 6);

					pixelmaps[i] = BrPixelmapAllocate(type, width, height, NULL, 0);
					pixelmaps[i]->row_bytes = rowsize;
					pixelmaps[i]->identifier = BrResStrDup(pixelmaps[i], BrMemReadString(&buffer));
					break;

				case 0:
					i++;
					parent = NULL;
					break;

				case 34:
					break;

				default:
					INFO("%s:", filename);
					INFO("unknown block %u (len %u) @ %u", blocktype, blocksize, (br_uint_32)buffer);
					break;
				}
			}

			BrMemFree(buffer_start);
			buffer_start = NULL;
			buffer = NULL;
		}
	}

	return num;
}

void BrPixelmapLine(br_pixelmap* dst, br_int_16 x1, br_int_16 y1, br_int_16 x2, br_int_16 y2, br_uint_32 colour)
{
	/* thank you, internet */
	br_int_16 dx, dy, inx, iny, e;
	dx = x2 - x1;
	dy = y2 - y1;
	inx = dx > 0 ? 1 : -1;
	iny = dy > 0 ? 1 : -1;
	dx = abs(dx);
	dy = abs(dy);
	if (dx >= dy) {
		dy <<= 1;
		e = dy - dx;
		dx <<= 1;
		while (x1 != x2) {
			BrPixelmapPlot(dst, x1, y1, colour);
			if (e >= 0) {
				y1 += iny;
				e -= dx;
			}
			e += dy; x1 += inx;
		}
	}
	else {
		dx <<= 1;
		e = dx - dy;
		dy <<= 1;
		while (y1 != y2) {
			BrPixelmapPlot(dst, x1, y1, colour);
			if (e >= 0) {
				x1 += inx;
				e -= dy;
			}
			e += dx; y1 += iny;
		}
	}
	BrPixelmapPlot(dst, x1, y1, colour);
}

void BrPixelmapPlot(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_32 colour)
{
	br_uint_8 *dst_pix = (br_uint_8 *)dst->pixels;
	dst_pix[(y*dst->row_bytes) + x] = (br_uint_8)colour;
}

void BrPixelmapCopy(br_pixelmap* dst, const br_pixelmap* src)
{
	FIXME("(): stub");
}

void BrPixelmapRectangleCopy(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, const br_pixelmap* src, br_int_16 sx, br_int_16 sy, br_uint_16 w, br_uint_16 h)
{
	br_uint_8 *src_pix = (br_uint_8 *)src->pixels;
	br_uint_8 *dst_pix = (br_uint_8 *)dst->pixels;

	if (src->type != dst->type)
	{
		WARN("src and dst types don't match! src is %d and dst is %d", src->type, dst->type);
		return;
	}

	if (src->type != BR_PMT_INDEX_8)
	{
		WARN("only 8 bit surfaces supported");
		return;
	}

	//	if (dx + w > dst->width)
	//		w = dst->width - dx;
	//
	//	if (dy + h > dst->height)
	//	{
	//		//INFO("Was %u now %u", h, (dst->height - dy));
	//		h = dst->height - dy;
	//	}

	for (int x = 0; x < w; x++)
	{
		if (dx + x < 0 || dx + x >= dst->width) { continue; }
		if (sx + x >= src->width) continue;

		for (int y = 0; y < h; y++)
		{
			if (dy + y < 0 || dy + y >= dst->height) { continue; }
			if (sy + y >= src->height) continue;

			dst_pix[(y + dy) * dst->row_bytes + (x + dx)] = src_pix[(sy + y) * src->row_bytes + (x + sx)];
		}
	}
}

void BrPixelmapRectangleFill(br_pixelmap* dst, br_int_16 dx, br_int_16 dy, br_uint_16 w, br_uint_16 h, br_uint_32 colour)
{
	br_uint_8 *dst_pix = (br_uint_8 *)dst->pixels;

	//	if (dy + h > dst->height)
	//		h = dst->height - dy;

	for (int x = 0; x < w; x++)
	{
		if (dx + x < 0 || dx + x >= dst->width) { continue; }

		for (int y = 0; y < h; y++)
		{
			if (dy + y < 0 || dy + y >= dst->height) { continue; }

			dst_pix[(dy + y) * dst->row_bytes + (dx + x)] = colour;
		}
	}
}

void BrPixelmapText(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_32 colour, const br_font* font, const char* text)
{
	br_uint_8 *dst_pix = (br_uint_8 *)dst->pixels;

	if (font->flags & BR_FONTF_PROPORTIONAL)
	{
		br_int_8 *widths = font->width;
		br_uint_8 *glyphs = font->glyphs;
		br_uint_16 dst_offset = (y*dst->row_bytes) + x;

		for (int i = 0; i < strlen(text); i++)
		{
			br_uint_8 pw = widths[(br_uint_8)text[i]];
			br_uint_16 src_offset = font->encoding[(br_uint_8)text[i]];

			//INFO("%ux%u (%ux%u) @ %u = %c", pw, font->glyph_y, bw, font->glyph_y, src_offset, text[i]);

			for (int fy = 0; fy < font->glyph_y; fy++)
			{
				br_uint_8 b;

				for (int fx = 0; fx < pw; fx++)
				{
					if (fx % 8 == 0) { b = glyphs[src_offset + fy + (fx / 8)]; }
					dst_pix[dst_offset + (fy*dst->row_bytes) + fx + i] = (b & (1 << (7 - fx)) ? colour : 0);
				}
			}

			dst_offset += pw;
		}
	}
	else
	{
		WARN("Can't draw fixed with fonts yet!");
	}
}

void BrPixelmapTextF(br_pixelmap* dst, br_int_16 x, br_int_16 y, br_uint_32 colour, const br_font* font, const char* fmt, ...)
{
	char buf[256];

	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	BrPixelmapText(dst, x, y, colour, font, buf);
}

br_uint_16 BrPixelmapTextWidth(const br_pixelmap* dst, const br_font* font, const char* text)
{
	/* FIXME : How is the dst pixelmap relevant? */
	if (font->flags & BR_FONTF_PROPORTIONAL)
	{
		br_uint_16 len = 0;
		br_int_8 *widths = font->width;

		for (int i = 0; i < strlen(text); i++)
		{
			len += widths[(br_uint_8)text[i]];
		}

		return len + strlen(text) - 1;
	}
	else
	{
		return strlen(text) * font->glyph_x;
	}
}

br_uint_32 BrPixelmapSave(const char* filename, const br_pixelmap* pixelmap)
{
	FIXME("(\"%s\", %p): stub", filename, pixelmap);
	return 0;
}

br_pixelmap* BrMapAdd(br_pixelmap* pixelmap)
{
	BrMapUpdate(pixelmap, BR_MAPU_ALL);
	BrListInsert(reg_textures, pixelmap);
	return pixelmap;
}

br_uint_32 BrMapAddMany(br_pixelmap* const* pixelmaps, int n)
{
	br_uint_32 added = 0;

	for (int i = 0; i < n; i++)
		if (BrMapAdd(pixelmaps[i]))
			added++;

	return added;
}

br_pixelmap* BrMapFind(const char* pattern)
{
	return (br_pixelmap *)BrListFind(reg_textures, pattern);
}

void BrMapUpdate(br_pixelmap* pixelmap, br_uint_16 flags)
{
	/* FIXME("(%p, %d): stub", pixelmap, flags); */
}

br_pixelmap* BrMapRemove(br_pixelmap* pixelmap)
{
	return (br_pixelmap *)BrListRemove(reg_textures, pixelmap);
}

br_pixelmap* BrTableAdd(br_pixelmap* pixelmap)
{
	BrTableUpdate(pixelmap, BR_TABU_ALL);
	BrListInsert(reg_tables, pixelmap);
	return pixelmap;
}

br_uint_32 BrTableAddMany(br_pixelmap* const* pixelmaps, int n)
{
	br_uint_32 added = 0;

	for (int i = 0; i < n; i++)
		if (BrTableAdd(pixelmaps[i]))
			added++;

	return added;
}

void BrTableUpdate(br_pixelmap* pixelmap, br_uint_16 flags)
{
	/* FIXME("(%p, %d): stub", pixelmap, flags); */
}

br_pixelmap* BrTableFind(const char* pattern)
{
	return (br_pixelmap *)BrListFind(reg_tables, pattern);
}

br_pixelmap* BrTableRemove(br_pixelmap* pixelmap)
{
	return (br_pixelmap *)BrListRemove(reg_tables, pixelmap);
}

br_uint_32 BrTableEnum(const char* pattern, br_table_enum_cbfn* callback, void* arg)
{
	WARN("Ignoring pattern (\"%s\"); processing everything", pattern);
	br_node *tabs = reg_tables->head;

	while (tabs)
	{
		callback((br_pixelmap *)tabs->data, arg);
		tabs = tabs->next;
	}

	return 0;
}
