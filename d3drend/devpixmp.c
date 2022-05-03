/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: devpixmp.c 1.4 1997/08/11 14:25:31 jon Exp JON $
 * $Locker: JON $
 *
 * Device pixelmap methods
 */
#include <stddef.h>

#include "drv.h"
#include "pm.h"
#include "host.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: devpixmp.c 1.4 1997/08/11 14:25:31 jon Exp JON $");


/*
 * Default dispatch table for device (defined at end of file)
 */
static const struct br_device_pixelmap_dispatch devicePixelmapDispatch;

/*
 * Device pixelmap info. template
 */
#define F(f)	offsetof(struct br_device_pixelmap, f)

static struct br_tv_template_entry devicePixelmapTemplateEntries[] = {

	{BRT(WIDTH_I32),		F(pm_width),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_I32_U16, },
	{BRT(HEIGHT_I32),		F(pm_height),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_I32_U16, },
	{BRT(PIXEL_TYPE_U8),	F(pm_type),			BRTV_QUERY | BRTV_ALL,	BRTV_CONV_I32_U8, },
	{BRT(INDEXED_B),		F(indexed),			BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT(OUTPUT_FACILITY_O),F(output_facility),	BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
	{BRT(FACILITY_O),		F(output_facility),	BRTV_QUERY,				BRTV_CONV_COPY, },
	{BRT(IDENTIFIER_CSTR),	F(pm_identifier),	BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
};
#undef F

static struct br_tv_template devicePixelmapTemplate = {
	BR_ASIZE(devicePixelmapTemplateEntries),
	devicePixelmapTemplateEntries
};


struct pixelmapNewTokens 
{
   HWND window;
};

#define F(f)   offsetof(struct pixelmapNewTokens, f)

static struct br_tv_template_entry pixelmapNewTemplateEntries[] = 
{
   {BRT(WINDOW_HANDLE_H),           F(window),         BRTV_SET,   BRTV_CONV_COPY, },
};
#undef F


/*
 * Create a new device pixelmap and set a display mode
 */
br_device_pixelmap *DevicePixelmapD3DAllocate(br_device *device, br_output_facility *facility, br_token_value *tv)
{
	struct pixelmapNewTokens pt = { NULL };
	DDSURFACEDESC surface_desc = { sizeof(DDSURFACEDESC) };
	LPDIRECTDRAWSURFACE surface;
	RECT rect;
	br_device_pixelmap *self;
	br_uint_32 count;

	/*
	 * Process any options
	 */
    if (device->templates.pixelmapNewTemplate == NULL)
		device->templates.pixelmapNewTemplate = BrTVTemplateAllocate(device,
			pixelmapNewTemplateEntries, BR_ASIZE(pixelmapNewTemplateEntries));

	BrTokenValueSetMany(&pt, &count, NULL, tv, device->templates.pixelmapNewTemplate);

	/*
	 * Can't function without a window
	 */
	if (pt.window == NULL)
		return NULL;
	
	/*
	 * Create object
	 */
	self = BrResAllocate(facility, sizeof(*self), BR_MEMORY_OBJECT);
	if (self == NULL)
		return NULL;	

	self->dispatch = (struct br_device_pixelmap_dispatch *)&devicePixelmapDispatch;

	self->device = device;
	self->output_facility = facility;

	self->use = BR_NULL_TOKEN;
	
	if (facility->fullscreen) {

		/*
		 * Fullscreen - get exclusive access to the display
		 *
		 * *** SteveW didn't allow minimization but we should if it works. ***
		 * *** Should we allow reboot?  Is this immediate reboot, or does it flip to the
		 *     restart dialog box? ***
		 */
		if (IDirectDraw2_SetCooperativeLevel(facility->ddraw.ddraw, pt.window,
			DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT) != DD_OK) {

			BrResFree(self);
			return NULL;
		}
		
		/*
		 * Set the desired display mode
		 *
		 * N.B. This is the root of the old 15/16 bit problem
		 */
		if (IDirectDraw2_SetDisplayMode(facility->ddraw.ddraw,
			facility->ddraw.surface_desc->dwWidth, facility->ddraw.surface_desc->dwHeight, facility->ddraw.surface_desc->ddpfPixelFormat.dwRGBBitCount,
			facility->ddraw.surface_desc->dwRefreshRate, 0) != DD_OK) {

			BrResFree(self);
			return NULL;
		}

		self->restore_mode = BR_TRUE;

		/*
		 * Get the primary surface
		 */
		surface_desc.dwFlags = DDSD_CAPS;
		surface_desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;
 		
		if (IDirectDraw2_CreateSurface(facility->ddraw.ddraw, &surface_desc, &surface, NULL) != DD_OK) {
			BrResFree(self);
			return NULL;
		}
	
	} else {

		/*
		 * Windowed - use normal access to the display
		 */
		if (IDirectDraw2_SetCooperativeLevel(facility->ddraw.ddraw, pt.window, DDSCL_NORMAL) != DD_OK) {
			BrResFree(self);
			return NULL;
		}

		/*
		 * Get the primary surface
		 */
		surface_desc.dwFlags = DDSD_CAPS;
		surface_desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		
		if (IDirectDraw2_CreateSurface(facility->ddraw.ddraw, &surface_desc, &surface, NULL) != DD_OK) {
			BrResFree(self);
			return NULL;
		}
	}

	/*
	 * Get the newer version of the DirectDrawSurface interface
	 */
	if (IDirectDrawSurface_QueryInterface(surface, &IID_IDirectDrawSurface2, &self->surface) != DD_OK) {
		BrResFree(self);
		return NULL;
	}

	IDirectDraw_Release(surface);

	if (!facility->fullscreen) {

		/*
		 * Create a clipper to prevent us from drawing outside our window
		 */
		if (IDirectDraw2_CreateClipper(facility->ddraw.ddraw, 0, &self->clipper, NULL) != DD_OK) {
			BrResFree(self);
			return NULL;
		}

		if (IDirectDrawClipper_SetHWnd(self->clipper, 0, pt.window) != DD_OK) {
			BrResFree(self);
			return NULL;
		}

		if (IDirectDrawSurface2_SetClipper(self->surface, self->clipper) != DD_OK) {
			BrResFree(self);
			return NULL;
		}
	}

	/*
	 * Fill in the pixelmap structure
	 */
	self->pm_type = (br_uint_8)facility->colour_type;

	self->indexed = facility->indexed;

	self->window = pt.window;
	
	if (facility->fullscreen) {
	
		self->pm_width = (br_uint_16)facility->width;
		self->pm_height = (br_uint_16)facility->height;

	} else {

		GetClientRect(self->window, &rect);

		self->pm_width  = (br_uint_16)rect.right;
		self->pm_height = (br_uint_16)rect.bottom;
	}
		
	self->pm_origin_x = 0;
	self->pm_origin_y = 0;

	self->pm_base_x = 0;
	self->pm_base_y = 0;

	self->pm_flags = BR_PMF_NO_ACCESS;

	self->matched = NULL;
	self->screen = self;

	self->pm_identifier = facility->identifier;

	ObjectContainerAddFront(facility, (br_object *)self);

	return self;
}

void BR_CMETHOD_DECL(br_device_pixelmap_d3d, free)(br_device_pixelmap *self)
{
	ObjectContainerRemove(self->output_facility, (br_object *)self);

	/*
	 * Gross hack to get around the fact that you cannot initialise the renderer without offscreen
	 * and depth buffers already allocated
	 */
	if (self->screen != NULL && self->screen->offscreen_surface == self->surface) {

		IDirectDrawSurface2_Release(self->surface);
		self->screen->offscreen_surface = NULL;
	}

	if (self->screen != NULL && self->screen->depth_surface == self->surface) {

		IDirectDrawSurface2_Release(self->surface);
		self->screen->depth_surface = NULL;
	}

	/*
	 * Free DirectDraw surface and clipper
	 */
	if (self->clipper != NULL)
		IDirectDrawClipper_Release(self->clipper);

	if (self->surface != NULL)
		IDirectDrawSurface2_Release(self->surface);

	/*
	 * Restore video mode
	 */
	if (self->restore_mode)
		IDirectDraw_SetCooperativeLevel(self->output_facility->ddraw.ddraw, self->window, DDSCL_NORMAL);
	
	BrResFreeNoCallback(self);
}

char *BR_CMETHOD_DECL(br_device_pixelmap_d3d, identifier)(br_device_pixelmap *self)
{
	return self->pm_identifier;
}

br_token BR_CMETHOD_DECL(br_device_pixelmap_d3d, type)(br_device_pixelmap *self)
{
	return BRT_DEVICE_PIXELMAP;
}

br_boolean BR_CMETHOD_DECL(br_device_pixelmap_d3d, isType)(br_device_pixelmap *self, br_token t)
{
	return (t == BRT_DEVICE_PIXELMAP) || (t == BRT_OBJECT);
}

br_device *BR_CMETHOD_DECL(br_device_pixelmap_d3d, device)(br_device_pixelmap *self)
{
	return self->device;
}

br_int_32 BR_CMETHOD_DECL(br_device_pixelmap_d3d, space)(br_device_pixelmap *self)
{
	return sizeof(br_device_pixelmap);
}


struct br_tv_template * BR_CMETHOD_DECL(br_device_pixelmap_d3d, templateQuery)(br_device_pixelmap *self)
{
    if (self->device->templates.devicePixelmapTemplate == NULL)
        self->device->templates.devicePixelmapTemplate = BrTVTemplateAllocate(self->device,
            devicePixelmapTemplateEntries,
            BR_ASIZE(devicePixelmapTemplateEntries));

    return self->device->templates.devicePixelmapTemplate;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, validSource)
	(br_device_pixelmap *self, br_object *h)
{
	/* ... */
	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, resize)(br_device_pixelmap *self, br_int_32 width, br_int_32 height)
{
	return BRE_FAIL;
}


/*
 * Structure used to unpack the 'match' tokens/values
 */
struct pixelmapMatchTokens {

   br_token use;
   br_int_32 pixel_bits;
   br_int_32 width;
   br_int_32 height;
   br_uint_8 type ;
};

#define F(f) offsetof(struct pixelmapMatchTokens, f)

static struct br_tv_template_entry pixelmapMatchTemplateEntries[] = {

	{BRT_USE_T,          0, F(use),        BRTV_SET, BRTV_CONV_COPY, },
	{BRT_PIXEL_BITS_I32, 0, F(pixel_bits), BRTV_SET, BRTV_CONV_COPY, },
	{BRT_WIDTH_I32,      0, F(width),      BRTV_SET, BRTV_CONV_COPY, },
	{BRT_HEIGHT_I32,     0, F(height),     BRTV_SET, BRTV_CONV_COPY, },
	{BRT_PIXEL_TYPE_U8,  0, F(type),       BRTV_SET, BRTV_CONV_COPY, },
};

#undef F


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, match)(br_device_pixelmap *self,
	br_device_pixelmap **newpm, br_token_value *tv)
{
	struct pixelmapMatchTokens mt;
	DDSURFACEDESC surface_desc = { sizeof(DDSURFACEDESC) };
	LPDIRECTDRAWSURFACE surface;
	LPDDPIXELFORMAT pixel_format;
	br_device_pixelmap *pm;
	br_uint_32 count;
	char *identifier;
	
	/*
	 * Set default values of match parameters
	 */
	mt.use = BRT_NONE;
	mt.pixel_bits = 0;
	mt.width = self->pm_width;
	mt.height = self->pm_height;
	mt.type = 0xff;
	
	/*
	 * Process match parameters
	 */
	if (self->device->templates.pixelmapMatchTemplate == NULL)
		self->device->templates.pixelmapMatchTemplate = BrTVTemplateAllocate(self->device,
			pixelmapMatchTemplateEntries, BR_ASIZE(pixelmapMatchTemplateEntries));
	
	BrTokenValueSetMany(&mt, &count, NULL, tv, self->device->templates.pixelmapMatchTemplate);
	
	if (mt.use == BRT_DEPTH)
	
		/*
		 * Determine pixelmap type from bit depth for depth buffers
		 *
		 * *** What about alpha buffers?
		 */
		switch (mt.pixel_bits) {

		case 8:
			mt.type = BR_PMT_DEPTH_8;
			break;

		case 15:
			mt.type = BR_PMT_DEPTH_15;
			break;

		case 0:

			/*
			 * If no bit depth specified, a type may have been specified.  Matching depth buffers from
			 * depth buffers is illegal so...
			 */
			if (mt.type != self->pm_type)
				break;

			mt.pixel_bits = 16;
			// DROP THROUGH...

		case 16:
			mt.type = BR_PMT_DEPTH_16;
			break;

		case 31:
			mt.type = BR_PMT_DEPTH_31;
			break;

		case 32:
			mt.type = BR_PMT_DEPTH_32;
			break;

		default:
			return BRE_UNSUPPORTED;
		}


	/*
	 * Set up surface descriptor
	 */
	surface_desc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	
	surface_desc.dwWidth = mt.width;
	surface_desc.dwHeight = mt.height;
	
	if (mt.type != 0xff) {

		if (PixelTypeToDDPixelFormat(&pixel_format, mt.type) != BRE_OK)
			return BRE_FAIL;

		surface_desc.ddpfPixelFormat = *pixel_format;

	} else {

		surface_desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		if (IDirectDrawSurface2_GetPixelFormat(self->surface, &surface_desc.ddpfPixelFormat) != DD_OK)
			return BRE_FAIL;
	}

	/*
	 * Add any additional requirements
	 */
	switch (mt.use) {

	case BRT_CLONE:

		/*
		 * Clone pixelmaps - copy all pertinent parameters over
		 *
		 * N.B. Cloning complex surfaces only clones one element, and at the moment I allow the
		 * clone to be in different sort of memory
		 */
		identifier = self->pm_identifier;
		
		if (IDirectDrawSurface2_GetSurfaceDesc(self->surface, &surface_desc) != DD_OK)
			return BRE_FAIL;

		if (surface_desc.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {

			/*
			 * Cloning primary surfaces requires stripping out almost all of the requirements
			 * otherwise it won't work, even if they're correct!
			 */
			surface_desc.dwFlags = DDSD_CAPS;
			surface_desc.ddsCaps.dwCaps &= (DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE);

		} else {

			/*
			 * Strip out capability flags which are really properties of the original
			 * surface, or concerned with complex surface structures
			 */
			surface_desc.dwFlags &= ~(DDSD_BACKBUFFERCOUNT | DDSD_ZBUFFERBITDEPTH |
				DDSD_ALPHABITDEPTH | DDSD_LPSURFACE | DDSD_MIPMAPCOUNT | DDSD_LINEARSIZE);

			surface_desc.ddsCaps.dwCaps &= ~(DDSCAPS_RESERVED1 | DDSCAPS_COMPLEX |
				DDSCAPS_BACKBUFFER | DDSCAPS_FRONTBUFFER | DDSCAPS_SYSTEMMEMORY |
				DDSCAPS_VIDEOMEMORY | DDSCAPS_WRITEONLY | DDSCAPS_RESERVED2 | DDSCAPS_LOCALVIDMEM |
				DDSCAPS_NONLOCALVIDMEM);
		}
			
		break;
	
	case BRT_OFFSCREEN:

		/*
		 * Offscreen surfaces must be valid rendering targets
		 */
		identifier = "offscreen";
		surface_desc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;
		break;

	case BRT_DEPTH:
	
		/*
		 * Depth buffers must be valid z targets
		 *
		 * Also it doesn't seem to work if you supply a pixel format (bah!)
		 */
		identifier = "depth";
		surface_desc.dwFlags &= ~DDSD_PIXELFORMAT;
		surface_desc.dwFlags |= DDSD_ZBUFFERBITDEPTH;
		surface_desc.dwZBufferBitDepth = surface_desc.ddpfPixelFormat.dwZBufferBitDepth;
		surface_desc.ddsCaps.dwCaps |= DDSCAPS_ZBUFFER;
		break;

	case BRT_HIDDEN:
	
		/*
		 * Hidden pixelmaps must be valid rendering targets
		 */
		identifier = "hidden";
		surface_desc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;
		break;

	case BRT_HIDDEN_BUFFER:
	
		/*
		 * Hidden buffer pixelmaps must be valid rendering targets and valid textures
		 */
		identifier = "hidden buffer";
		surface_desc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE | DDSCAPS_TEXTURE;
		break;

	case BRT_NO_RENDER:
	
		/*
		 * No render pixelmaps have no extra requirements
		 */
		identifier = "no render";
		break;

	case BRT_BUFFER:
	
		/*
		 * Buffer pixelmaps must be valid textures
		 */
		identifier = "buffer";
		surface_desc.ddsCaps.dwCaps |= DDSCAPS_TEXTURE;
		break;

	default:

		return BRE_UNSUPPORTED;
	}
	
	/*
	 * Create object
	 */
	pm = BrResAllocate(self->output_facility, sizeof(*pm), BR_MEMORY_OBJECT);
	if (pm == NULL)
		return BRE_FAIL;	

	pm->dispatch = (struct br_device_pixelmap_dispatch *)&devicePixelmapDispatch;

	pm->device = self->device;
	pm->output_facility = self->output_facility;

	if (mt.use == BRT_CLONE)
		pm->use = self->use;
	else
		pm->use = mt.use;
	
	/*
	 * Create the surface.
	 *
	 * N.B. Use driver's scheme for electing to place surfaces in system, local video and non-local
	 * video memory at present.
	 */
	if (IDirectDraw2_CreateSurface(self->output_facility->ddraw.ddraw, &surface_desc, &surface, NULL) != DD_OK) {
		BrResFree(pm);
		return BRE_FAIL;
	}

	/*
	 * Get the newer version of the DirectDrawSurface interface
	 */
	if (IDirectDrawSurface_QueryInterface(surface, &IID_IDirectDrawSurface2, &pm->surface) != DD_OK) {
		BrResFree(pm);
		return BRE_FAIL;
	}

	IDirectDrawSurface_Release(surface);

	/*
	 * Attach the surface if necessary
	 *
	 * Offscreen buffers are not attached for windowed operation
	 */
	if (mt.use == BRT_DEPTH || mt.use == BRT_OFFSCREEN && self->output_facility->fullscreen)

		if (IDirectDrawSurface2_AddAttachedSurface(self->surface, pm->surface) != DD_OK) {
			BrResFree(pm);
			return BRE_FAIL;
		}
	
	/*
	 * Gross hack to get around the fact that you cannot initialise the renderer without offscreen
	 * and depth buffers already allocated
	 *
	 * Remember the first offscreen buffer matched from the screen, and the first depth buffer matched
	 * from the screen or an offscreen buffer matched from the screen
	 */
	switch (mt.use) {

	case BRT_OFFSCREEN:

		if (self == self->screen) {
			self->screen->offscreen_surface = pm->surface;
			IDirectDrawSurface2_AddRef(pm->surface);
		}
		break;

	case BRT_DEPTH:

		if (self == self->screen || self->use == BRT_OFFSCREEN && self->matched == self->screen) {
			self->screen->depth_surface = pm->surface;
			IDirectDrawSurface2_AddRef(pm->surface);
		}
		break;
	}

	/*
	 * Fill in the pixelmap structure
	 */
	pm->pm_type = mt.type;

	pm->indexed = PixelTypeIndexed(mt.type);

	pm->pm_width = (br_uint_16)mt.width;
	pm->pm_height = (br_uint_16)mt.height;
		
	/*
	 * Copy origin over when dimensions are the same
	 */
	if (mt.width == self->pm_width && mt.height == self->pm_height) {

		pm->pm_origin_x = self->pm_origin_x;
		pm->pm_origin_y = self->pm_origin_y;

	} else {
	
		pm->pm_origin_x = self->pm_origin_x;
		pm->pm_origin_y = self->pm_origin_y;
	}

	pm->pm_base_x = 0;
	pm->pm_base_y = 0;

	pm->pm_flags = BR_PMF_NO_ACCESS;

	pm->matched = self;
	pm->screen = self->screen;
	
	pm->pm_identifier = identifier;

	ObjectContainerAddFront(self->output_facility, (br_object *)pm);

	*newpm = pm;

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, allocateSub)(br_device_pixelmap *self, br_device_pixelmap **newpm, br_rectangle *rect)
{
	return BRE_FAIL;
}

	
br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, doubleBuffer)(br_device_pixelmap *self,
	br_device_pixelmap *src)
{
	br_uint_16 self_copy_function;
	br_uint_16 src_copy_function;
	br_error r;
	HRESULT dderr;
 
	if (self->output_facility->fullscreen) {

		/*
		 * Use flip for fullscreen display modes
		 */
		do {
			dderr = IDirectDrawSurface2_Flip(self->surface, src->surface, DDFLIP_WAIT);

			switch (dderr) {

			case DD_OK:
				break;

			case DDERR_SURFACELOST:

				/*
				 * Not clear which surface is lost, so restore both.
				 */
				if (IDirectDrawSurface2_Restore(self->surface) != DD_OK ||
					IDirectDrawSurface2_Restore(src->surface) != DD_OK)

					return BRE_FAIL;

				break;

			default:
				return BRE_FAIL;
			}

		} while (dderr != DD_OK);

	} else {
		
		/*
		 * Otherwise use blit, temporarily disabling colour keying
		 */
		self_copy_function = self->pm_copy_function;
		src_copy_function = src->pm_copy_function;
		
		self->pm_copy_function = BR_PMCOPY_NORMAL;
		src->pm_copy_function = BR_PMCOPY_NORMAL;

		r = BR_CMETHOD_CALL(br_device_pixelmap, copy, self)(self, src);

		self->pm_copy_function = self_copy_function;
		src->pm_copy_function = src_copy_function;
	}	

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, rectangleCopy)(br_device_pixelmap *self,
	br_point *p, br_device_pixelmap *src, br_rectangle *r)
{
	br_rectangle ar;
	br_point ap;
	POINT origin;
	RECT source, dest;
	DWORD flags;
	DDBLTFX bltfx = { sizeof(DDBLTFX) };
	HRESULT dderr;

	/*
	 * Clip the regions and resolve any sub-pixelmap nonsense
	 *
	 * Should be possible to set up a clipper for sub-pixelmaps and leave all clipping down to
	 * Direct3D
	 */
	if (PixelmapRectangleClipTwo(&ar, &ap, r, p, (br_pixelmap *)self, (br_pixelmap *)src) == BR_CLIP_REJECT)
		return BRE_OK;

	/*
	 * Calculate the source and destination regions of the surfaces, offsetting any on pixelmaps
	 * representing windows on the primary surface
	 */
	if (self->window != NULL) {

		origin.x = 0;
		origin.y = 0;

		if (!ClientToScreen(self->window, &origin))
			return BRE_FAIL;

		dest.left = origin.x + self->pm_base_x + ap.x;
		dest.top = origin.y + self->pm_base_y + ap.y;

	} else {
	
		dest.left = self->pm_base_x + ap.x;
		dest.top = self->pm_base_y + ap.y;
   	}

	dest.right = dest.left + ar.w;
	dest.bottom = dest.top + ar.h;
	
	if (src->window != NULL) {

		origin.x = 0;
		origin.y = 0;

		if (!ClientToScreen(src->window, &origin))
			return BRE_FAIL;

		source.left = origin.x + src->pm_base_x + ar.x;
		source.top = origin.y + src->pm_base_y + ar.y;

	} else {
	
		source.left = self->pm_base_x + ar.x;
		source.top = self->pm_base_y + ar.y;
   	}

	source.right = source.left + ar.w;
	source.bottom = source.top + ar.h;
	
	/*
	 * Determine the correct blit flags and colour keys
	 *
	 * Could add support for pixelmaps with alpha and depth components
	 */
	flags = DDBLT_WAIT;

	/*
	 * Dodgy, interpreting the copy_function as a flags field!
	 */
	if (self->pm_copy_function & BR_PMCOPY_DST_KEYED) {
		
		flags |= DDBLT_KEYDEST | DDBLT_KEYDESTOVERRIDE;

		bltfx.ddckDestColorkey.dwColorSpaceLowValue = (DWORD)self->pm_dst_key.low;
		bltfx.ddckDestColorkey.dwColorSpaceHighValue = (DWORD)self->pm_dst_key.high;
	}
	
	if (src->pm_copy_function & BR_PMCOPY_SRC_KEYED) {
		
		flags |= DDBLT_KEYSRC | DDBLT_KEYSRCOVERRIDE;

		bltfx.ddckSrcColorkey.dwColorSpaceLowValue = (DWORD)src->pm_dst_key.low;
		bltfx.ddckSrcColorkey.dwColorSpaceHighValue = (DWORD)src->pm_dst_key.high;
	}
	
	/*
	 * Perform the blit
	 */
	do {
		dderr = IDirectDrawSurface2_Blt(self->surface, &dest, src->surface, &source, flags, &bltfx);
			
		switch (dderr) {

		case DD_OK:
			break;

		case DDERR_SURFACELOST:

			/*
			 * Not clear which surface is lost, so restore both.
			 */
			if (IDirectDrawSurface2_Restore(self->surface) != DD_OK ||
				IDirectDrawSurface2_Restore(src->surface) != DD_OK)

				return BRE_FAIL;

			break;

		default:
			return BRE_FAIL;
		}

	} while (dderr != DD_OK);

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, rectangleStretchCopy)(br_device_pixelmap *self,
	br_rectangle *d, br_device_pixelmap *src, br_rectangle *s)
{
	br_rectangle ad, as;
	POINT origin;
	RECT source, dest;
	DWORD flags;
	DDBLTFX bltfx = { sizeof(DDBLTFX) };
	HRESULT dderr;

	/*
	 * Clip the regions and resolve any sub-pixelmap nonsense
	 *
	 * Should be possible to set up a clipper for sub-pixelmaps and leave all clipping down to
	 * Direct3D
	 */
	if (PixelmapRectanglesClipTwo(&ad, &as, d, s, (br_pixelmap *)self, (br_pixelmap *)src) == BR_CLIP_REJECT)
		return BRE_OK;

	/*
	 * Calculate the source and destination regions of the surfaces, offsetting any on pixelmaps
	 * representing windows on the primary surface
	 */
	if (self->window != NULL) {

		origin.x = 0;
		origin.y = 0;

		if (!ClientToScreen(self->window, &origin))
			return BRE_FAIL;

		dest.left = origin.x + self->pm_base_x + ad.x;
		dest.top = origin.y + self->pm_base_y + ad.y;

	} else {
	
		dest.left = self->pm_base_x + ad.x;
		dest.top = self->pm_base_y + ad.y;
   	}

	dest.right = dest.left + ad.w;
	dest.bottom = dest.top + ad.h;
	
	if (src->window != NULL) {

		origin.x = 0;
		origin.y = 0;

		if (!ClientToScreen(src->window, &origin))
			return BRE_FAIL;

		source.left = origin.x + src->pm_base_x + as.x;
		source.top = origin.y + src->pm_base_y + as.y;

	} else {
	
		source.left = self->pm_base_x + as.x;
		source.top = self->pm_base_y + as.y;
   	}

	source.right = source.left + as.w;
	source.bottom = source.top + as.h;
	
	/*
	 * Determine the correct blit flags and colour keys
	 *
	 * Could add support for pixelmaps with alpha and depth components
	 */
	flags = DDBLT_WAIT;

	/*
	 * Dodgy, interpreting the copy_function as a flags field!
	 */
	if (self->pm_copy_function & BR_PMCOPY_DST_KEYED) {
		
		flags |= DDBLT_KEYDEST | DDBLT_KEYDESTOVERRIDE;

		bltfx.ddckDestColorkey.dwColorSpaceLowValue = (DWORD)self->pm_dst_key.low;
		bltfx.ddckDestColorkey.dwColorSpaceHighValue = (DWORD)self->pm_dst_key.high;
	}
	
	if (src->pm_copy_function & BR_PMCOPY_SRC_KEYED) {
		
		flags |= DDBLT_KEYSRC | DDBLT_KEYSRCOVERRIDE;

		bltfx.ddckSrcColorkey.dwColorSpaceLowValue = (DWORD)src->pm_dst_key.low;
		bltfx.ddckSrcColorkey.dwColorSpaceHighValue = (DWORD)src->pm_dst_key.high;
	}
	
	/*
	 * Perform the blit
	 */
	do {

		dderr = IDirectDrawSurface2_Blt(self->surface, &dest, src->surface, &source, flags, &bltfx);
			
		switch (dderr) {

		case DD_OK:
			break;

		case DDERR_SURFACELOST:

			/*
			 * Not clear which surface is lost, so restore both.
			 */
			if (IDirectDrawSurface2_Restore(self->surface) != DD_OK ||
				IDirectDrawSurface2_Restore(src->surface) != DD_OK)

				return BRE_FAIL;

			break;

		default:
			return BRE_FAIL;
		}

	} while (dderr != DD_OK);

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, rectangleFill)(br_device_pixelmap *self,
	br_rectangle *r, br_uint_32 colour)
{
	br_rectangle ar;
	POINT origin;
	RECT dest;
	DDBLTFX bltfx = { sizeof(DDBLTFX) };
	HRESULT dderr;

	/*
	 * Clip the region and resolve any sub-pixelmap nonsense
	 *
	 * Should be possible to set up a clipper for sub-pixelmaps and leave all clipping down to
	 * Direct3D
	 */
	if (PixelmapRectangleClip(&ar, r, (br_pixelmap *)self) == BR_CLIP_REJECT)
		return BRE_OK;

	/*
	 * Calculate the destination regions of the surface, offsetting any on pixelmaps
	 * representing windows on the primary surface
	 */
	if (self->window != NULL) {

		origin.x = 0;
		origin.y = 0;

		if (!ClientToScreen(self->window, &origin))
			return BRE_FAIL;

		dest.left = origin.x + self->pm_base_x + ar.x;
		dest.top = origin.y + self->pm_base_y + ar.y;

	} else {
	
		dest.left = self->pm_base_x + ar.x;
		dest.top = self->pm_base_y + ar.y;
   	}

	dest.right = dest.left + ar.w;
	dest.bottom = dest.top + ar.h;
	
	bltfx.dwFillColor = colour;
	
	/*
	 * Perform the blit
	 */
	do {

		/*
		 * Ought to Clear depth buffers - need D3D device first though = problem the first time around!
		 */
		if (self->pm_type == BR_PMT_DEPTH_16)
			dderr = IDirectDrawSurface2_Blt(self->surface, &dest, NULL, NULL, DDBLT_WAIT | DDBLT_DEPTHFILL, &bltfx);
		else
			dderr = IDirectDrawSurface2_Blt(self->surface, &dest, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &bltfx);
			
		switch (dderr) {

		case DD_OK:
			break;

		case DDERR_SURFACELOST:

			if (IDirectDrawSurface2_Restore(self->surface) != DD_OK)
				return BRE_FAIL;

			break;

		default:
			return BRE_FAIL;
		}

	} while (dderr != DD_OK);

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, originSet)(br_device_pixelmap *self, br_point *p)
{
 	self->pm_origin_x = (br_uint_16)p->x;
 	self->pm_origin_y = (br_uint_16)p->y;

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, flush)(struct br_device_pixelmap *self)
{
	/*
	 * At present, I know of no way or need to flush blits or other drawing operations
	 */

	/*
	 * Flush any renderer that is drawing to this surface
	 */
	if (self->renderer != NULL)
		RendererFlush(self->renderer, BR_FALSE);

	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, synchronise)(struct br_device_pixelmap *self,
	br_token sync_type, br_boolean block)
{
	switch (sync_type) {

	case BRT_VERTICAL_BLANK:

		if (IDirectDraw2_WaitForVerticalBlank(self->output_facility->ddraw.ddraw,
			block? DDWAITVB_BLOCKBEGIN: 0, NULL) != DD_OK)

			return BRE_FAIL;

		break;

	case BRT_RENDERER:

		/*
		 * Wait until any blits have finished
		 *
		 * *** There is a busy wait here, which is ***very*** nasty ***
		 */
		if (block)
			while (IDirectDrawSurface2_GetBltStatus(self->surface, DDGBS_ISBLTDONE) == DDERR_WASSTILLDRAWING);
		else
			if (IDirectDrawSurface2_GetBltStatus(self->surface, DDGBS_ISBLTDONE) == DDERR_WASSTILLDRAWING)
				return BRE_FAIL;

		/*
		 * Synchronise any renderer that is drawing to this surface
		 */
		if (self->renderer != NULL)
			RendererSynchronise(self->renderer, sync_type, block);

		break;

	/*
	 * Could add support for synching to flip status
	 */
	default:

		return BRE_UNSUPPORTED;
	}
	
	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, directLock)(struct br_device_pixelmap *self,
	br_boolean block)
{
	DDSURFACEDESC surface_desc = { sizeof(DDSURFACEDESC) };
	RECT region;
	br_uint_16 selector;

#if 1
	/*
	 * MAJOR HACK
	 */
	return BRE_FAIL;
#endif
	
	if (self->parent != NULL) {

		/*
		 * Lock only a region of a sub-pixelmap.
		 */
		region.left = self->pm_base_x;
		region.top = self->pm_base_y;
		region.right = self->pm_base_x + self->pm_width;
		region.bottom = self->pm_base_y + self->pm_height;
	}

	/*
	 * Ensure that a flip is not pending on this lock
	 *
	 * *** Is this necessary?
	 *
	 * *** There is a busy wait here, which is ***very*** nasty ***
	 */
	if (block)
		while (IDirectDrawSurface2_GetFlipStatus(self->surface, DDGFS_ISFLIPDONE) == DDERR_WASSTILLDRAWING);
	else
		if (IDirectDrawSurface2_GetFlipStatus(self->surface, DDGFS_ISFLIPDONE) == DDERR_WASSTILLDRAWING)
			return BRE_FAIL;
	
	/*
	 * Attempt to lock the surface
	 *
	 * *** Ought to try to work the READONLY and WRITEONLY flags in?
	 */
	if (IDirectDrawSurface2_Lock(self->surface, self->parent != NULL? &region: NULL, &surface_desc,
		DDLOCK_SURFACEMEMORYPTR | (block? DDLOCK_WAIT: 0), NULL) != DD_OK)

		return BRE_FAIL;

	/*
	 * Set the pointer, stride and flags in the pixelmap structure
	 */
	self->pm_pixels = (char *)surface_desc.lpSurface;
	self->pm_row_bytes = (br_uint_16)surface_desc.lPitch;

	/*
	 * Indicate that the pixels can be accessed with near references, but set the
	 * selector anyway
	 */
	self->pm_flags |= BR_PMF_PIXELS_NEAR;

	HostSelectorDS(&selector);
	self->pm_pixels_qualifier = selector;
	
	/*
	 * Set linear flag if no gap between rows
	 */
	if (self->pm_row_bytes == (br_int_32)(self->pm_width * PixelTypePixelBytes(self->pm_type)))
		self->pm_flags |= BR_PMF_LINEAR;

	/*
	 * Set wholepixels flag if stride is an integer number of pixels
	 */
	if (self->pm_row_bytes % PixelTypePixelBytes(self->pm_type) == 0)
		self->pm_flags |= BR_PMF_ROW_WHOLEPIXELS;
   
	self->pm_flags &= ~BR_PMF_NO_ACCESS;
		
	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_d3d, directUnlock)(struct br_device_pixelmap *self)
{
	if (IDirectDrawSurface2_Unlock(self->surface, self->pm_pixels) != DD_OK)
		return BRE_FAIL;

	/*
	 * Revert the pointer, stride and flags
	 */
	self->pm_pixels = NULL;
	self->pm_row_bytes = 0;
	self->pm_flags &= ~(BR_PMF_LINEAR | BR_PMF_ROW_WHOLEPIXELS | BR_PMF_PIXELS_NEAR);
	self->pm_flags |= BR_PMF_NO_ACCESS;

	return BRE_OK;
}


/*
 * Routines for operations which are performed by locking the pixelmap and using the memory
 * pixelmap methods (could go into library)
 */
br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, rectangleCopyTo)
	(br_device_pixelmap *self, br_point *p, br_device_pixelmap *src, br_rectangle *r)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, rectangleCopyTo)(self, p, src, r);

	DevicePixelmapDirectUnlock(self);

	return error;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, rectangleCopyFrom)
	(br_device_pixelmap *self, br_point *p, br_device_pixelmap *dest, br_rectangle *r)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, rectangleCopyFrom)(self, p, dest, r);

	DevicePixelmapDirectUnlock(self);

	return error;
}
	
br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, rectangleStretchCopyFrom)
	(br_device_pixelmap *self, br_rectangle *d, br_device_pixelmap *dest, br_rectangle *s)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, rectangleStretchCopyFrom)(self, d, dest, s);

	DevicePixelmapDirectUnlock(self);

	return error;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, rectangleStretchCopyTo)
	(br_device_pixelmap *self, br_rectangle *d, br_device_pixelmap *src, br_rectangle *s)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, rectangleStretchCopyTo)(self, d, src, s);

	DevicePixelmapDirectUnlock(self);

	return error;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, pixelSet)
	(br_device_pixelmap *self, br_point *p, br_uint_32 colour)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, pixelSet)(self, p, colour);

	DevicePixelmapDirectUnlock(self);

	return error;
}


br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, line)
	(br_device_pixelmap *self, br_point *s, br_point *e, br_uint_32 colour)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, line)(self, s, e, colour);

	DevicePixelmapDirectUnlock(self);

	return error;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, copyBits)
	(br_device_pixelmap *self, br_point *point,
	br_uint_8 *src,br_uint_16 s_stride,
	br_rectangle *bit_rect,
	br_uint_32 colour)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, copyBits)(self, point, src, s_stride, bit_rect, colour);

	DevicePixelmapDirectUnlock(self);

	return error;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_lock, pixelQuery)
	(br_device_pixelmap *self, br_uint_32 *pcolour, br_point *p)
{
	br_error error;

	error = DevicePixelmapDirectLock(self, BR_TRUE);
	if (error != BRE_OK)
		return error;

	error = BR_CMETHOD(br_device_pixelmap_mem, pixelQuery)(self, pcolour, p);

	DevicePixelmapDirectUnlock(self);

	return error;
}


/*
 * Routines for unsupported features (could go into library)
 */
br_error BR_CMETHOD_DECL(br_device_pixelmap_fail, rowSize)(br_device_pixelmap *self,
	br_size_t *sizep)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_fail, rowQuery)(br_device_pixelmap *self,
	void *buffer,
	br_size_t buffer_size,
	br_int_32 row)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_fail, rowSet)(br_device_pixelmap *self,
	void *buffer,
	br_size_t buffer_size,
	br_int_32 row)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_fail, pixelAddressQuery)
	(br_device_pixelmap *self, void **pptr, br_uint_32 *pqual, br_point *p)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_device_pixelmap_fail, pixelAddressSet)(br_device_pixelmap *self, void *ptr, br_uint_32 *qual)
{
	return BRE_FAIL;
}


/*
 * Default dispatch table for device pixelmap
 */
static const struct br_device_pixelmap_dispatch devicePixelmapDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_device_pixelmap_d3d, free),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, identifier),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, type),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, isType),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, device),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, space),

	BR_CMETHOD_REF(br_device_pixelmap_d3d, templateQuery),
	BR_CMETHOD_REF(br_object,              query),
	BR_CMETHOD_REF(br_object,              queryBuffer),
	BR_CMETHOD_REF(br_object,              queryMany),
	BR_CMETHOD_REF(br_object,              queryManySize),
	BR_CMETHOD_REF(br_object,              queryAll),
	BR_CMETHOD_REF(br_object,              queryAllSize),

	BR_CMETHOD_REF(br_device_pixelmap_d3d, validSource),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, resize),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, match),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, allocateSub),

	BR_CMETHOD_REF(br_device_pixelmap_gen, copy),
	BR_CMETHOD_REF(br_device_pixelmap_gen, copyTo),
	BR_CMETHOD_REF(br_device_pixelmap_gen, copyFrom),
	BR_CMETHOD_REF(br_device_pixelmap_gen, fill),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, doubleBuffer),

	BR_CMETHOD_REF(br_device_pixelmap_gen, copyDirty),
	BR_CMETHOD_REF(br_device_pixelmap_gen, copyToDirty),
	BR_CMETHOD_REF(br_device_pixelmap_gen, copyFromDirty),
	BR_CMETHOD_REF(br_device_pixelmap_gen, fillDirty),
	BR_CMETHOD_REF(br_device_pixelmap_gen, doubleBufferDirty),

	BR_CMETHOD_REF(br_device_pixelmap_gen, rectangle),
	BR_CMETHOD_REF(br_device_pixelmap_gen, rectangle2),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, rectangleCopy),
	BR_CMETHOD_REF(br_device_pixelmap_lock, rectangleCopyTo),
	BR_CMETHOD_REF(br_device_pixelmap_lock, rectangleCopyFrom),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, rectangleStretchCopy),
	BR_CMETHOD_REF(br_device_pixelmap_lock, rectangleStretchCopyTo),
	BR_CMETHOD_REF(br_device_pixelmap_lock, rectangleStretchCopyFrom),
 	BR_CMETHOD_REF(br_device_pixelmap_d3d, rectangleFill),
	BR_CMETHOD_REF(br_device_pixelmap_lock, pixelSet),
	BR_CMETHOD_REF(br_device_pixelmap_lock, line),
	BR_CMETHOD_REF(br_device_pixelmap_lock, copyBits),

	BR_CMETHOD_REF(br_device_pixelmap_gen, text),
	BR_CMETHOD_REF(br_device_pixelmap_gen, textBounds),

	BR_CMETHOD_REF(br_device_pixelmap_fail, rowSize),
	BR_CMETHOD_REF(br_device_pixelmap_fail, rowQuery),
	BR_CMETHOD_REF(br_device_pixelmap_fail, rowSet),

	BR_CMETHOD_REF(br_device_pixelmap_lock, pixelQuery),
	BR_CMETHOD_REF(br_device_pixelmap_fail, pixelAddressQuery),

	BR_CMETHOD_REF(br_device_pixelmap_fail, pixelAddressSet),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, originSet),

	BR_CMETHOD_REF(br_device_pixelmap_d3d, flush),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, synchronise),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, directLock),
	BR_CMETHOD_REF(br_device_pixelmap_d3d, directUnlock),
};

