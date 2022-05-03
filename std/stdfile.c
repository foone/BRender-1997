/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: stdfile.c 2.7 1996/11/20 18:07:52 STEVEW Exp $
 * $Locker: $
 *
 * Default file handler that uses <stdio.h>
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "brender.h"
#include "host.h"
#include "brassert.h"


BR_RCS_ID("$Id: stdfile.c 2.7 1996/11/20 18:07:52 STEVEW Exp $")

/*
 * Access functions for stdio
 */
static br_uint_32 BR_CALLBACK BrStdioAttributes(void)
{
	return 
		BR_FS_ATTR_READABLE   | 
		BR_FS_ATTR_WRITEABLE  |
		BR_FS_ATTR_HAS_TEXT   |
		BR_FS_ATTR_HAS_BINARY |
		BR_FS_ATTR_HAS_ADVANCE;
}

/*
 * Open a file for reading
 *
 * Use BRENDER_PATH to locate the file if necessary
 *
 * Having found the file, invoke a supplied callback, (if present)
 * to find out if the file is text, binary, or unknown
 *
 * Text mode files are primarily used for debugging but it can be
 * useful to allow hand editting of input data
 *
 * Return a void * file handle ('FILE *' cast to 'void *') or NULL
 * if open failed
 */
void * BR_CALLBACK BrStdioOpenRead(const char *name, br_size_t n_magics,
		br_mode_test_cbfn *identify,
		int *mode_result)
{
	FILE *fh;
	char *br_path, config_path[512];
   	char try_name[512],*cp;
	br_uint_8 magics[BR_MAX_FILE_MAGICS];
	int open_mode = BR_FS_MODE_BINARY;

	/*
	 * Try the current directory
	 */
	strncpy(try_name,name,BR_ASIZE(try_name)-1);

	if((fh = fopen(try_name,"rb")) == NULL) {

		/*
		 * If that fails, and if a drive or a
	     * directory were specified, don't search along path
		 */
		if(strchr(name,':') || strchr(name,'/') || strchr(name,'\\'))
			return NULL;

		/*
		 * For each element of the path, if it exists
		 */

      if ( BrSystemConfigQueryString( BRT_BRENDER_PATH_STR, config_path, 
                           BR_ASIZE(config_path) ) != BRE_OK )
         return NULL ;

		br_path = config_path;

		while(*br_path) {
			/*
			 * Take characters until next seperator or terminator
			 */
			cp = try_name;

			while(*br_path != ';' && *br_path != '\0')
				*cp++ = *br_path++;

			if(*br_path == ';')
				br_path++;

			/*
			 * Add a directory seperator if none
			 */
			if(cp != try_name && (*(cp-1) != ':' && *(cp-1) != '/' &&*(cp-1) != '\\'))
				*cp++ = '/';

			strcpy(cp,name);

			if((fh = fopen(try_name,"rb")) != NULL)
				break;
		}

		if(fh == NULL)
			return NULL;
	}

	if(mode_result)
		 open_mode = *mode_result;

	if(n_magics != 0) {
		/*
		 * Now have an open file, try and grab the first bytes from it
		 */
		if(fread(magics, 1, n_magics,fh) != n_magics) {
			/*
			 * Could not read all the required data, close and punt
			 */
			fclose(fh);
			return NULL;
		}

		/*
		 * Try and identify the file
		 */
		if(identify)
			open_mode = identify(magics, n_magics);

		if(mode_result)
			*mode_result = open_mode;
	}

	/*
	 * Reopen file with it's new identity (or abandon if unknown identity)
	 */
	fclose(fh);

	switch(open_mode) {
	case BR_FS_MODE_TEXT:
		fh = fopen(try_name,"r");
		break;

	case BR_FS_MODE_BINARY:
		fh = fopen(try_name,"rb");
		break;

	case BR_FS_MODE_UNKNOWN:
		fh = NULL;
		break;

	default:
		BR_ERROR1("BrStdFileOpenRead: invalid open_mode %d",open_mode);
	}

	return fh;
}


/*
 * Open a file for writing, overwrites any existing file of the same name
 *
 * Return a void * file handle ('FILE *' cast to 'void *') or NULL
 * if open failed
 */
static void * BR_CALLBACK BrStdioOpenWrite(const char *name, int mode)
{
	FILE *fh = NULL;

	fh = fopen(name,(mode == BR_FS_MODE_TEXT)?"w":"wb");

	return fh;
}

/*
 * Close an open file
 */
static void BR_CALLBACK BrStdioClose(void *f)
{
	fclose(f);
}

/*
 * Test EOF
 */
static int BR_CALLBACK BrStdioEof(void *f)
{
	return feof((FILE *)f);
}

/*
 * Read one character from file
 */
static int BR_CALLBACK BrStdioGetChar(void *f)
{
	return getc((FILE *)f);
}


/*
 * Write one character to file
 */
static void BR_CALLBACK BrStdioPutChar(int c, void *f)
{
	fputc(c,(FILE *)f);
}

/*
 * Read a block from a file
 */
static br_size_t BR_CALLBACK BrStdioRead(void *buf, br_size_t size, unsigned int n, void *f)
{
	return fread(buf,size,n,(FILE *)f);
}

/*
 * Write a block to a file
 */
static br_size_t BR_CALLBACK BrStdioWrite(void *buf, br_size_t size, unsigned int n, void *f)
{
	return fwrite(buf,size,n,(FILE *)f);
}

/*
 * Read a line of text from stdin and trim any terminators
 *
 * Return length of line
 */
static br_size_t BR_CALLBACK BrStdioGetLine(char *buf, br_size_t buf_len, void * f)
{
	br_size_t l;

	ASSERT( f );
	
	if(fgets(buf,buf_len,(FILE *)f) == NULL)
		return 0;

	l = strlen(buf);

	if(l > 0 && buf[l-1] == '\n')
		buf[--l] = '\0';
		
	return l;
}

/*
 * Write a line to text file, followed by newline
 */
void BR_CALLBACK BrStdioPutLine(char *buf, void * f)
{
	fputs(buf,(FILE *)f);
	fputc('\n',(FILE *)f);
}

/*
 * Advance N bytes through a binary stream
 */
static void BR_CALLBACK BrStdioAdvance(br_size_t count, void *f)
{
	fseek(f,(long int)count,SEEK_CUR);
}

/*
 * Filesystem structure
 */
br_filesystem BrStdioFilesystem = {
	"Standard IO",			/* identifier */
	
	BrStdioAttributes,		/* attributes */
	BrStdioOpenRead,		/* open_read  */
	BrStdioOpenWrite,		/* openwrite  */
	BrStdioClose,			/* close      */
	BrStdioEof,				/* eof		  */

	BrStdioGetChar,			/* getchar	  */
	BrStdioPutChar,			/* putchar	  */

	BrStdioRead,			/* read		  */
	BrStdioWrite,			/* write	  */

	BrStdioGetLine,			/* getline	  */
	BrStdioPutLine,			/* putline	  */

	BrStdioAdvance,			/* advance	  */
};

/*
 * Override global variable s.t. the default filesystem will be stdio
 */
br_filesystem * BR_ASM_DATA _BrDefaultFilesystem = &BrStdioFilesystem;
