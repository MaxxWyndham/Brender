#include "brender.h"
#include "brfile.h"

#include <stdio.h>

br_filesystem *BrFilesystemSet(br_filesystem *fs)
{
	static br_filesystem our_fs =
	{
		"Default",

		NULL,

		BrFileOpenRead,
		BrFileOpenWrite,
		BrFileClose,
		BrFileEof,
		BrFileGetChar,
		BrFilePutChar,
		BrFileRead,
		BrFileWrite,
		BrFileGetLine,
		BrFilePutLine,
		BrFileAdvance
	};

	INFO("(%p): ignoring filesystem");
	return &our_fs;
}

br_uint_32 BrFileAttributes(void)
{
	FIXME("(void): stub");
	return 0xFFFFFFFF;
}

void* BrFileOpenWrite(const char* name, int mode)
{
	if (mode == BR_FS_MODE_TEXT)
	{
		ERR("Text mode, seriously, wtf?");
	}

	return fopen(name, "wb");
}

br_size_t BrFileWrite(void* buf, br_size_t size, unsigned int n, void* f)
{
	return fwrite(buf, size, n, f);
}

int BrFilePrintf(void* f, const char* fmt, ...)
{
	FIXME("(%p, \"%s\", ...): stub", f, fmt);
	return 0;
}

void BrFilePutLine(const char* buf, void* f)
{
	fputs(buf, f);
}

void BrFilePutChar(int c, void* f)
{
	fputc(c, f);
}

void* BrFileOpenRead(const char* name, br_size_t n_magics, br_mode_test_cbfn* mode_test, int* mode_result)
{
	if (mode_test != NULL)
	{
		FIXME("mode_test ignored");
	}

	return fopen(name, "rb");
}

br_size_t BrFileRead(void* buf, br_size_t size, unsigned int n, void* f)
{
	return fread(buf, size, n, f);
}

int BrFileGetLine(char* buf, br_size_t buf_len, void* f)
{
	fgets(buf, buf_len, f);

	FIXME("returning zero instead of bytes read");
	return 0;
}

int BrFileGetChar(void* f)
{
	return fgetc(f);
}

void BrFileAdvance(long int count, void* f)
{
	fseek(f, count, SEEK_CUR);
}

int BrFileEof(void* f)
{
	return feof((FILE *)f);
}

void BrFileClose(void* f)
{
	fclose(f);
}

br_uint_32 BrFileGetLength(const char* name)
{
    FILE *fp = BrFileOpenRead(name, 0, NULL, NULL);
    if (!fp) { return 0; }

    fseek(fp, 0, SEEK_END);
    br_uint_32 size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    BrFileClose(fp);

    return size;
}

int BrWriteModeSet(int mode)
{
	FIXME("(%d): stub", mode);
	return 0;
}
