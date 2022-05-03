/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Sbuffer.h 1.2 1997/08/11 14:25:22 jon Exp JON $
 * $Locker: JON $
 *
 * Stored buffer structure
 */
#ifndef _SBUFFER_H_
#define _SBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Flags
 */
enum {
	SBUFF_SHARED   = 0x0001,	/* Data is shared with user */
	SBUFF_ALLOCONLOAD = 0x0002,	/* Contents must be copied on use */
	SBUFF_LOADED = 0x0004,		/* Contents have been loaded */
};


typedef struct br_buffer_stored 
{
	/*
	 * Dispatch table
	 */
	struct br_buffer_stored_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

	br_device *device;
	br_renderer *renderer;

	/*
	 * Flags
	 */
	br_uint_32 flags;

	/*
	 * Source pixelmap
	 */
	br_pixelmap *source;
	
	/*
	 * Surface and texture interface representing the source version
	 *
	 * The texture interface is only used for ALLOCONLOAD textures
	 */
	LPDIRECTDRAWSURFACE2 source_surface;
	LPDIRECT3DTEXTURE2 source_texture;

	/*
	 * Surface and texture interface representing the stored version
	 */
	LPDIRECTDRAWSURFACE2 stored_surface;
	LPDIRECT3DTEXTURE2 stored_texture;

	/*
	 * Texture handle, and a timestamp for when it was set
	 */
	D3DTEXTUREHANDLE handle;
	br_timestamp handle_timestamp;

} br_buffer_stored;

#ifdef __cplusplus
};
#endif
#endif


