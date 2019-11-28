#include "compiler.h"

#ifndef _BRENDER_BRFILE_H_

/* FIXME: use correct types */
typedef void* br_mode_test_cbfn;

typedef void* brfile_attributes_cbfn;
typedef void* (*brfile_open_read_cbfn)(const char *, br_size_t, br_mode_test_cbfn *, int *);
typedef void* (*brfile_open_write_cbfn)(const char *, int);
typedef void  (*brfile_close_cbfn)(void *);
typedef int   (*brfile_eof_cbfn)(void *);
typedef int   (*brfile_getchr_cbfn)(void *);
typedef void  (*brfile_putchr_cbfn)(int c, void *);
typedef br_size_t (*brfile_read_cbfn)(void *, br_size_t, unsigned int, void *);
typedef br_size_t (*brfile_write_cbfn)(void *, br_size_t, unsigned int, void *);
typedef int   (*brfile_getline_cbfn)(char *, br_size_t, void *);
typedef void  (*brfile_putline_cbfn)(const char *, void *);
typedef void  (*brfile_advance_cbfn)(long int, void *);

typedef struct
{
	char					*identifier;

	brfile_attributes_cbfn	attributes;

	brfile_open_read_cbfn	open_read;
	brfile_open_write_cbfn	open_write;
	brfile_close_cbfn		close;
	brfile_eof_cbfn			eof;
	brfile_getchr_cbfn		getchr;
	brfile_putchr_cbfn		putchr;
	brfile_read_cbfn		read;
	brfile_write_cbfn		write;
	brfile_getline_cbfn		getline;
	brfile_putline_cbfn		putline;
	brfile_advance_cbfn		advance;
} br_filesystem;

#define BR_FS_ATTR_READABLE		(1 << 0)
#define BR_FS_ATTR_WRITEABLE	(1 << 1)
#define BR_FS_ATTR_HAS_TEXT		(1 << 2)
#define BR_FS_ATTR_HAS_BINARY	(1 << 3)
#define BR_FS_ATTR_HAS_ADVANCE	(1 << 4)

enum {
	BR_FS_MODE_TEXT,
	BR_FS_MODE_BINARY
};

br_filesystem *BrFilesystemSet(br_filesystem *);
br_uint_32 BrFileAttributes(void);

void* BrFileOpenWrite(const char* name, int mode);
br_size_t BrFileWrite(void* buf, br_size_t size, unsigned int n, void* f);
int BrFilePrintf(void* f, const char* fmt, ...);
void BrFilePutLine(const char* buf, void* f);
void BrFilePutChar(int c, void* f);
void* BrFileOpenRead(const char* name, br_size_t n_magics, br_mode_test_cbfn* mode_test, int* mode_result);
br_size_t BrFileRead(void* buf, br_size_t size, unsigned int n, void* f);
int BrFileGetLine(char* buf, br_size_t buf_len, void* f);
int BrFileGetChar(void* f);
void BrFileAdvance(long int count, void* f);
int BrFileEof(void* f);
void BrFileClose(void* f);
br_uint_32 BrFileGetLength(const char* name);

int BrWriteModeSet(int mode);

#define _BRENDER_BRFILE_H_
#endif
