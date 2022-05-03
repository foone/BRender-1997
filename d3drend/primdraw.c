/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: primdraw.c 1.2 1997/08/12 17:30:39 jon Exp JON $
 * $Locker: JON $
 *
 * Functions for dealing with primitive drawing
 */

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"


static struct brp_block block;
static struct br_renderer *current_renderer;
 
/*
 * Temporary code to set D3D state
 *
 * N.B. Lines that are commented out are either being set to their default value, or relate
 *      to state which is already set up by some other call
 *
 * Should also set state where default value is not same as that for default state
 */
br_error SetD3DGlobalState(struct br_renderer *renderer)
{

	/*
	 * Always enable sub-pixel accuracy
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SUBPIXEL, (DWORD)TRUE);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SUBPIXELX, (DWORD)TRUE); 

	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMAPBLEND, (DWORD)D3DTBLEND_DECAL);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SPECULARENABLE, (DWORD)FALSE);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SHADEMODE, (DWORD)D3DSHADE_FLAT);

	/*
	 * Always write if Z is <=
	 *
	 * Actually we use ALWAYS for FORCE_FRONT
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZFUNC, (DWORD)D3DCMP_LESSEQUAL); 

	/*
	 * Currently we always use RGB lighting values, but it might be better to detect mono-lit
	 * scenes and use mono lighting
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_MONOENABLE, (DWORD)FALSE);

	/*
	 * Don't want patterned lines
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_LINEPATTERN, (DWORD)0);

	/*
	 * Use normal ROP
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ROP2, (DWORD)R2_COPYPEN);

	/*
	 * Enable writing to all planes
	 *
	 * Actually we use this to disable colour write
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_PLANEMASK, (DWORD)0xffffffff);

	/*
	 * Disable last pixel on each line
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_LASTPIXEL, (DWORD)TRUE);

	/*
	 * Disable z-buffer checks on bounding rectangle of primitives
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZVISIBLE, (DWORD)FALSE);
	
	/*
	 * Always use wrapped texture addressing
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREADDRESSU, (DWORD)D3DTADDRESS_WRAP); 
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREADDRESSV, (DWORD)D3DTADDRESS_WRAP); 
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_WRAPU, (DWORD)TRUE);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_WRAPV, (DWORD)TRUE);

//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_BORDERCOLOR, (DWORD)0);
	
	/*
	 * Never use anisotrophic texture filtering
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ANISOTROPY, (DWORD)0);

	/*
	 * Never use stippling
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_STIPPLEENABLE, (DWORD)FALSE);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_STIPPLEDALPHA, (DWORD)FALSE);

	/*
	 * We do our own face culling
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_CULLMODE, (DWORD)D3DCULL_NONE);

	/*
	 * Always use colour keyed transparency
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_COLORKEYENABLE, (DWORD)TRUE);

	/*
	 * Never use edge anti-aliasing
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ANTIALIAS, (DWORD)D3DANTIALIAS_NONE);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_EDGEANTIALIAS, (DWORD)FALSE);

	/*
	 * Never use a z-bias
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZBIAS, (DWORD)0);

	/*
	 * Never use alpha testing
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ALPHATESTENABLE, (DWORD)FALSE);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ALPHAFUNC, (DWORD)D3DCMP_ALWAYS);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ALPHAREF, (DWORD)0);

	/*
	 * Never use fog tables
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGTABLEMODE, (DWORD)D3DFOG_LINEAR);
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGTABLEDENSITY, (DWORD)1.0); 

	/*
	 * Never use range fog
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_RANGEFOGENABLE, (DWORD)FALSE);

	/*
	 * Never bias the LOD
	 */
//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_MIPMAPLODBIAS, (DWORD)0);

	/*
	 * Always use normal alpha-blending
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SRCBLEND, (DWORD)D3DBLEND_SRCALPHA);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_DESTBLEND, (DWORD)D3DBLEND_INVSRCALPHA);

#if 1 // KLUDGE to always enable bilinear and clamping

	/*
	 * Set texture map filtering
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMAG, (DWORD)D3DFILTER_LINEAR);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMIN, (DWORD)D3DFILTER_LINEAR);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREADDRESS, (DWORD)D3DTADDRESS_CLAMP); 
#endif

#if 0
	/*
	 * Set fogging
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGENABLE, (DWORD)
		((renderer->state.prim.flags & PRIMF_FOG) != 0));

	//...
	
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGCOLOR, (DWORD)0);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGTABLESTART, (DWORD)0.0);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGTABLEEND, (DWORD)1.0);
#endif

	return BRE_OK;
}

br_error SetD3DOutputState(struct br_renderer *renderer)
{
	if (renderer->state.out.depth != NULL) {
		/*
		 * Enable z comparison and write-back
		 */
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZENABLE, (DWORD)TRUE);
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZWRITEENABLE, (DWORD)TRUE);

	} else {

		/*
		 * Disable z comparison and write-back
		 */
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZENABLE, (DWORD)FALSE);
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ZWRITEENABLE, (DWORD)FALSE);
	}

	return BRE_OK;	
}

br_error SetD3DPrimitiveState(struct br_renderer *renderer, struct state_prim *new)
{
	br_uint_32 changed_flags = renderer->state.prim.flags ^ new->flags;

	current_renderer = renderer;

	/*
	 * Set flat or smooth shading
	 */
	if (changed_flags & PRIMF_SMOOTH)
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SHADEMODE, (DWORD)
			(new->flags & PRIMF_SMOOTH)? D3DSHADE_GOURAUD: D3DSHADE_FLAT);

	/*
	 * Set dithering of colour buffer
	 */
	if (changed_flags & PRIMF_DITHER_COLOUR)
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_DITHERENABLE, (DWORD)
			((new->flags & PRIMF_DITHER_COLOUR) != 0));

	/*
	 * Set texture map
	 *
	 * *** Should keep texture handles and use timestamps to only query them when the render
	 * target changes ***
	 */
	if (renderer->state.prim.colour_map != new->colour_map)

		if (new->colour_map != NULL) {

			/*
			 * Copy the texture over if necessary
			 */
			if (new->colour_map->flags & SBUFF_ALLOCONLOAD) {

				if (!(new->colour_map->flags & SBUFF_LOADED) ||
					IDirectDrawSurface2_IsLost(new->colour_map->stored_surface) == DDERR_SURFACELOST) {

					if (IDirect3DTexture2_Load(new->colour_map->stored_texture, new->colour_map->source_texture) != DD_OK)
						return BRE_FAIL;

					new->colour_map->flags |= SBUFF_LOADED;
				}
			}

			/*
			 * *** SHOULD DEAL WITH PALETTE ***
			 */
					
			/*
			 * Get the texture handle
			 *
			 * *** SHOULD CHECK FOR CHANGED RENDERER ***
			 */
//			if (new->colour_map->handle == 0)
				if (IDirect3DTexture2_GetHandle(new->colour_map->stored_texture, renderer->d3d_device, &new->colour_map->handle) != DD_OK)
					return BRE_FAIL;
			
			IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREHANDLE, (DWORD)new->colour_map->handle);

		} else

			IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREHANDLE, (DWORD)NULL);

	/*
	 * Set modulation of textures
	 */
	if (changed_flags & PRIMF_MODULATE)
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMAPBLEND, (DWORD)
			((new->flags & PRIMF_MODULATE)? D3DTBLEND_MODULATE: D3DTBLEND_DECAL));

//	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_SPECULARENABLE, (DWORD)FALSE);

	/*
	 * Set perspective correction
	 */
	if (changed_flags & PRIMF_PERSPECTIVE)
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, (DWORD)
			((new->flags & PRIMF_PERSPECTIVE) != 0));

	/*
	 * Set alpha blending
	 */
	if (changed_flags & PRIMF_BLEND)
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_ALPHABLENDENABLE, (DWORD)
			((new->flags & PRIMF_BLEND) != 0));

#if 0 // KLUDGE to always enable bilinear and clamping

	/*
	 * Set texture map filtering
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMAG, (DWORD)D3DFILTER_LINEAR);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMIN, (DWORD)D3DFILTER_LINEAR);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREADDRESS, (DWORD)D3DTADDRESS_CLAMP); 
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREADDRESSU, (DWORD)D3DTADDRESS_CLAMP); 
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREADDRESSV, (DWORD)D3DTADDRESS_CLAMP); 
#endif

#if 0
	/*
	 * Set texture map filtering
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMAG, (DWORD)D3DFILTER_NEAREST);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_TEXTUREMIN, (DWORD)D3DFILTER_NEAREST);

	/*
	 * Set fogging
	 */
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGENABLE, (DWORD)
		((new->flags & PRIMF_FOG) != 0));

	//...
	
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGCOLOR, (DWORD)0);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGTABLESTART, (DWORD)0.0);
	IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FOGTABLEEND, (DWORD)1.0);
#endif
	
	return BRE_OK;
}


/*
 * Temporary code to draw primitives
 */
void BR_ASM_CALL DrawTriangle(struct brp_block *block, WORD *i, D3DTLVERTEX *v)
{
	IDirect3DDevice2_DrawPrimitive(current_renderer->d3d_device, D3DPT_TRIANGLELIST, D3DVT_TLVERTEX,
//		v, 3, D3DDP_DONOTCLIP);
		v, 3, D3DDP_DONOTCLIP | D3DDP_WAIT);
//		v, 3, D3DDP_DONOTCLIP | D3DDP_OUTOFORDER);

	return;
}


void BR_ASM_CALL DrawLine(struct brp_block *block, WORD *i, D3DTLVERTEX *v)
{
	IDirect3DDevice2_DrawPrimitive(current_renderer->d3d_device, D3DPT_LINELIST, D3DVT_TLVERTEX,
		v, 2, D3DDP_DONOTCLIP);
//		v, 2, D3DDP_DONOTCLIP | D3DDP_OUTOFORDER);

	return;
}

void BR_ASM_CALL DrawPoint(struct brp_block *block, WORD *i, D3DTLVERTEX *v)
{
	IDirect3DDevice2_DrawPrimitive(current_renderer->d3d_device, D3DPT_POINTLIST, D3DVT_TLVERTEX,
		v, 1, D3DDP_DONOTCLIP);
//		v, 1, D3DDP_DONOTCLIP | D3DDP_OUTOFORDER);

	return;
}

void BR_ASM_CALL DrawIndexedTriangle(struct brp_block *block, WORD *i, br_boolean modified)
{
	IDirect3DDevice2_DrawIndexedPrimitive(current_renderer->d3d_device, D3DPT_TRIANGLELIST, D3DVT_TLVERTEX,
//		rend.d3dtlvertices, rend.nvertices, i, 3, D3DDP_DONOTCLIP);
		rend.d3dtlvertices, rend.nvertices, i, 3, D3DDP_DONOTCLIP | D3DDP_WAIT);
//		rend.d3dtlvertices, rend.nvertices, i, 3, D3DDP_DONOTCLIP | D3DDP_OUTOFORDER);

	return;
}


void BR_ASM_CALL DrawIndexedLine(struct brp_block *block, WORD *i, br_boolean modified)
{
	IDirect3DDevice2_DrawIndexedPrimitive(current_renderer->d3d_device, D3DPT_LINELIST, D3DVT_TLVERTEX,
		rend.d3dtlvertices, rend.nvertices, i, 2, D3DDP_DONOTCLIP);
//		rend.d3dtlvertices, rend.nvertices, i, 2, D3DDP_DONOTCLIP | D3DDP_OUTOFORDER);

	return;
}

void BR_ASM_CALL DrawIndexedPoint(struct brp_block *block, WORD *i, br_boolean modified)
{
	IDirect3DDevice2_DrawIndexedPrimitive(current_renderer->d3d_device, D3DPT_POINTLIST, D3DVT_TLVERTEX,
		rend.d3dtlvertices, rend.nvertices, i, 1, D3DDP_DONOTCLIP);
//		rend.d3dtlvertices, rend.nvertices, i, 1, D3DDP_DONOTCLIP | D3DDP_OUTOFORDER);

	return;
}


/*
 * Temporary code to set up and return a brp_block
 */
void InitBrpBlock(struct br_renderer *renderer)
{
	block.chain = NULL;
	block.flags = 0;
	block.convert_mask_f = 0xffff;
	block.convert_mask_x = 0;
	block.convert_mask_i = 0;
	block.constant_mask = 0;
	block.subdivide_tolerance = 0;

	return;
}


struct brp_block *GetBrpBlock(struct br_renderer *renderer, br_token type)
{
	switch (type) {

	case BRT_POINT:
		block.identifier = "D3D Point";
		block.render = DrawPoint;
		block.render_indexed = DrawIndexedPoint;
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FILLMODE, D3DFILL_POINT);
		break;

	case BRT_LINE:
		block.identifier = "D3D Line";
		block.render = DrawLine;
		block.render_indexed = DrawIndexedLine;
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
		break;

	case BRT_TRIANGLE:
		block.identifier = "D3D Triangle";
		block.render = DrawTriangle;
		block.render_indexed = DrawIndexedTriangle;
		IDirect3DDevice2_SetRenderState(renderer->d3d_device, D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);
		break;
	}

	block.type = type;
	block.constant_components = 0;
	block.vertex_components = CM_SX | CM_SY;

	if (renderer->state.out.depth != NULL)
		block.vertex_components |= CM_SZ;
	
	if (renderer->state.prim.colour_map != NULL) {

		block.vertex_components |= CM_U | CM_V;

		if (renderer->state.prim.flags & PRIMF_PERSPECTIVE)
			block.vertex_components |= CM_W;

//		if (renderer->state.prim.flags & PRIMF_MODULATE)
			if (renderer->state.prim.flags & PRIMF_SMOOTH)
				block.vertex_components |= CM_R | CM_G | CM_B;
			else
				block.constant_components |= CM_R | CM_G | CM_B;

	} else
	
		if (renderer->state.prim.flags & PRIMF_SMOOTH)
			block.vertex_components |= CM_R | CM_G | CM_B;
		else
			block.constant_components |= CM_R | CM_G | CM_B;
	
	if (renderer->state.prim.flags & PRIMF_BLEND)
		block.vertex_components |= CM_A;

	return &block;
}
