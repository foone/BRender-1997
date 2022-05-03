/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: SURFTYPE.C 1.3 1997/07/23 15:51:32 jon Exp JON $
 * $Locker: JON $
 *
 * Useful routines for dealing with BRender and DirectDraw pixel formats
 */
#include "drv.h"
#include "surftype.h"

BR_RCS_ID("$Id: SURFTYPE.C 1.3 1997/07/23 15:51:32 jon Exp JON $");



/*
 * DirectDraw pixel format structures for every BRender pixel format
 */
struct {
	
	br_boolean valid;
	br_boolean indexed;
	br_uint_32 pixel_bits;
	br_uint_32 r_bits;
	br_uint_32 g_bits;
	br_uint_32 b_bits;
	br_uint_32 a_bits;
	
	DDPIXELFORMAT ddpixelformat;

} pixeltype[] = {

	// BR_PMT_INDEX_1

	{ BR_TRUE, BR_TRUE, 1, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED1 | DDPF_RGB, 0,
	    1, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_INDEX_2

	{ BR_TRUE, BR_TRUE, 2, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED2 | DDPF_RGB, 0,
	  2, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_INDEX_4

	{ BR_TRUE, BR_TRUE, 4, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED4 | DDPF_RGB, 0,
	  4, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_INDEX_8

	{ BR_TRUE, BR_TRUE, 8, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED8 | DDPF_RGB, 0,
	    8, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_RGB_555

	{ BR_TRUE, BR_FALSE, 15, 5, 5, 5, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB, 0,
	    16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00000000 } },
	
	// BR_PMT_RGB_565

	{ BR_TRUE, BR_FALSE, 16, 5, 6, 5, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB, 0,
	    16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 } },
	
	// BR_PMT_RGB_888

	{ BR_TRUE, BR_FALSE, 24, 8, 8, 8, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB, 0,
	    24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 } },
	
	// BR_PMT_RGBX_888

	{ BR_TRUE, BR_FALSE, 32, 8, 8, 8, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB, 0,
	    32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 } },
	
	// BR_PMT_RGBA_8888

	{ BR_TRUE, BR_FALSE, 32, 8, 8, 8, 8,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB | DDPF_ALPHAPIXELS, 0,
	    32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 } },
	
	// BR_PMT_YUYV_8888	*** HMMMMMMM ***

	{ BR_TRUE, BR_FALSE, 32, 16, 8, 8, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_YUV, 0,
	    32, 0xff00ff00, 0x00ff0000, 0x000000ff, 0x00000000 } },
	
	// BR_PMT_YUV_888

	{ BR_TRUE, BR_FALSE, 24, 8, 8, 8, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_YUV, 0,
	    24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 } },
	
	// BR_PMT_DEPTH_16

	{ BR_TRUE, BR_FALSE, 16, 0, 0, 0, 16,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x0000ffff } },
	
	// BR_PMT_DEPTH_32

	{ BR_TRUE, BR_FALSE, 32, 0, 0, 0, 32,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    32, 0x00000000, 0x00000000, 0x00000000, 0xffffffff } },
	
	// BR_PMT_ALPHA_8

	{ BR_TRUE, BR_FALSE, 8, 0, 0, 0, 8,
	  { sizeof(DDPIXELFORMAT), DDPF_ALPHA, 0,
	    8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff } },
	
	// BR_PMT_INDEXA_88	*** HMMMMMMM *** *** MASK CORRECT? ***

	{ BR_TRUE, BR_FALSE, 16, 0, 0, 0, 8,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x0000ff00 } },
	
	// BR_PMT_NORMAL_INDEX_8 *** HMMMMMMM ***

	{ BR_FALSE, BR_FALSE, 8, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), 0, 0,
	    8, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_NORMAL_XYZ *** HMMMMMMM *** *** BIT COUNT CORRECT? ***

	{ BR_FALSE, BR_FALSE, 24, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), 0, 0,
	    24, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_BGR_555

	{ BR_TRUE, BR_FALSE, 24, 5, 5, 5, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB, 0,
	    24, 0x0000001f, 0x000003e0, 0x00007c00, 0x00000000 } },
	
	// BR_PMT_RGBA_4444

	{ BR_TRUE, BR_FALSE, 16, 4, 4, 4, 4,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB | DDPF_ALPHAPIXELS, 0,
	    16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 } },
	
    // BR_PMT_RBG_bab *** HMMMMMMM ***

	{ BR_FALSE, BR_FALSE, 16, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), 0, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
    // BR_PMT_RBG_1aba *** HMMMMMMM ***

	{ BR_FALSE, BR_FALSE, 16, 0, 0, 0, 0,
	  { sizeof(DDPIXELFORMAT), 0, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x00000000 } },
	
	// BR_PMT_RGB_332

	{ BR_TRUE, BR_FALSE, 8, 3, 3, 2, 0,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB, 0,
	    8, 0x000000e0, 0x0000001c, 0x00000003, 0x00000000 } },
	
	// BR_PMT_DEPTH_8

	{ BR_TRUE, BR_FALSE, 8, 0, 0, 0, 8,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff } },
	
	// BR_PMT_ARGB_8888 *** MASKS CORRECT? ***

	{ BR_TRUE, BR_FALSE, 32, 8, 8, 8, 8,
	  { sizeof(DDPIXELFORMAT), DDPF_RGB | DDPF_ALPHAPIXELS, 0,
	    32, 0x0000f000, 0x00000f00, 0x000000f0, 0x0000000f } },
	
	// BR_PMT_ALPHA_4

	{ BR_TRUE, BR_FALSE, 4, 0, 0, 0, 4,
	  { sizeof(DDPIXELFORMAT), DDPF_ALPHA, 0,
	    4, 0x00000000, 0x00000000, 0x00000000, 0x0000000f } },
	
	// BR_PMT_AINDEX_44 *** HMMMMMMM *** *** MASK CORRECT? ***

	{ BR_TRUE, BR_FALSE, 8, 0, 0, 0, 4,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED4 | DDPF_ALPHAPIXELS, 0,
	    8, 0x00000000, 0x00000000, 0x00000000, 0x000000f0 } },
	
	// BR_PMT_AINDEX_88 *** HMMMMMMM *** *** MASK CORRECT? ***

	{ BR_TRUE, BR_FALSE, 16, 0, 0, 0, 8,
	  { sizeof(DDPIXELFORMAT), DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x0000ff00 } },
	
	// BR_PMT_DEPTH_15 *** HMMMMMMM ***

	{ BR_TRUE, BR_FALSE, 16, 0, 0, 0, 15,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x00007fff } },
	
	// BR_PMT_DEPTH_31 *** HMMMMMMM ***

	{ BR_TRUE, BR_FALSE, 32, 0, 0, 0, 31,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    32, 0x00000000, 0x00000000, 0x00000000, 0x7fffffff } },
	
	// BR_PMT_DEPTH_FP16 *** HMMMMMMM ***

	{ BR_TRUE, BR_FALSE, 16, 0, 0, 0, 16,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x0000ffff } },
	
	// BR_PMT_DEPTH_FP15

	{ BR_TRUE, BR_FALSE, 16, 0, 0, 0, 15,
	  { sizeof(DDPIXELFORMAT), DDPF_ZBUFFER, 0,
	    16, 0x00000000, 0x00000000, 0x00000000, 0x00007fff } },
};	


/*
 * Pixel format flags that are significant for comparison purposes for indexed and direct pixel
 * types
 */
#define INDEXED_DDPF_MASK (DDPF_PALETTEINDEXED1 | DDPF_PALETTEINDEXED2 | DDPF_PALETTEINDEXED4 |\
		DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS | DDPF_ZPIXELS | DDPF_COMPRESSED)
		
#define DIRECT_DDPF_MASK (DDPF_ALPHA | DDPF_ALPHAPIXELS | DDPF_ZBUFFER | DDPF_ZPIXELS |\
		DDPF_PALETTEINDEXEDTO8 | DDPF_RGB | DDPF_YUV | DDPF_COMPRESSED | DDPF_RGBTOYUV)
		


/*
 * Convert a BRender pixel type to a DirectDraw pixel format
 */
br_error PixelTypeToDDPixelFormat(LPDDPIXELFORMAT *format, br_uint_32 type)
{
	if (type > BR_ASIZE(pixeltype) || !pixeltype[type].valid)
		return BRE_FAIL;

	*format = &pixeltype[type].ddpixelformat;

	return BRE_OK;
}


/*
 * Convert a BRender pixel type to a DirectDraw pixel format
 */
br_error DDPixelFormatToPixelType(br_uint_32 *type, LPDDPIXELFORMAT format)
{
	br_uint_32 i;

	for (i = 0; i < BR_ASIZE(pixeltype); i++) {

		/*
		 * Check appropriate flags are correct
		 *
		 * At the moment I ignore the PALETTEINDEXEDTO8, RGB and YUV flags for indexed types because
		 * I don't care what the palette is.  I also ignore some other flags which have no place
		 * in an indexed type
		 */
		if (format->dwFlags & (DDPF_PALETTEINDEXED1 | DDPF_PALETTEINDEXED2 |
			DDPF_PALETTEINDEXED4 | DDPF_PALETTEINDEXED8)) {

			if ((format->dwFlags & INDEXED_DDPF_MASK) !=
				(pixeltype[i].ddpixelformat.dwFlags & INDEXED_DDPF_MASK))

				continue;

		} else {

			if ((format->dwFlags & DIRECT_DDPF_MASK) !=
				(pixeltype[i].ddpixelformat.dwFlags & DIRECT_DDPF_MASK))

				continue;
		}

		if (format->dwFlags & DDPF_RGB &&
			(format->dwRBitMask != pixeltype[i].ddpixelformat.dwRBitMask ||
			 format->dwGBitMask != pixeltype[i].ddpixelformat.dwGBitMask ||
			 format->dwBBitMask != pixeltype[i].ddpixelformat.dwBBitMask))

			continue;

		if (format->dwFlags & DDPF_YUV &&
			(format->dwYBitMask != pixeltype[i].ddpixelformat.dwYBitMask ||
			 format->dwUBitMask != pixeltype[i].ddpixelformat.dwUBitMask ||
			 format->dwVBitMask != pixeltype[i].ddpixelformat.dwVBitMask))

			continue;

		if (format->dwFlags & (DDPF_ALPHA | DDPF_ALPHAPIXELS) &&
			format->dwRGBAlphaBitMask != pixeltype[i].ddpixelformat.dwRGBAlphaBitMask)

			continue;

		if (format->dwFlags & (DDPF_ZBUFFER | DDPF_ZPIXELS) &&
			format->dwRGBZBitMask != pixeltype[i].ddpixelformat.dwRGBZBitMask)

			continue;

		*type = i;
		return BRE_OK;
	}

	return BRE_FAIL;
}


/*
 * Simple pixel type properties - would prefer to make these inline, but H2INC can't cope with the
 * windows includes
 */
br_boolean PixelTypeIndexed(br_uint_32 type)
{
	return pixeltype[type].indexed;
}

br_uint_32 PixelTypePixelBits(br_uint_32 type)
{
	return pixeltype[type].pixel_bits;
}

br_uint_32 PixelTypePixelBytes(br_uint_32 type)
{
	return (pixeltype[type].pixel_bits + 7) / 8;
}

br_uint_32 PixelTypeRBits(br_uint_32 type)
{
	return pixeltype[type].r_bits;
}

br_uint_32 PixelTypeGBits(br_uint_32 type)
{
	return pixeltype[type].g_bits;
}

br_uint_32 PixelTypeBBits(br_uint_32 type)
{
	return pixeltype[type].b_bits;
}

br_uint_32 PixelTypeABits(br_uint_32 type)
{
	return pixeltype[type].a_bits;
}

