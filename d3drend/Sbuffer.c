/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Sbuffer.c 1.3 1997/08/12 17:27:48 jon Exp JON $
 * $Locker: JON $
 *
 * Stored buffer methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "host.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: Sbuffer.c 1.3 1997/08/12 17:27:48 jon Exp JON $");


/*
 * Default dispatch table for primitive state (defined at end of file)
 */
static struct br_buffer_stored_dispatch bufferStoredDispatch;


/*
 * Primitive state info. template
 */
#define F(f) offsetof(struct br_buffer_stored, f)

static struct br_tv_template_entry bufferStoredTemplateEntries[] = {
	{ BRT_IDENTIFIER_CSTR,	0,	F(identifier),		BRTV_QUERY | BRTV_ALL,	BRTV_CONV_COPY, },
};
#undef F


/*
 * Set up a static device object
 */
struct br_buffer_stored *BufferStoredD3DAllocate(br_renderer *renderer,
	br_token use, struct br_device_pixelmap *pm, br_token_value *tv)
{
	struct br_buffer_stored *self;
	char *ident;
		
	switch (use) {

	case BRT_TEXTURE_O:
	case BRT_COLOUR_MAP_O:
		ident ="Colour-Map";
		break;

	case BRT_INDEX_SHADE_O:
		ident ="Shade-Table";
		break;
	
	case BRT_INDEX_BLEND_O:
		ident ="Blend-Table";
		break;
	
	case BRT_SCREEN_DOOR_O:
		ident ="Screendoor-Table";
		break;
	
	case BRT_INDEX_LIGHT_O:
		ident ="Lighting-Table";
		break;
	
	case BRT_BUMP_O:
		ident ="Bump-Map";
		break;

	case BRT_UNKNOWN:
		ident ="Unknown";
		break;

	default:
		return NULL;
	}

	self = BrResAllocate(renderer, sizeof(*self), BR_MEMORY_OBJECT);

	if (self == NULL)
		return NULL;

	self->device = renderer->device;

	self->dispatch = &bufferStoredDispatch;
	self->identifier = ident;
	self->renderer = renderer;

	/*
	 * Should get this flag from the token-value arguments
	 */
	self->flags |= SBUFF_SHARED;

	BufferStoredUpdate(self, pm, tv);
	
	ObjectContainerAddFront(renderer, (br_object *)self);

	return self;
}

static br_error BR_CMETHOD_DECL(br_buffer_stored_d3d, update)(
	struct br_buffer_stored *self,
	struct br_device_pixelmap *pm,
	br_token_value *tv)
{
	LPD3DDEVICEDESC tri_desc;
	DDSURFACEDESC desc = { sizeof(DDSURFACEDESC) };
	LPDDPIXELFORMAT stored_format;
	br_uint_16 stored_width, stored_height;
	br_boolean convert, stretch, alloconload;
	LPDIRECTDRAWSURFACE surface;
	LPDIRECTDRAWSURFACE2 dest_surface;
	br_device *pm_device;
	br_uint_16 ds;
	DDBLTFX bltfx = { sizeof(DDBLTFX) };
	HRESULT dderr;
	br_uint_32 i;

	/*
	 * Release any old interfaces
	 */
	self->handle = 0;
	
	if (self->stored_texture != NULL) {
		IDirect3DTexture2_Release(self->stored_texture);
		self->stored_texture = NULL;
	}

	if (self->stored_surface != NULL) {
		IDirectDrawSurface2_Release(self->stored_surface);
		self->stored_surface = NULL;
	}

	if (self->source_texture != NULL) {
		IDirect3DTexture2_Release(self->source_texture);
		self->stored_texture = NULL;
	}

	if (self->source_surface != NULL) {
		IDirectDrawSurface2_Release(self->source_surface);
		self->source_surface = NULL;
	}
	
	self->source = (br_pixelmap *)pm;

	/*
	 * Find out where the pixelmap comes from
	 */
	pm_device = ObjectDevice(pm);
	
	if (pm_device == NULL) {

		/*
		 * The pixelmap is a plain BRender memory pixelmap.  Make sure that the pixels can be accessed
		 */
		if (pm->pm_flags & BR_PMF_NO_ACCESS || pm->pm_pixels == NULL)
			return BRE_FAIL;
		
		/*
		 * Make sure that the pixelmap has a near pointer
		 * N.B. Could try to map area in
		 */
		if (!(pm->pm_flags & BR_PMF_PIXELS_NEAR)) {

			HostSelectorDS(&ds);

			if (pm->pm_pixels_qualifier != ds)
				return BRE_FAIL;
		}

		/*
		 * If the format or size of this pixelmap is not directly supported, find an alternative
		 */
#if 1
		convert = BR_FALSE;

		if (PixelTypeToDDPixelFormat(&stored_format, pm->pm_type) != BRE_OK)
			return BRE_FAIL;
#else
		if (!self->renderer->texture_pixel_type[pm->pm_type].supported)
			return BRE_FAIL;

		convert = self->renderer->texture_pixel_type[pm->pm_type].convert;
		stored_format = self->renderer->texture_pixel_type[pm->pm_type].pixel_format;
#endif

		stored_width = pm->pm_width;
		stored_height = pm->pm_height;
		stretch = BR_FALSE;
		//...

		/*
		 * Work out whether we will be using the hal or the hel
		 */
		if (self->renderer->renderer_facility->d3d.hal_desc->dwFlags & D3DDD_TRICAPS)
			tri_desc = self->renderer->renderer_facility->d3d.hal_desc;
		else
			tri_desc = self->renderer->renderer_facility->d3d.hel_desc;

		/*
		 * Create the texture surface
		 */
		desc.dwFlags =  DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		desc.dwHeight = stored_height;
		desc.dwWidth = stored_width;
		desc.ddpfPixelFormat = *stored_format;

		surface = NULL;
		alloconload = BR_FALSE;

		/*
		 * N.B. Current strategy is always to try local video memory first because it is currently
		 * faster on everything
		 */
		
		/*
		 * Try to create an ALLOCONLOAD surface in local video memory
		 *
		 * *** Ought to implement a texture cache in case VRAM can't hold all of the textures ***
		 */
#if 1
		if (surface == NULL) {
#else
		if (surface == NULL && tri_desc->dwDevCaps & D3DDEVCAPS_TEXTUREVIDEOMEMORY) {
#endif

			desc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM | DDSCAPS_TEXTURE | DDSCAPS_ALLOCONLOAD;

			if (IDirectDraw2_CreateSurface(self->renderer->renderer_facility->d3d.ddraw, &desc, &surface, NULL) == DD_OK)
				alloconload = BR_TRUE;
		}

		/*
		 * Try to allocate a non-local video memory surface
		 */
#if 1
		if (surface == NULL) {
#else
		if (surface == NULL && tri_desc->dwDevCaps & D3DDEVCAPS_TEXTURENONLOCALVIDMEM) {
#endif

			desc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_NONLOCALVIDMEM | DDSCAPS_TEXTURE;

			IDirectDraw2_CreateSurface(self->renderer->renderer_facility->d3d.ddraw, &desc, &surface, NULL);
		}				

		/*
		 * Try system memory
		 */
#if 1
		if (surface == NULL) {
#else
		if (surface == NULL && tri_desc->dwDevCaps & D3DDEVCAPS_TEXTURESYSTEMMEMORY) {
#endif

			desc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;

			IDirectDraw2_CreateSurface(self->renderer->renderer_facility->d3d.ddraw, &desc, &surface, NULL);
		}

		if (surface == NULL)
			return BRE_FAIL;

		/*
		 * Get the new version of the surface interface and the texture interface
		 */
		if (IDirectDrawSurface_QueryInterface(surface, &IID_IDirectDrawSurface2, &self->stored_surface) != DD_OK) {
			IDirectDrawSurface_Release(surface);
			return BRE_FAIL;
		}

		IDirectDrawSurface_Release(surface);

		if (IDirectDrawSurface2_QueryInterface(self->stored_surface, &IID_IDirect3DTexture2, &self->stored_texture) != DD_OK)
			return BRE_FAIL;

		/*
		 * Create an intermediate surface for ALLOCONLOAD textures, or where the stored surface
		 * is compressed
		 */
		if (alloconload || (stored_format->dwFlags & DDPF_COMPRESSED) != 0) {

			desc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_TEXTURE;
			desc.ddpfPixelFormat.dwFlags &= ~DDPF_COMPRESSED;

			if (IDirectDraw2_CreateSurface(self->renderer->renderer_facility->d3d.ddraw, &desc, &surface, NULL) != DD_OK)
				return BRE_FAIL;

			/*
			 * Get the new version of the surface interface and the texture interface
			 */
			if (IDirectDrawSurface_QueryInterface(surface, &IID_IDirectDrawSurface2, &self->source_surface) != DD_OK) {
				IDirectDrawSurface_Release(surface);
				return BRE_FAIL;
			}

			IDirectDrawSurface_Release(surface);

			if (IDirectDrawSurface2_QueryInterface(self->source_surface, &IID_IDirect3DTexture2, &self->source_texture) != DD_OK)
				return BRE_FAIL;

			dest_surface = self->source_surface;
				
		} else

			/*
			 * Otherwise copy/convert directly to the texture
			 */
			dest_surface = self->stored_surface;
			
		/*
		 * Copy or convert the data as necessary
		 */
		if (convert || stretch) {

			/*
			 * If format or size conversion is necessary, use DirectDraw blit, Win32 blit or custom code
			 * to do it
			 */
			// Copy from source_surface into stored_surface, performing format and size conversion
			//...
			
		} else {

			IDirectDrawSurface2_Lock(dest_surface, NULL, &desc, DDLOCK_NOSYSLOCK | DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL);

			for (i = 0; i < pm->pm_height; i++)
				memcpy((char *)desc.lpSurface + i * desc.lPitch, (char *)pm->pm_pixels + i * pm->pm_row_bytes, pm->pm_width * PixelTypePixelBytes(pm->pm_type));
				
			IDirectDrawSurface2_Unlock(dest_surface, NULL);
		}

		/*
		 * If the intermediate pixelmap was only created for a non-ALLOCONLOAD compressed texture,
		 * copy it across and free it
		 */
		if (!alloconload && stored_format->dwFlags & DDPF_COMPRESSED) {

			/*
			 * Just copy straight across
			 */
			do {
				dderr = IDirectDrawSurface2_Blt(self->stored_surface, NULL, self->source_surface, NULL, DDBLT_WAIT, &bltfx);
				
				switch (dderr) {

				case DD_OK:
					break;

				case DDERR_SURFACELOST:

					/*
					 * Not clear which surface is lost, so restore both.
					 */
					if (IDirectDrawSurface2_Restore(self->stored_surface) != DD_OK ||
						IDirectDrawSurface2_Restore(self->source_surface) != DD_OK)

						return BRE_FAIL;

					break;

				default:
					return BRE_FAIL;
				}

			} while (dderr != DD_OK);

			IDirectDrawSurface2_Release(self->source_surface);
			self->source_surface = NULL;
		}

		/*
		 * *** SHOULD DEAL WITH ANY PALETTES! ***
		 */

		/*
		 * Set the alloc on load flag
		 */
		if (alloconload)
			self->flags |= SBUFF_ALLOCONLOAD;
		else
			self->flags &= ~SBUFF_ALLOCONLOAD;

		
	} else if (pm_device != self->device) {

		/*
		 * The pixelmaps came from this device.  We assume it to be valid texture already.  Use it
		 * directly and get the texture interface
		 */
		self->stored_surface = pm->surface;
		IDirectDrawSurface2_AddRef(pm->surface);

		if (IDirectDrawSurface2_QueryInterface(self->stored_surface, &IID_IDirect3DTexture2, &self->stored_texture) != DD_OK)
			return BRE_FAIL;
		
	} else

		/*
		 * The pixelmap is from another device, we can't use it
		 */
		return BRE_FAIL;

	return BRE_OK;
}

static void BR_CMETHOD_DECL(br_buffer_stored_d3d, free)(br_buffer_stored *self)
{
	/*
	 * Release any interfaces
	 */
	if (self->stored_texture != NULL) {
		IDirect3DTexture2_Release(self->stored_texture);
		self->stored_texture = NULL;
	}

	if (self->stored_surface != NULL) {
		IDirectDrawSurface2_Release(self->stored_surface);
		self->stored_surface = NULL;
	}

	if (self->source_texture != NULL) {
		IDirect3DTexture2_Release(self->source_texture);
		self->source_texture = NULL;
	}

	if (self->source_surface != NULL) {
		IDirectDrawSurface2_Release(self->source_surface);
		self->source_surface = NULL;
	}
	
	ObjectContainerRemove(self->renderer, (br_object *)self);
}

static char *BR_CMETHOD_DECL(br_buffer_stored_d3d, identifier)(br_buffer_stored *self)
{
	return self->identifier;
}

static br_token BR_CMETHOD_DECL(br_buffer_stored_d3d, type)(br_buffer_stored *self)
{
	return BRT_BUFFER_STORED;
}

static br_boolean BR_CMETHOD_DECL(br_buffer_stored_d3d, isType)(br_buffer_stored *self, br_token t)
{
	return (t == BRT_BUFFER_STORED) || (t == BRT_OBJECT);
}

static br_device *BR_CMETHOD_DECL(br_buffer_stored_d3d, device)(br_buffer_stored *self)
{
	return self->device;
}

static br_int_32 BR_CMETHOD_DECL(br_buffer_stored_d3d, space)(br_buffer_stored *self)
{
	return BrResSizeTotal(self);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_buffer_stored_d3d,templateQuery)
	(br_buffer_stored *self)
{
    if(self->device->templates.bufferStoredTemplate == NULL)
        self->device->templates.bufferStoredTemplate = BrTVTemplateAllocate(self->device,
            bufferStoredTemplateEntries,
            BR_ASIZE(bufferStoredTemplateEntries));

    return self->device->templates.bufferStoredTemplate ;
}

/*
 * Default dispatch table for device
 */
static struct br_buffer_stored_dispatch bufferStoredDispatch = {
	NULL,
	NULL,
	NULL,
	NULL,
	BR_CMETHOD_REF(br_buffer_stored_d3d,	free),
	BR_CMETHOD_REF(br_buffer_stored_d3d,	identifier),
	BR_CMETHOD_REF(br_buffer_stored_d3d,	type),
	BR_CMETHOD_REF(br_buffer_stored_d3d,	isType),
	BR_CMETHOD_REF(br_buffer_stored_d3d,	device),
	BR_CMETHOD_REF(br_buffer_stored_d3d,	space),

	BR_CMETHOD_REF(br_buffer_stored_d3d,	templateQuery),
	BR_CMETHOD_REF(br_object,				query),
	BR_CMETHOD_REF(br_object, 				queryBuffer),
	BR_CMETHOD_REF(br_object, 				queryMany),
	BR_CMETHOD_REF(br_object, 				queryManySize),
	BR_CMETHOD_REF(br_object, 				queryAll),
	BR_CMETHOD_REF(br_object,	 			queryAllSize),

	BR_CMETHOD_REF(br_buffer_stored_d3d,	update),
};

