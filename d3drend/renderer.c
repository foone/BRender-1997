/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: renderer.c 1.3 1997/08/11 14:25:57 jon Exp JON $
 * $Locker: JON $
 *
 * Renderer methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: renderer.c 1.3 1997/08/11 14:25:57 jon Exp JON $");

/*
 * Default dispatch table for renderer (defined at end of file)
 */
static const struct br_renderer_dispatch rendererDispatch;

/*
 * 0 terminated list of tokens that are used to represent the state for
 * this library
 */
const br_token RendererPartsTokens[] = {
	BRT_CULL,
	BRT_SURFACE,
	BRT_MATRIX,
	BRT_ENABLE,
	BRT_LIGHT,
	BRT_CLIP,
	0
};

/*
 * Renderer info. template
 */

/*
 * Return a token list of ourself and that of the primitive library
 */
static br_error BR_CALLBACK customPartsQuery(br_uint_32 *pvalue, void **pextra, br_size_t *pextra_size,
	void *block, struct br_tv_template_entry *tep)
{
	struct br_renderer *self = block;
	int i;
	br_uint_32 dummy;
	br_error r;
	br_token **ppt = (br_token **)pextra;

	if(pextra == NULL || *pextra == NULL || pextra_size == NULL)
		return BRE_FAIL;

	*((void **)pvalue) = *pextra;

	/*
	 * Check there is space 
	 */
	if(((BR_ASIZE(RendererPartsTokens)-1) * sizeof(br_token)) > *pextra_size)
		return BRE_FAIL;

	/*
	 * Add our own tokens to list
	 */
	for(i=0; i < BR_ASIZE(RendererPartsTokens)-1; i++) {
		*(*ppt)++ = RendererPartsTokens[i];
		*pextra_size -= sizeof(br_token);
	}
	
	/*
	 * Hand off to primitive library for rest of list
	 */
	r = ObjectQueryBuffer(self->plib, &dummy, *ppt, *pextra_size, BRT_PARTS_TL);

	if(r != BRE_OK)
		return r;

	/*
	 * Work out how much space was consumed
	 */
	for(i = 0; (*ppt)[i] ; i++)
		*pextra_size -= sizeof(br_token);
		
	/*
	 * NULL
	 */
	*pextra_size -= sizeof(br_token);
		
	return BRE_OK;
}

static br_size_t BR_CALLBACK customPartsExtra(void *block, struct br_tv_template_entry *tep)
{
	struct br_renderer *self = block;
	br_size_t s;
	br_token_value tv[] = {
		{BRT_PARTS_TL,0},
		{0,0}
	};

	/*
	 * Find out size of primitive's list
	 */
	if(ObjectQueryManySize((br_object *)self->plib, &s, tv) != BRE_OK)
		return 0;

	/*
	 * Add our own contribution
	 */
	return s + (BR_ASIZE(RendererPartsTokens)-1) * sizeof(br_token);
}

static struct br_tv_custom customPartsConv = {
	customPartsQuery,
	NULL,
	customPartsExtra,
};

#define F(f)	offsetof(struct br_renderer, f)

/*
 * Shortcuts for template flags
 */
#define _S BRTV_SET
#define _Q BRTV_QUERY
#define _A BRTV_ALL

#if BASED_FIXED
#define _AX BRTV_ALL
#else 
#define _AX 0
#endif

#if BASED_FLOAT
#define _AF BRTV_ALL
#else 
#define _AF 0
#endif

static struct br_tv_template_entry rendererTemplateEntries[] = {
	{BRT(IDENTIFIER_CSTR),		F(identifier),				_Q | _A,	BRTV_CONV_COPY, },
	{BRT(PARTS_TL),				0,							_Q | _A,	BRTV_CONV_CUSTOM, (br_int_32)&customPartsConv},
};
#undef F

static HRESULT FAR PASCAL enumTextureFormat(LPDDSURFACEDESC surface_desc, struct br_renderer *renderer);
static br_boolean betterTextureFormat(br_uint_32 pixel_type, LPDDPIXELFORMAT old_pixel_format, LPDDPIXELFORMAT new_pixel_format);

/*
 * Create a new renderer
 */
br_renderer *RendererD3DAllocate(br_device *device, struct br_renderer_facility *facility, struct br_device_pixelmap *dest)
{
	br_renderer *self;
	DDSURFACEDESC surface_desc = { sizeof(DDSURFACEDESC) };
	LPDIRECTDRAWSURFACE target, depth;
	br_boolean attached_depth = BR_FALSE;
	br_uint_32 i;

	/*
	 * Check that destination is valid
	 */
	if (dest == NULL || ObjectDevice(dest) != device)
		return NULL;
	
	self = BrResAllocate(facility, sizeof(*self), BR_MEMORY_OBJECT);

	if (self == NULL)
		return NULL;

	self->dispatch = (struct br_renderer_dispatch *)&rendererDispatch;
 	self->identifier = facility->identifier;
    self->device = device;
 	self->renderer_facility = facility;

	/*
	 * Get a valid rendering target from which to create the device, and use the IDirectDrawSurface
	 * interface
	 */
	if (IDirectDrawSurface2_GetSurfaceDesc(dest->surface, &surface_desc) != DD_OK) {
		BrResFree(self);
		return NULL;
	}

	if (surface_desc.ddsCaps.dwCaps & DDSCAPS_3DDEVICE) {

		/*
		 * Use the screen buffer if it is a valid rendering target
		 */
		if (IDirectDrawSurface2_QueryInterface(dest->surface, &IID_IDirectDrawSurface, &target) != DD_OK) {
			BrResFree(self);
			return NULL;
		}

	} else if (dest->offscreen_surface != NULL) {
	
		/*
		 * Use an offscreen buffer matched from the screen, if any
		 */
		if (IDirectDrawSurface2_QueryInterface(dest->offscreen_surface, &IID_IDirectDrawSurface, &target) != DD_OK) {
			BrResFree(self);
			return NULL;
		}

	} else {

		/*
		 * Otherwise try to allocate a temporary one
		 *
		 * Leave the pixel format the same as the screen and make the width and height the same as
		 * the screen pixelmap (not the same as the surface if windowed) - should try smaller if
		 * the allocation fails (in case the image is intended to be stretched)
		 *
		 * Should use renderer caps to determine what sort of memory to use
		 */
		surface_desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		surface_desc.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER | DDSCAPS_3DDEVICE;
		surface_desc.dwWidth = dest->pm_width;
		surface_desc.dwHeight = dest->pm_height;

		if (IDirectDraw2_CreateSurface(dest->output_facility->ddraw.ddraw, &surface_desc, &target, NULL) != DD_OK) {
			BrResFree(self);
			return NULL;
		}
	}
		
	/*
	 * Get a valid depth target (I understand that this is necessary to ensure that the device is
	 * capable of rendering to depth buffers)
	 */
	if (dest->depth_surface != NULL) {
	
		/*
		 * Use a first depth buffer matched from the screen or an offscreen buffer from the screen, if any
		 */
		if (IDirectDrawSurface2_QueryInterface(dest->depth_surface, &IID_IDirectDrawSurface, &depth) != DD_OK) {
			BrResFree(self);
			return NULL;
		}

	} else {

		/*
		 * Otherwise try to allocate a temporary one and attach it to the target
		 *
		 * Leave the width and height the same as the offscreen buffer and choose a common bit depth (FIX!)
		 *
		 * Should use renderer caps to determine what sort of memory to use
		 */
		if (IDirectDrawSurface_GetSurfaceDesc(target, &surface_desc) != DD_OK) {
			IDirectDrawSurface_Release(target);
			BrResFree(self);
			return NULL;
		}

		surface_desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_ZBUFFERBITDEPTH;
		surface_desc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
		surface_desc.dwZBufferBitDepth = 16;

		if (IDirectDraw2_CreateSurface(dest->output_facility->ddraw.ddraw, &surface_desc, &depth, NULL) == DD_OK) {

			IDirectDrawSurface2_AddAttachedSurface(target, depth);
			attached_depth = BR_TRUE;

		} else

			depth = NULL;
	}
		
	/*
	 * Create the D3D device
	 */
	if (IDirect3D2_CreateDevice(self->renderer_facility->d3d.d3d, self->renderer_facility->d3d.guid, target, &self->d3d_device) != DD_OK) {
		if (depth != NULL) {
			if (attached_depth)
				IDirectDrawSurface_DeleteAttachedSurface(target, 0, depth);
			IDirectDrawSurface_Release(depth);
		}
		IDirectDrawSurface_Release(target);
		BrResFree(self);
		return NULL;
	}

	/*
	 * TEMPORARY - Set the global state up
	 */
	if (SetD3DGlobalState(self) != BRE_OK) {
		if (depth != NULL) {
			if (attached_depth)
				IDirectDrawSurface_DeleteAttachedSurface(target, 0, depth);
			IDirectDrawSurface_Release(depth);
		}
		IDirectDrawSurface_Release(target);
		BrResFree(self);
		return NULL;
	}

	/*
	 * Compile a list of the best texture format for each BRender pixelmap type
	 */
	for (i = 0; i < BR_ASIZE(self->texture_pixel_type); i++)
		self->texture_pixel_type[i].supported = BR_FALSE;

	IDirect3DDevice2_EnumTextureFormats(self->d3d_device, enumTextureFormat, self);

	/*
	 * Create the viewport
	 *
	 * N.B. I may end up keeping a separate viewport with each pixelmap
	 */
	if (IDirect3D2_CreateViewport(self->renderer_facility->d3d.d3d, &self->viewport, NULL) != DD_OK) {
		BrResFree(self);
		return NULL;
	}

	if (IDirect3DDevice2_AddViewport(self->d3d_device, self->viewport) != DD_OK) {
		BrResFree(self);
		return NULL;
	}

	if (IDirect3DDevice2_SetCurrentViewport(self->d3d_device, self->viewport) != DD_OK) {
		BrResFree(self);
		return NULL;
	}

	/*
	 * Set up out object list
	 */
	self->object_list = BrObjectListAllocate(self);

	/*
	 * Remember default state
	 */
	self->default_state = &facility->default_state;
 	self->state.renderer = self;

	/*
	 * State starts out as default
	 */
	RendererStateDefault(self, (br_uint_32)BR_STATE_ALL);

	/*
	 * Associate renderer with screen pixelmap
	 */
	dest->screen->renderer = self;	

	/*
	 * *** TEMPORARY ***
	 *
	 * Initialise the static brp_block, and the scales and ranges
	 */
	InitBrpBlock(self);

	ObjectContainerAddFront(facility, (br_object *)self);

	return self;
}


static HRESULT FAR PASCAL enumTextureFormat(LPDDSURFACEDESC surface_desc, struct br_renderer *renderer)
{
	br_uint_32 i;
	br_uint_32 type;

	/*
	 * See if this corresponds directly to a BRender pixel type
	 */
	if (DDPixelFormatToPixelType(&type, &surface_desc->ddpfPixelFormat) != BRE_OK)
		type = BR_ASIZE(renderer->texture_pixel_type);

	/*
	 * For each BRender pixel format, see if this texture format is a suitable substitute
	 */
	for (i = 0; i < BR_ASIZE(renderer->texture_pixel_type); i++) {

		/*
		 * If this is the ideal match, use it
		 */
		if (i == type) {

			if (renderer->texture_pixel_type[i].pixel_format != NULL)
				BrResFree(renderer->texture_pixel_type[i].pixel_format);

			PixelTypeToDDPixelFormat(&renderer->texture_pixel_type[i].pixel_format, type);

			renderer->texture_pixel_type[i].supported = BR_TRUE;
			renderer->texture_pixel_type[i].convert = BR_FALSE;

			continue;
		}

		/*
		 * If an ideal match has already been found, ignore
		 */
		if (renderer->texture_pixel_type[i].supported && !renderer->texture_pixel_type[i].convert)
			continue;

		/*
		 * If this a better match than the previous one, use it
		 *
		 * N.B. Could cache some of the details determined by betterTextureFormat for the conversion
		 * code to use
		 */
		if (betterTextureFormat(i, renderer->texture_pixel_type[i].pixel_format, &surface_desc->ddpfPixelFormat)) {

			/*
			 * If a pixel format already exists, it must have been allocated (unless it was ideally
			 * matched already in which case it wouldn't reach this code)
			 */
			if (renderer->texture_pixel_type[i].pixel_format == NULL) {

				renderer->texture_pixel_type[i].pixel_format = BrResAllocate(renderer, sizeof(*renderer->texture_pixel_type[i].pixel_format), BR_MEMORY_OBJECT_DATA);
				if (renderer->texture_pixel_type[i].pixel_format == NULL)
					continue;
			}

			BrMemCpy(renderer->texture_pixel_type[i].pixel_format, &surface_desc->ddpfPixelFormat, sizeof(*renderer->texture_pixel_type[i].pixel_format));

			renderer->texture_pixel_type[i].supported = BR_TRUE;
			renderer->texture_pixel_type[i].convert = BR_TRUE;
		}
	}

	return DDENUMRET_OK;
}

/*
 * Pixel format flags that are significant for comparison purposes for pixel format and channels
 */
#define FORMAT_DDPF_MASK (DDPF_PALETTEINDEXED1 | DDPF_PALETTEINDEXED2 | DDPF_PALETTEINDEXED4 |\
		DDPF_PALETTEINDEXED8 | DDPF_PALETTEINDEXEDTO8 | DDPF_RGB | DDPF_YUV)
		
#define CHANNELS_DDPF_MASK (DDPF_ALPHA | DDPF_ALPHAPIXELS | DDPF_ZBUFFER | DDPF_ZPIXELS)
		
/*
 * Convert the mask for a channel into a number of bits
 */
static br_uint_32 ChannelMaskToBits(br_uint_32 mask)
{
	br_uint_32 bits;

	/*
	 * Count the number of bits in the mask.  N.B. Bits don't have to be consecutive (e.g. in Y the
	 * case of YUYV pixel formats) - this should be handled by the conversion code
	 */
	bits = 0;

	while (mask != 0) {

		if ((mask & 1) != 0)
			bits++;

		mask >>= 1;
	}

	return bits;
}
 

/*
 * Determine if a particular DirectDraw pixel format is a better match for a BRender pixel format than
 * another
 */
static br_boolean betterTextureFormat(br_uint_32 pixel_type, LPDDPIXELFORMAT old_pixel_format, LPDDPIXELFORMAT new_pixel_format)
{
	LPDDPIXELFORMAT ideal_pixel_format;
	br_int_32 r_bits, g_bits, b_bits, a_bits;
	br_int_32 bits_lost;
	br_uint_32 new_bits_lost, old_bits_lost;

	/*
	 * Get the pixel format for the BRender pixel type
	 *
	 * If no valid pixel format exists, there isn't going to be a format that can be converted to it
	 * as far as I can work out
	 */
	if (PixelTypeToDDPixelFormat(&ideal_pixel_format, pixel_type) != BRE_OK)
		return BR_FALSE;

	/*
	 * If the new format is totally unsuitable, reject
	 *
	 * Grounds for rejection are:
	 *
	 *	Wrong number of index bits (could this be relaxed to same or higher?)
	 *  Wrong colour format (including indexedto8)
	 *	Required colour, alpha or depth channel not supported (unnecessary channels aren't a problem
	 *  since they can be filled with 1s (alpha and colour) or 0s (depth) during conversion)
	 *
	 * N.B. Compressed surfaces aren't a problem as we copy into them later
	 */
	if ((ideal_pixel_format->dwFlags & FORMAT_DDPF_MASK) != 0 && 
		(ideal_pixel_format->dwFlags & FORMAT_DDPF_MASK) != (new_pixel_format->dwFlags & FORMAT_DDPF_MASK))
		return BR_FALSE;

	switch (ideal_pixel_format->dwFlags & CHANNELS_DDPF_MASK) {

	case 0:
		if ((new_pixel_format->dwFlags & (DDPF_ALPHA | DDPF_ZBUFFER)) != 0)
			return BR_FALSE;
		break;

	case DDPF_ALPHA:

		if ((new_pixel_format->dwFlags & (DDPF_ALPHA | DDPF_ALPHAPIXELS)) == 0)
			return BR_FALSE;
		break;

	case DDPF_ALPHAPIXELS:

		if ((new_pixel_format->dwFlags & DDPF_ALPHAPIXELS) == 0)
			return BR_FALSE;
		break;

	case DDPF_ZBUFFER:

		if ((new_pixel_format->dwFlags & (DDPF_ZBUFFER | DDPF_ZPIXELS)) == 0)
			return BR_FALSE;
		break;

	case DDPF_ZPIXELS:

		if ((new_pixel_format->dwFlags & DDPF_ZPIXELS) == 0)
			return BR_FALSE;
		break;

	default:
		return BR_FALSE;
	}

	/*
	 * If there was an old format, see if the new format is better, and reject if not
	 *
	 * Criteria for being a better match are:
	 *
	 *	Fewer bits of accuracy dropped, or
	 *	Fewer bits required
	 *	Being compressed
	 *
	 * At the moment bits lost in alpha channel are weighted twice as heavily as bits lost in
	 * colour channels
	 */
	if (old_pixel_format != NULL) {

		/*
		 * Determine the number of bits of accuracy lost in total for the new and old formats (could
		 * cache value for old format)
		 */
		r_bits = PixelTypeRBits(pixel_type);
		g_bits = PixelTypeGBits(pixel_type);
		b_bits = PixelTypeBBits(pixel_type);
		a_bits = PixelTypeABits(pixel_type);

		new_bits_lost = 0;

		bits_lost = r_bits - ChannelMaskToBits(new_pixel_format->dwRBitMask);
		if (bits_lost > 0)
			new_bits_lost += bits_lost;

		bits_lost = g_bits - ChannelMaskToBits(new_pixel_format->dwGBitMask);
		if (bits_lost > 0)
			new_bits_lost += bits_lost;

		bits_lost = b_bits - ChannelMaskToBits(new_pixel_format->dwBBitMask);
		if (bits_lost > 0)
			new_bits_lost += bits_lost;

		bits_lost = a_bits - ChannelMaskToBits(new_pixel_format->dwRGBAlphaBitMask);
		if (bits_lost > 0)
			new_bits_lost += bits_lost * 2;

		old_bits_lost = 0;

		bits_lost = r_bits - ChannelMaskToBits(old_pixel_format->dwRBitMask);
		if (bits_lost > 0)
			old_bits_lost += bits_lost;

		bits_lost = g_bits - ChannelMaskToBits(old_pixel_format->dwGBitMask);
		if (bits_lost > 0)
			old_bits_lost += bits_lost;

		bits_lost = b_bits - ChannelMaskToBits(old_pixel_format->dwBBitMask);
		if (bits_lost > 0)
			old_bits_lost += bits_lost;

		bits_lost = a_bits - ChannelMaskToBits(old_pixel_format->dwRGBAlphaBitMask);
		if (bits_lost > 0)
			old_bits_lost += bits_lost * 2;

		/*
		 * If the new format loses more bits of accuracy, it isn't better
		 */
		if (new_bits_lost > old_bits_lost)
			return BR_FALSE;

		/*
		 * If the new format loses the same number bits of accuracy but uses more bits total,
		 * it isn't better
		 */
		if (new_bits_lost == old_bits_lost && new_pixel_format->dwRGBBitCount > old_pixel_format->dwRGBBitCount)
			return BR_FALSE;

		/*
		 * If the old format was compressed, it isn't better
		 */
		if (old_pixel_format->dwFlags & DDPF_COMPRESSED)
			return BR_FALSE;
	}

	/*
	 * If all else fails, it must be better!
	 */
	return BR_TRUE;
}


static void BR_CMETHOD_DECL(br_renderer_d3d, free)(br_renderer *self)
{
	ObjectContainerRemove(self->renderer_facility, (br_object *)self);

	/*
	 * Free Direct3D device
	 */
	if (self->d3d_device != NULL)
		IDirect3DDevice2_Release(self->d3d_device);
	
	self->renderer_facility->num_instances--;

	BrObjectContainerFree((br_object_container *)self, BR_NULL_TOKEN, NULL, NULL);

	BrResFreeNoCallback(self);
}

static char *BR_CMETHOD_DECL(br_renderer_d3d, identifier)(br_renderer *self)
{
	return self->identifier;
}

static br_token BR_CMETHOD_DECL(br_renderer_d3d, type)(br_renderer *self)
{
	return BRT_RENDERER;
}

static br_boolean BR_CMETHOD_DECL(br_renderer_d3d, isType)(br_renderer *self, br_token t)
{
	return (t == BRT_RENDERER) || (t == BRT_OBJECT);
}

static struct br_device *BR_CMETHOD_DECL(br_renderer_d3d, device)(br_renderer *self)
{
	return self->device;
}

static br_int_32 BR_CMETHOD_DECL(br_renderer_d3d, space)(br_renderer *self)
{
	return sizeof(br_renderer);
}

static struct br_tv_template * BR_CMETHOD_DECL(br_renderer_d3d,templateQuery)
	(br_renderer *self)
{
    if(self->device->templates.rendererTemplate == NULL)
        self->device->templates.rendererTemplate = BrTVTemplateAllocate(self->device,
            rendererTemplateEntries,
            BR_ASIZE(rendererTemplateEntries));

    return self->device->templates.rendererTemplate;
}

static void * BR_CMETHOD_DECL(br_renderer_d3d,listQuery)(br_device *self)
{
	return self->object_list;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, validDestination)(br_renderer *self, br_boolean *bp, br_object *h)
{
	return BRE_OK;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, stateStoredNew)
	(struct br_renderer *self, struct br_renderer_state_stored **pss, br_uint_32 mask, br_token_value *tv)
{
	br_renderer_state_stored *ss;

	ss = RendererStateStoredD3DAllocate(self, &self->state, mask, tv);

	if(ss) {
		*pss = ss;
		return BRE_OK;
	} else
		return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, stateStoredAvail)
	(struct br_renderer *self, br_int_32 *psize, br_uint_32 mask, br_token_value *tv)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, bufferStoredNew)
	(struct br_renderer *self, struct br_buffer_stored **psm,
	br_token use, struct br_device_pixelmap *pm, br_token_value *tv)
{
	struct br_buffer_stored *sm;

	sm = BufferStoredD3DAllocate(self, use, pm, tv);

	if (sm) {
		*psm = sm;
		return BRE_OK;
	} else
		return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, bufferStoredAvail)(
	struct br_renderer *self,
		br_int_32 *space,
		br_token use,
		br_token_value *tv)
{
	/*
	 * Should return free VRAM
	 */
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, flush)
	(struct br_renderer *self, br_boolean wait)
{
	br_error r;

	/*
	 * End the scene if necessary.
	 *
	 * Never any need to wait with Direct3D (I believe)
	 */
	if (self->scene_started) {

		r = RendererSceneEnd(self);

		if (r != BRE_OK)
			return r;
	}

	return BRE_OK;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d, synchronise)
	(struct br_renderer *self, br_token sync_type, br_boolean block)
{
	switch (sync_type) {

	case BRT_RENDERER:

		/*
		 * Wait until any drawing has finished
		 *
		 * *** Don't know how to do this, if it is possible
		 */
		// ....
		break;

	default:

		return BRE_UNSUPPORTED;
	}
	
	return BRE_OK;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModeSet)
	(struct br_renderer *self, br_token mode)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModeQuery)
	(struct br_renderer *self, br_token *mode)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModeDefault)
	(struct br_renderer *self)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModePush)
	(struct br_renderer *self)
{
	return BRE_FAIL;
}

static br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModePop)
	(struct br_renderer *self)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d, sceneBegin)
	(struct br_renderer *self)
{
	LPDIRECTDRAWSURFACE surface;
	D3DVIEWPORT2 viewport = { sizeof(D3DVIEWPORT2) };

	/*
	 * Check we have a rendering target
	 */
	if (self->state.out.colour == NULL)
		return BRE_FAIL;

	/*
	 * We need the old interface for the destination surface
	 */
	if (IDirectDrawSurface2_QueryInterface(self->state.out.colour->surface, &IID_IDirectDrawSurface, &surface) != DD_OK)
		return BRE_FAIL;

	/*
	 * Set the target for the device
	 */
	if (IDirect3DDevice2_SetRenderTarget(self->d3d_device, surface, 0) != DD_OK) {
		IDirectDrawSurface_Release(surface);
		return BRE_FAIL;
	}

	/*
	 * Update the viewport for the new render target
	 *
	 * No 3D clipping is done but I still need to give sensible view volume values
	 */
	viewport.dwX = self->state.out.colour->pm_base_x;
	viewport.dwY = self->state.out.colour->pm_base_y;
	viewport.dwWidth = self->state.out.colour->pm_width;
	viewport.dwHeight = self->state.out.colour->pm_height;
		
	viewport.dvClipX = -1.0;
	viewport.dvClipY = -1.0;
	viewport.dvClipWidth = 2.0f;
	viewport.dvClipHeight = 2.0f;
	viewport.dvMinZ = 0.0f;
	viewport.dvMaxZ = 1.0f;
	
	if (IDirect3DViewport2_SetViewport2(self->viewport, &viewport) != DD_OK) {
		IDirectDrawSurface_Release(surface);
		return BRE_FAIL;
	}
	
	IDirectDrawSurface_Release(surface);

	if (self->state.out.depth != NULL) {

		/*
		 * Ensure that this depth buffer is attached
		 */
		switch (IDirectDrawSurface2_AddAttachedSurface(self->state.out.colour->surface, self->state.out.depth->surface)) {

		case DD_OK:
		case DDERR_SURFACEALREADYATTACHED:
			break;

		default:
			return BRE_FAIL;
		}
	}

	if (IDirect3DDevice2_BeginScene(self->d3d_device) != DD_OK)
		return BRE_FAIL;
	
	/*
	 * Set various flags to show that rendering is occurring
	 */
	self->state.out.colour->renderer_active = BR_TRUE;

	if (self->state.out.depth != NULL)
		self->state.out.depth->renderer_active = BR_TRUE;

	self->scene_started = BR_TRUE;

	return BRE_OK;	
}

br_error BR_CMETHOD_DECL(br_renderer_d3d, sceneEnd)
	(struct br_renderer *self)
{
	if (IDirect3DDevice2_EndScene(self->d3d_device) != DD_OK)
		return BRE_FAIL;
	
	self->scene_started = BR_FALSE;

	if (self->state.out.colour != NULL)
		self->state.out.colour->renderer_active = BR_FALSE;

	if (self->state.out.depth != NULL)
		self->state.out.depth->renderer_active = BR_FALSE;

	return BRE_OK;
}


/*
 * Default dispatch table for renderer
 */
static const struct br_renderer_dispatch rendererDispatch = {
	BR_CMETHOD_REF(br_renderer_d3d, sceneBegin),
	BR_CMETHOD_REF(br_renderer_d3d, sceneEnd),
	NULL,
	NULL,
	BR_CMETHOD_REF(br_renderer_d3d, free),
	BR_CMETHOD_REF(br_renderer_d3d, identifier),
	BR_CMETHOD_REF(br_renderer_d3d, type),
	BR_CMETHOD_REF(br_renderer_d3d, isType),
	BR_CMETHOD_REF(br_renderer_d3d, device),
	BR_CMETHOD_REF(br_renderer_d3d, space),
	BR_CMETHOD_REF(br_renderer_d3d, templateQuery),
	BR_CMETHOD_REF(br_object, query),
	BR_CMETHOD_REF(br_object, queryBuffer),
	BR_CMETHOD_REF(br_object, queryMany),
	BR_CMETHOD_REF(br_object, queryManySize),
	BR_CMETHOD_REF(br_object, queryAll),
	BR_CMETHOD_REF(br_object, queryAllSize),
	BR_CMETHOD_REF(br_renderer_d3d,                listQuery),
	BR_CMETHOD_REF(br_object_container,             tokensMatchBegin),
	BR_CMETHOD_REF(br_object_container,             tokensMatch),
	BR_CMETHOD_REF(br_object_container,             tokensMatchEnd),
	BR_CMETHOD_REF(br_object_container,             addFront),
	BR_CMETHOD_REF(br_object_container,             removeFront),
	BR_CMETHOD_REF(br_object_container,             remove),
	BR_CMETHOD_REF(br_object_container,             find),
	BR_CMETHOD_REF(br_object_container,             findMany),
	BR_CMETHOD_REF(br_object_container,             count),

	BR_CMETHOD_REF(br_renderer_d3d,                validDestination),
	BR_CMETHOD_REF(br_renderer_d3d,                stateStoredNew),
	BR_CMETHOD_REF(br_renderer_d3d,                stateStoredAvail),
	BR_CMETHOD_REF(br_renderer_d3d,                bufferStoredNew),
	BR_CMETHOD_REF(br_renderer_d3d,                bufferStoredAvail),
	BR_CMETHOD_REF(br_renderer_d3d,                partSet),
	BR_CMETHOD_REF(br_renderer_d3d,                partSetMany),
	BR_CMETHOD_REF(br_renderer_d3d,                partQuery),
	BR_CMETHOD_REF(br_renderer_d3d,                partQueryBuffer),
	BR_CMETHOD_REF(br_renderer_d3d,                partQueryMany),
	BR_CMETHOD_REF(br_renderer_d3d,                partQueryManySize),
	BR_CMETHOD_REF(br_renderer_d3d,                partQueryAll),
	BR_CMETHOD_REF(br_renderer_d3d,                partQueryAllSize),
	BR_CMETHOD_REF(br_renderer_d3d,				partIndexQuery),
	BR_CMETHOD_REF(br_renderer_d3d,                modelMulF),
	BR_CMETHOD_REF(br_renderer_d3d,                modelMulX),
	BR_CMETHOD_REF(br_renderer_d3d,                modelPopPushMulF),
	BR_CMETHOD_REF(br_renderer_d3d,                modelPopPushMulX),
	BR_CMETHOD_REF(br_renderer_d3d,                modelInvert),
	BR_CMETHOD_REF(br_renderer_d3d,                statePush),
	BR_CMETHOD_REF(br_renderer_d3d,                statePop),
	BR_CMETHOD_REF(br_renderer_d3d,                stateSave),
	BR_CMETHOD_REF(br_renderer_d3d,                stateRestore),
	BR_CMETHOD_REF(br_renderer_d3d,                stateMask),
	BR_CMETHOD_REF(br_renderer_d3d,                stateDefault),
	BR_CMETHOD_REF(br_renderer_d3d,                boundsTestF),
	BR_CMETHOD_REF(br_renderer_d3d,                boundsTestX),
	BR_CMETHOD_REF(br_renderer_d3d,                coverageTestF),
	BR_CMETHOD_REF(br_renderer_d3d,                coverageTestX),
	BR_CMETHOD_REF(br_renderer_d3d,                viewDistanceF),
	BR_CMETHOD_REF(br_renderer_d3d,                viewDistanceX),
	BR_CMETHOD_REF(br_renderer_d3d,                commandModeSet),
	BR_CMETHOD_REF(br_renderer_d3d,                commandModeQuery),
	BR_CMETHOD_REF(br_renderer_d3d,                commandModeDefault),
	BR_CMETHOD_REF(br_renderer_d3d,                commandModePush),
	BR_CMETHOD_REF(br_renderer_d3d,                commandModePop),
	BR_CMETHOD_REF(br_renderer_d3d,                flush),
	BR_CMETHOD_REF(br_renderer_d3d,                synchronise),
    BR_CMETHOD_REF(br_renderer_d3d,                partQueryCapability),
    BR_CMETHOD_REF(br_renderer_d3d,                stateQueryPerformance),
};
