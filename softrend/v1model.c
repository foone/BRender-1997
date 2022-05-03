/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: v1model.c 2.16 1997/05/22 14:45:12 jon Exp JOHNG $
 * $Locker: JOHNG $
 *
 * Support routines for rendering models
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"
#include "blockops.h"

#include "vecifns.h"
#include "timing.h"


#include "gamerend.h"


BR_RCS_ID("$Id: v1model.c 2.16 1997/05/22 14:45:12 jon Exp JOHNG $");

#define ASSUME_PERSPECTIVE_CAMERA 1


static void GEOMETRY_CALL V1Faces_ScratchAllocate(struct br_geometry *self, struct br_renderer *renderer){
	char *sp;
	br_size_t scratch_size;

	/*
	 * Scratch space
	 */
	scratch_size  = SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_counts));
	scratch_size += SCRATCH_ALIGN(rend.nfaces    * sizeof(*rend.temp_faces));
	scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.temp_vertices));

	if(rend.block->type == BRT_LINE)
		scratch_size += SCRATCH_ALIGN(rend.nedges * sizeof(*rend.edge_flags));

	if(rend.block->type == BRT_POINT)
		scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_flags));

	if(renderer->state.hidden.type == BRT_BUCKET_SORT ||
		rend.block->flags & BR_PRIMF_BLENDED && renderer->state.hidden.type == BRT_BUCKET_SORT_DEFERRED)

		scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_heap_pointers));

	rend.scratch = BrScratchAllocate(scratch_size+SCRATCH_BOUNDARY);

	/*
	 * Allocate common scratch areas
	 */
	sp = rend.scratch;

	rend.vertex_counts = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_counts));

	rend.temp_faces = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nfaces * sizeof(*rend.temp_faces));

	rend.temp_vertices = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.temp_vertices));

	/*
	 * Allocate optional scratch areas (edge and vertex flags)
	 */
	if(rend.block->type == BRT_LINE){
		rend.edge_flags = (void *)sp;
		sp += SCRATCH_ALIGN(rend.nedges * sizeof(*rend.edge_flags));
		memset(rend.edge_flags, 0, rend.nedges * sizeof(*rend.edge_flags));

		/*
		 * edge 0 is never rendererd (used for internal edges)
		 */
		rend.edge_flags[0] = 1;
	}

	if(rend.block->type == BRT_POINT){
		rend.vertex_flags = (void *)sp;
		sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_flags));
		memset(rend.vertex_flags,0,rend.nvertices * sizeof(*rend.vertex_flags));
	}

	if(renderer->state.hidden.type == BRT_BUCKET_SORT ||
		rend.block->flags & BR_PRIMF_BLENDED && renderer->state.hidden.type == BRT_BUCKET_SORT_DEFERRED) {

		rend.vertex_heap_pointers = (void *)sp;
		sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_heap_pointers));
		memset(rend.vertex_heap_pointers,0,rend.nvertices * sizeof(*rend.vertex_heap_pointers));
	}

	/*
	 * Clear vertex counts
	 */
	memset(rend.vertex_counts,0,(rend.nvertices * sizeof(*rend.vertex_counts)));
}

/*
 * Pre Culling operations
 */
#ifndef V1Face_CullNone
static void GEOMETRY_CALL V1Face_CullNone(struct br_geometry *self, struct br_renderer *renderer)
{
	int f;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	/*
	 * Assume all faces are visible
	 */
	for(f=0; f < rend.nfaces; f++, fp++, tfp++) {
		tfp->flag = TFF_VISIBLE;

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;
	}

//	rend.nvisible_faces = rend.nfaces;
}
#endif

#ifndef V1Face_OS_CullNone
static void GEOMETRY_CALL V1Face_OS_CullNone(struct br_geometry *self, struct br_renderer *renderer)
{
	int f;
	struct temp_face *tfp = rend.temp_faces;

	/*
	 * Assume all faces are visible
	 */
	for(f=0; f < rend.nfaces; f++, tfp++)
		tfp->flag = TFF_VISIBLE;

	/*
	 * Assume all vertices are visible (and that vertex counts are bytes)
	 */
	memset(rend.vertex_counts,1,rend.nvertices);

//	rend.nvisible_faces = rend.nfaces;
}
#endif

#if 1 // switch between asm and c implimentatations.
void GEOMETRY_CALL V1Face_CullOneSidedPerspective(struct br_geometry *self, struct br_renderer *renderer);
#else
#ifndef V1Face_CullOneSidedPerspective
static void GEOMETRY_CALL V1Face_CullOneSidedPerspective(struct br_geometry *self, struct br_renderer *renderer)
{
	int f,n;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;
	union {
		float f;
		int i;
	}result;

//	rend.nvisible_faces = 0;

 	for(f=0,n=0; f < rend.nfaces; f++, fp++, tfp++) {

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		result.f=BrVector3Dot((br_vector3 *)&fp->eqn,&scache.eye_m)-fp->eqn.v[3];
		if(result.i&0x80000000){
			tfp->flag = 0;
			continue;
		}

		tfp->flag = TFF_VISIBLE;

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

//		rend.nvisible_faces++;
	}
}
#endif
#endif

#ifndef V1Face_OS_CullOneSidedPerspective
#define V1Face_OS_CullOneSidedPerspective V1Face_CullOneSidedPerspective
#endif

#ifndef V1Face_CullOneSidedParallel
static void GEOMETRY_CALL V1Face_CullOneSidedParallel(struct br_geometry *self, struct br_renderer *renderer)
{
	int f,n;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

//	rend.nvisible_faces = 0;

 	for(f=0,n=0; f < rend.nfaces; f++, fp++, tfp++) {

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if(BrVector3Dot((br_vector3 *)&fp->eqn,&scache.eye_m) < S0) {
			tfp->flag = 0;
			continue;
		}

		tfp->flag = TFF_VISIBLE;

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

//		rend.nvisible_faces++;
	}
}
#endif

#ifndef V1Face_OS_CullOneSidedParallel
#define V1Face_OS_CullOneSidedParallel V1Face_CullOneSidedParallel
#endif

#ifndef V1Face_CullTwoSidedPerspective
static void GEOMETRY_CALL V1Face_CullTwoSidedPerspective(struct br_geometry *self, struct br_renderer *renderer)
{
	int f,df;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	for(f=0; f < rend.nfaces; f++, fp++, tfp++) {

		tfp->flag = TFF_VISIBLE;
		df = TVDIR_FRONT;

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if(BrVector3Dot((br_vector3 *)&fp->eqn,&scache.eye_m) < fp->eqn.v[3]) {
			tfp->flag |= TFF_REVERSED;
			df = TVDIR_BACK;
		}

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

		rend.temp_vertices[fp->vertices[0]].flags |= df;
		rend.temp_vertices[fp->vertices[1]].flags |= df;
		rend.temp_vertices[fp->vertices[2]].flags |= df;
	}

//	rend.nvisible_faces = rend.nfaces;
}
#endif

#ifndef V1Face_OS_CullTwoSidedPerspective
#define V1Face_OS_CullTwoSidedPerspective V1Face_CullTwoSidedPerspective
#endif

#ifndef V1Face_CullTwoSidedParallel
static void GEOMETRY_CALL V1Face_CullTwoSidedParallel(struct br_geometry *self, struct br_renderer *renderer)
{
	int f,df;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	for(f=0; f < rend.nfaces; f++, fp++, tfp++) {

		tfp->flag = TFF_VISIBLE;
		df = TVDIR_FRONT;

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if(BrVector3Dot((br_vector3 *)&fp->eqn,&scache.eye_m) < S0) {
			tfp->flag |= TFF_REVERSED;
			df = TVDIR_BACK;
		}

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

		rend.temp_vertices[fp->vertices[0]].flags |= df;
		rend.temp_vertices[fp->vertices[1]].flags |= df;
		rend.temp_vertices[fp->vertices[2]].flags |= df;
	}

//	rend.nvisible_faces = rend.nfaces;
}
#endif

#ifndef V1Face_OS_CullTwoSidedParallel
#define V1Face_OS_CullTwoSidedParallel V1Face_CullTwoSidedParallel
#endif

#ifndef V1Face_CullOneSided
static void GEOMETRY_CALL V1Face_CullOneSided(struct br_geometry *self, struct br_renderer *renderer)
{
	switch(renderer->state.matrix.view_to_screen_hint) {
	case BRT_PERSPECTIVE:
		V1Face_CullOneSidedPerspective(self, renderer);
		break;

	case BRT_PARALLEL:
		V1Face_CullOneSidedParallel(self, renderer);
		break;

	default:
		V1Face_CullNone(self, renderer);
		break;
	}
}
#endif

#ifndef V1Face_OS_CullOneSided
static void GEOMETRY_CALL V1Face_OS_CullOneSided(struct br_geometry *self, struct br_renderer *renderer)
{
	switch(renderer->state.matrix.view_to_screen_hint) {
	case BRT_PERSPECTIVE:
		V1Face_OS_CullOneSidedPerspective(self, renderer);
		break;

	case BRT_PARALLEL:
		V1Face_OS_CullOneSidedParallel(self, renderer);
		break;

	default:
		V1Face_OS_CullNone(self, renderer);
		break;
	}
}
#endif

#ifndef V1Face_CullTwoSided
static void GEOMETRY_CALL V1Face_CullTwoSided(struct br_geometry *self, struct br_renderer *renderer)
{
	switch(renderer->state.matrix.view_to_screen_hint) {
	case BRT_PERSPECTIVE:
		V1Face_CullTwoSidedPerspective(self, renderer);
		break;

	case BRT_PARALLEL:
		V1Face_CullTwoSidedParallel(self, renderer);
		break;

	default:
		V1Face_CullNone(self, renderer);
		break;
	}
}
#endif

#ifndef V1Face_OS_CullTwoSided
static void GEOMETRY_CALL V1Face_OS_CullTwoSided(struct br_geometry *self, struct br_renderer *renderer)
{
	switch(renderer->state.matrix.view_to_screen_hint) {
	case BRT_PERSPECTIVE:
		V1Face_OS_CullTwoSidedPerspective(self, renderer);
		break;

	case BRT_PARALLEL:
		V1Face_OS_CullTwoSidedParallel(self, renderer);
		break;

	default:
		V1Face_OS_CullNone(self, renderer);
		break;
	}
}
#endif

/*
 * Do per face work - find all the faces that are on screen
 */
#ifndef V1Face_Outcode
static void GEOMETRY_CALL V1Face_Outcode(struct br_geometry *self, struct br_renderer *renderer)
{
	int f;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;
	br_uint_32 combined_codes;

	rend.faces_clipped = BR_FALSE;

	for(f=0; f < rend.nfaces; f++,fp++,tfp++) {

		if(!tfp->flag)
			continue;

		/*
		 * Work out AND/OR of outcodes
		 */
		combined_codes = rend.temp_vertices[fp->vertices[0]].flags |
						 rend.temp_vertices[fp->vertices[1]].flags |
						 rend.temp_vertices[fp->vertices[2]].flags;
		/*
		 * If completely of one edge of view volume (by outcodes)
		 *	mark as not visible
		 *	continue
		 */
		if((combined_codes & OUTCODES_NOT) != OUTCODES_NOT) {
			rend.nvisible_faces--;

			tfp->flag = 0;

			rend.vertex_counts[fp->vertices[0]]--;
			rend.vertex_counts[fp->vertices[1]]--;
			rend.vertex_counts[fp->vertices[2]]--;
			continue;
		}

		/*
		 * If any outcode is set - mark as needing clipping and remember combined codes
		 */
  		if(combined_codes & OUTCODES_ALL) {
			tfp->flag |= TFF_CLIPPED;
			tfp->codes = (br_uint_16)(combined_codes & OUTCODES_ALL);
			rend.faces_clipped = BR_TRUE;
		}
	}
}
#endif

#if DEBUG && ENABLE_FACE_GROUP_COUNT
extern int trianglesDrawnCount;
#endif

#ifndef V1Face_Render
static void GEOMETRY_CALL V1Face_Render(struct br_geometry *self, struct br_renderer *renderer)
{
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;
	int f;
	brp_block * unclipped = renderer->state.cache.face_blocks_onscreen[renderer->state.cache.nface_blocks_onscreen].chain;
	brp_block * clipped  = renderer->state.cache.face_blocks[renderer->state.cache.nface_blocks].chain;

	/*
	 * Go through each face in loop
	 */
	for(f=0; f < rend.nfaces; f++,fp++, tfp++) {

		if(!(tfp->flag & TFF_VISIBLE))
			continue;

		rend.current_face = f;

		if(tfp->flag & TFF_CLIPPED) {
#if DEBUG && ENABLE_FACE_GROUP_COUNT
			trianglesDrawnCount++;
#endif

			clipped->render(clipped,
				rend.temp_vertices+fp->vertices[0],
				rend.temp_vertices+fp->vertices[1],
				rend.temp_vertices+fp->vertices[2], fp, tfp);
		} else {
			unclipped->render(unclipped,
				rend.temp_vertices+fp->vertices[0],
				rend.temp_vertices+fp->vertices[1],
				rend.temp_vertices+fp->vertices[2], fp, tfp);
		}
	}
}
#endif

#ifndef V1Face_OS_Render
void GEOMETRY_CALL V1Face_OS_Render(struct br_geometry *self, struct br_renderer *renderer)
{
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;
	int f;
	brp_block * unclipped = renderer->state.cache.face_blocks_onscreen[renderer->state.cache.nface_blocks_onscreen].chain;

	/*
	 * Go through each face in loop
	 */
	for(f=0; f < rend.nfaces; f++,fp++, tfp++) {
		if(tfp->flag & TFF_VISIBLE) {

			rend.current_face = f;

#if DEBUG && ENABLE_FACE_GROUP_COUNT
			trianglesDrawnCount++;
#endif

#if 1
			unclipped->render(unclipped,
				rend.temp_vertices+fp->vertices[0],
				rend.temp_vertices+fp->vertices[1],
				rend.temp_vertices+fp->vertices[2],
				fp, tfp);
#endif
		}
	}
}
#endif

#ifndef V1Face_OSV_Render
void GEOMETRY_CALL V1Face_OSV_Render(struct br_geometry *self, struct br_renderer *renderer)
{
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;
	int f;
	brp_block * unclipped = renderer->state.cache.face_blocks_onscreen[renderer->state.cache.nface_blocks_onscreen].chain;

	/*
	 * Go through each face in loop
	 */
	for(f=0; f < rend.nfaces; f++,fp++, tfp++) {

		rend.current_face = f;

		unclipped->render(unclipped,
			rend.temp_vertices+fp->vertices[0],
			rend.temp_vertices+fp->vertices[1],
			rend.temp_vertices+fp->vertices[2],
			fp, tfp);
	}
}
#endif

#if BASED_FIXED
#define CONVERT_MASK_OTHER convert_mask_f
#endif

#if BASED_FLOAT
#define CONVERT_MASK_OTHER convert_mask_x
#endif

/*
 * Select initial culling operations
 */
static void GEOMETRY_CALL V1Faces_GeometryFnsCull(struct br_renderer *renderer)
{
	GeometryFunctionAdd(renderer, Vertex_ClearFlags);
	if(renderer->state.cull.type == BRT_TWO_SIDED)
		GeometryFunctionOnScreenAdd(renderer, Vertex_ClearFlags);

	switch(renderer->state.cull.type) {
	case BRT_ONE_SIDED:
#if ASSUME_PERSPECTIVE_CAMERA
		GeometryFunctionAdd(renderer, V1Face_CullOneSidedPerspective);
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_CullOneSidedPerspective);
#else
		GeometryFunctionAdd(renderer, V1Face_CullOneSided);
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_CullOneSided);
#endif
		break;

	case BRT_TWO_SIDED:
#if ASSUME_PERSPECTIVE_CAMERA
		GeometryFunctionAdd(renderer, V1Face_CullTwoSidedPerspective);
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_CullTwoSidedPerspective);
#else
		GeometryFunctionAdd(renderer, V1Face_CullTwoSided);
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_CullTwoSided);
#endif
		break;

	default:
		GeometryFunctionAdd(renderer, V1Face_CullNone);
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_CullNone);
		break;
	}
}

/*
 * Generate a function to replicate constant values to all vertices
 */

static void GEOMETRY_CALL AddReplicateConstant(struct br_geometry *self, struct br_renderer *renderer)
{
   	if(rend.block->constant_mask == (1 << C_I)) {
		PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleReplicateConstantI);
	} else if(rend.block->constant_mask == (1 << C_R)|(1 << C_G)|(1 << C_B)) {
		PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleReplicateConstantRGB);
	} else {
		PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleReplicateConstant);
	}
}

/*
 * Generate the cached lists of geometry and per-face functions
 */
static void GEOMETRY_CALL V1Faces_GeometryFnsUpdate(struct br_geometry *self, struct br_renderer *renderer, br_boolean divert)
{
	/*
	 * Reset function lists
	 */
	GeometryFunctionReset(renderer);
	PrimBlockReset(renderer);

	/*
	 * Initially sharing of vertices in the primitive heap is allowed if no constant components are
	 * stored there
	 */
	renderer->state.cache.share_vertex_0 = rend.block->constant_components == 0;
	renderer->state.cache.share_other_vertices = !(rend.block->flags & BR_PRIMF_CONST_DUPLICATE);
	
	/**
	 ** Geometry operations
	 **/

	/*
	 * Setup
	 */
	GeometryFunctionBothAdd(renderer, V1Faces_ScratchAllocate);

	/*
	 * Model space Culling
	 */
	V1Faces_GeometryFnsCull(renderer);

	/*
	 * Vertex transform project, outcodes, bounds, & lighting
	 */
	VertexGeometryFns(renderer, V1Face_Outcode);

#if FAST_LIGHTING
	/*
	 * coloured lighting after vertex counts have been minimized
	 */

	if(rend.block->vertex_components & (CM_R|CM_G|CM_B)) 
		if(renderer->state.surface.lighting)
			GeometryFunctionBothAdd(renderer, GeometryColourLit);

	if(rend.block->vertex_components & CM_I) 
		if(renderer->state.surface.lighting) 
			GeometryFunctionBothAdd(renderer, GeometryIndexLit);

#endif

	/*
	 * Rendering
	 */
	GeometryFunctionAdd(renderer, V1Face_Render);

	if(renderer->state.cull.type == BRT_ONE_SIDED)
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_Render);
	else
		GeometryFunctionOnScreenAdd(renderer, V1Face_OSV_Render);

	/*
	 * Clean up
	 */
#if DEBUG
	GeometryFunctionBothAdd(renderer, ScratchFree);
#endif
	/**
	 ** Generate primitive output chain
	 **/

	/*
	 * Divert primitives to order table if required
	 */
	if(divert) {
		if(rend.block->convert_mask_i || rend.block->CONVERT_MASK_OTHER) {

			switch(rend.block->type) {
			case BRT_POINT:
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddPointConvert);
				break;
			case BRT_LINE:
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddLineConvert);
				break;
			case BRT_TRIANGLE:
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddTriangleConvert);
				break;
			}
		} else {
			switch(rend.block->type) {
			case BRT_POINT:
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddPoint);
				break;
			case BRT_LINE:
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddLine);
				break;
			case BRT_TRIANGLE:
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddTriangle);
				break;
			}
		}
	} else {

		/*
		 * Convertion of parameters
		 */
		if(rend.block->convert_mask_i || rend.block->CONVERT_MASK_OTHER) {
			switch(rend.block->type) {
			case BRT_POINT:
				PrimBlockAddBoth(renderer, (brp_render_fn *)RenderConvert1);
				break;
			case BRT_LINE:
				PrimBlockAddBoth(renderer, (brp_render_fn *)RenderConvert2);
				break;
			case BRT_TRIANGLE:
				PrimBlockAddBoth(renderer, (brp_render_fn *)RenderConvert3);
				break;
			}
		}
	}

	/*
	 * Clipping and constant parameter evalation, and optional fragmentation of triangles into lines or points
	 */
	switch(rend.block->type) {
	case BRT_POINT:
		PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleToPoints);
		PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleToPoints_OS);

		if(renderer->state.cache.nconstant_fns) {
			AddReplicateConstant(self, renderer);
			if(renderer->state.cull.type == BRT_TWO_SIDED)
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleTwoSidedConstantSurf);
			else
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleConstantSurf);
		} 
		break;

	case BRT_LINE:

		PrimBlockAdd(renderer, (brp_render_fn *)OpLineClip);
		PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleToLines);

		if(renderer->state.cache.nconstant_fns) {
			AddReplicateConstant(self, renderer);

			if(renderer->state.cull.type == BRT_TWO_SIDED)
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleTwoSidedConstantSurf);
			else
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleConstantSurf);
		}
		break;

	case BRT_TRIANGLE:

		if(renderer->state.cache.nconstant_fns) {
			if(rend.block->flags & BR_PRIMF_CONST_DUPLICATE)
				AddReplicateConstant(self, renderer);

			PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleClipConstantSurf);

			if(renderer->state.cull.type == BRT_TWO_SIDED)
				PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleTwoSidedConstantSurf);
			else
				PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleConstantSurf);
		} else
		   PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleClip);

        /*
         * Optional subdivision if the primitive requests it
         */
#if 0 // removed subdivision for <REDACTEDGAME>
        if(rend.block->flags & BR_PRIMF_SUBDIVIDE) {
    		PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleSubdivide);
	    	PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleSubdivideOnScreen);
			SubdivideSetThreshold(rend.block->subdivide_tolerance);
        }
#endif
		break;
	}

#if 0//FAST_LIGHTING
	if(rend.block->constant_components & (CM_R|CM_G|CM_B)) 
		if(renderer->state.surface.lighting)
			PrimBlockAddBoth(renderer, (brp_render_fn *)FaceColourLit);

	if(rend.block->constant_components & CM_I) 
		if(renderer->state.surface.lighting) 
			PrimBlockAddBoth(renderer, (brp_render_fn *)FaceIndexLit);

#endif

	/*
	 * Two-sided relighting of vertices
	 */
#if 0	// Removed two sided relighting for <REDACTEDGAME>
		if((renderer->state.cull.type == BRT_TWO_SIDED) && renderer->state.cache.nvertex_fns)
	   PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleRelightTwoSided);
#endif

	/*
	 * Environment wrapping fixup
	 */
	if((renderer->state.surface.mapping_source == BRT_ENVIRONMENT_INFINITE)||
	   (renderer->state.surface.mapping_source == BRT_ENVIRONMENT_LOCAL))
	   PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleMappingWrapFix);

	/*
	 * Quad based texture mapping
	 */
	if((renderer->state.surface.mapping_source == BRT_QUAD)) {

		PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleMapQuad);

		/*
		 * Sharing of vertices is not possible once they have been quad mapped
		 */
		renderer->state.cache.share_vertex_0 = BR_FALSE;
		renderer->state.cache.share_other_vertices = BR_FALSE;
	}
	
	/*
	 * Update cache comparison info.
	 */
	renderer->state.cache.format = self;
}


#if DEBUG && ENABLE_FACE_GROUP_COUNT
extern int faceGroupCount;
extern int modelsDrawnCount;
extern int trianglesRenderedCount;
extern int verticesRenderedCount;
#endif

#if FAST_LIGHTING
int useLight[BR_MAX_LIGHTS];
#endif

static br_error V1Model_Render
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
		struct br_renderer_state_stored *default_state,
		br_token type, br_boolean on_screen)
{
	int i,g;
	struct br_renderer_state_stored *state;
	br_error r;

	br_boolean z_sort, z_sort_deferred;
#if FAST_LIGHTING
	struct active_light *alp;
#endif

	/*																	           
	 */

#if DEBUG && ENABLE_FACE_GROUP_COUNT
	modelsDrawnCount++;
#endif
	
	CheckPrimitiveState(renderer);

	/*
	 * Remember current renderer and format in use
	 */
	rend.geometry = self;
	rend.renderer = renderer;

	/*
	 * Make local copy of bounds
	 */
	scache.min = renderer->state.bounds.min;
	scache.max = renderer->state.bounds.max;

	/*
	 * Do per-scene and per-model updates if necessary
	 */
	if(!scache.valid_per_scene) {
		StaticCacheUpdate_PerScene(renderer);
		scache.valid_per_scene = BR_TRUE;
	}

	if(!scache.valid_per_model) {
		StaticCacheUpdate_PerModel(renderer);
		scache.valid_per_model = BR_TRUE;
	}


#if FAST_LIGHTING
#ifndef SQ
#define SQ(a) ((a)*(a))
#endif
	if(model->flags&V11MODF_LIT){
		alp = scache.lights;
		for(g=0; g < scache.nlights_model; g++, alp++){
			useLight[g]=BR_TRUE;
			if(alp->type==BRT_POINT)
				if((SQ(alp->position.v[0])+SQ(alp->position.v[1])+SQ(alp->position.v[2]))>(SQ(alp->s->attenuation_q+model->radius)))
					useLight[g]=BR_FALSE;
		}
	}

#endif

	/*
	 * Go through each group
	 */
	for(g=0; g < model->ngroups; g++) {
		rend.faces = model->groups[g].faces;
		rend.vertices = model->groups[g].vertices;
		rend.face_colours = model->groups[g].face_colours;
		rend.vertex_colours = model->groups[g].vertex_colours;
		rend.face_flags = model->groups[g].face_flags;
		rend.nfaces = model->groups[g].nfaces;
		rend.nvertices = model->groups[g].nvertices;
		rend.nedges = model->groups[g].nedges;

#if DEBUG && ENABLE_FACE_GROUP_COUNT
		faceGroupCount++;
		trianglesRenderedCount+=rend.nfaces;
		verticesRenderedCount+=rend.nvertices;
#endif
		
		/*
	 	 * Restore state for group
		 */
		state = model->groups[g].stored?model->groups[g].stored:default_state;

		if(state)
			RendererStateRestore(renderer, state, (br_uint_32)BR_STATE_ALL);

		/*
		 * Synchronise with primitives library
		 */

		z_sort = (renderer->state.hidden.type == BRT_BUCKET_SORT &&
				 renderer->state.hidden.order_table != NULL &&
				 renderer->state.hidden.heap != NULL);

		z_sort_deferred = (renderer->state.hidden.type == BRT_BUCKET_SORT_DEFERRED &&
				 renderer->state.hidden.order_table != NULL &&
				 renderer->state.hidden.heap != NULL);

		r = PrimitiveStateRenderBegin(renderer->state.pstate,
			&rend.block, &rend.block_changed, &rend.range_changed, z_sort, type);

		if(r != BRE_OK)
			return r;

		/*
		 * Calculate working data
		 */

		/*
		 * Merge opacity into alpha byte of colour
		 */
		scache.colour = renderer->state.surface.colour & 0xFFFFFF;
		scache.colour |= BrScalarToInt(BR_CONST_MUL(renderer->state.surface.opacity,256)) << 24;

		/*
		 * Make sure base primitive block is hooked up to the right place
		 */
		renderer->state.cache.face_blocks[0].chain = rend.block;
		renderer->state.cache.face_blocks_onscreen[0].chain = rend.block;

		if (rend.block_changed || rend.range_changed || !renderer->state.cache.valid) {
			
			CacheUpdate(renderer);

			if (rend.block_changed || !renderer->state.cache.valid)
				V1Faces_GeometryFnsUpdate(self, renderer,
					z_sort || rend.block->flags & BR_PRIMF_BLENDED && z_sort_deferred);

			renderer->state.cache.valid = BR_TRUE;
			renderer->state.timestamp_cache = Timestamp();
		}

#if BASED_FIXED
		if(on_screen) {
			scache.scale_x = renderer->state.cache.comp_scales[C_SX];
			scache.scale_y = renderer->state.cache.comp_scales[C_SY];
			scache.scale_z = renderer->state.cache.comp_scales[C_SZ];

			scache.offset_x = renderer->state.cache.comp_offsets[C_SX];
			scache.offset_y = renderer->state.cache.comp_offsets[C_SY];
			scache.offset_z = renderer->state.cache.comp_offsets[C_SZ];

			ModelToViewportUpdate_A();
		}
#endif
		/*
		 * Invoke the current set of renderer functions on the group
		 */

		if(on_screen) {
			for(i=0; i < renderer->state.cache.ngeometry_fns_onscreen; i++)
				renderer->state.cache.geometry_fns_onscreen[i](self,renderer);
		} else {
			for(i=0; i < renderer->state.cache.ngeometry_fns; i++)
				renderer->state.cache.geometry_fns[i](self,renderer);
		}

		/*
		 * Save any cached state
		 */
		if (state)
			RendererStateSave(renderer, state, MASK_CACHED_STATES | BR_STATE_CACHE);

		/*
		 * Finish with primitives
		 */
		PrimitiveStateRenderEnd(renderer->state.pstate,rend.block);
	}

	/*
	 * Put updated bounds back
	 */
	renderer->state.bounds.min = scache.min;
	renderer->state.bounds.max = scache.max;

	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_geometry_v1_model_soft, render)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
		struct br_renderer_state_stored *default_state,
		br_token type)
{
	br_error r;

	r = V1Model_Render(self, renderer, model, default_state, type, BR_FALSE);

	return r;
}

br_error BR_CMETHOD_DECL(br_geometry_v1_model_soft, renderOnScreen)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
		struct br_renderer_state_stored *default_state,
		br_token type)
{
	br_error r;

	r = V1Model_Render(self, renderer, model, default_state, type, BR_TRUE);

	return r;
}

