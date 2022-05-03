/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: v1model.c 1.4 1997/08/12 17:30:57 jon Exp JON $
 * $Locker: JON $
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


BR_RCS_ID("$Id: v1model.c 1.4 1997/08/12 17:30:57 jon Exp JON $");

static void GEOMETRY_CALL V1Faces_ScratchAllocate(struct br_geometry *self, struct br_renderer *renderer)
{
	char *sp;
	br_size_t scratch_size;

	/*
	 * Scratch space
	 */
	scratch_size  = SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_counts));
	scratch_size += SCRATCH_ALIGN(rend.nfaces    * sizeof(*rend.temp_faces));

	scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.d3dhvertices));
	scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.d3dhw));
	scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.d3dtlvertices));

	if (rend.block->type == BRT_LINE)
		scratch_size += SCRATCH_ALIGN(rend.nedges * sizeof(*rend.edge_flags));

	if (rend.block->type == BRT_POINT)
		scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_flags));

#if ENABLE_ZSORT
	if (renderer->state.hidden.type == BRT_BUCKET_SORT)
		scratch_size += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_heap_pointers));
#endif

	rend.scratch = BrScratchAllocate(scratch_size + SCRATCH_BOUNDARY);

	/*
	 * Allocate common scratch areas
	 */
	sp = (char *)SCRATCH_ALIGN((br_uint_32)rend.scratch);

	rend.vertex_counts = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_counts));

	rend.temp_faces = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nfaces * sizeof(*rend.temp_faces));

	rend.d3dhvertices = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.d3dhvertices));

	rend.d3dhw = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.d3dhw));

	rend.d3dtlvertices = (void *)sp;
	sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.d3dtlvertices));

	/*
	 * Allocate optional scratch areas (edge and vertex flags)
	 */
	if (rend.block->type == BRT_LINE) {
		rend.edge_flags = (void *)sp;
		sp += SCRATCH_ALIGN(rend.nedges * sizeof(*rend.edge_flags));
		memset(rend.edge_flags, 0, rend.nedges * sizeof(*rend.edge_flags));

		/*
		 * edge 0 is never rendererd (used for internal edges)
		 */
		rend.edge_flags[0] = 1;
	}

	if (rend.block->type == BRT_POINT) {
		rend.vertex_flags = (void *)sp;
		sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_flags));
		memset(rend.vertex_flags, 0, rend.nvertices * sizeof(*rend.vertex_flags));
	}

#if ENABLE_ZSORT
	if (renderer->state.hidden.type == BRT_BUCKET_SORT) {
		rend.vertex_heap_pointers = (void *)sp;
		sp += SCRATCH_ALIGN(rend.nvertices * sizeof(*rend.vertex_heap_pointers));
		memset(rend.vertex_heap_pointers, 0, rend.nvertices * sizeof(*rend.vertex_heap_pointers));
	}
#endif

	/*
	 * Clear vertex counts
	 */
	memset(rend.vertex_counts, 0,(rend.nvertices * sizeof(*rend.vertex_counts)));
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
	for (f = 0; f < rend.nfaces; f++, fp++, tfp++) {
		tfp->flag = TFF_VISIBLE;

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;
	}

	rend.nvisible_faces = rend.nfaces;
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
	for (f = 0; f < rend.nfaces; f++, tfp++)
		tfp->flag = TFF_VISIBLE;

	/*
	 * Assume all vertices are visible (and that vertex counts are bytes)
	 */
	memset(rend.vertex_counts, 1, rend.nvertices);

	rend.nvisible_faces = rend.nfaces;
}
#endif

#ifndef V1Face_CullOneSidedPerspective
static void GEOMETRY_CALL V1Face_CullOneSidedPerspective(struct br_geometry *self, struct br_renderer *renderer)
{
	int f, n;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	rend.nvisible_faces = 0;

 	for (f = 0, n = 0; f < rend.nfaces; f++, fp++, tfp++) {

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if (BrVector3Dot((br_vector3 *)&fp->eqn, &scache.eye_m) < fp->eqn.v[3]) {
			tfp->flag = 0;
			continue;
		}

		tfp->flag = TFF_VISIBLE;

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

		rend.nvisible_faces++;
	}
}
#endif

#ifndef V1Face_OS_CullOneSidedPerspective
#define V1Face_OS_CullOneSidedPerspective V1Face_CullOneSidedPerspective
#endif

#ifndef V1Face_CullOneSidedParallel
static void GEOMETRY_CALL V1Face_CullOneSidedParallel(struct br_geometry *self, struct br_renderer *renderer)
{
	int f, n;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	rend.nvisible_faces = 0;

 	for (f = 0, n = 0; f < rend.nfaces; f++, fp++, tfp++) {

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if (BrVector3Dot((br_vector3 *)&fp->eqn, &scache.eye_m) < S0) {
			tfp->flag = 0;
			continue;
		}

		tfp->flag = TFF_VISIBLE;

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

		rend.nvisible_faces++;
	}
}
#endif

#ifndef V1Face_OS_CullOneSidedParallel
#define V1Face_OS_CullOneSidedParallel V1Face_CullOneSidedParallel
#endif

#ifndef V1Face_CullTwoSidedPerspective
static void GEOMETRY_CALL V1Face_CullTwoSidedPerspective(struct br_geometry *self, struct br_renderer *renderer)
{
	int f, df;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	for (f = 0; f < rend.nfaces; f++, fp++, tfp++) {

		tfp->flag = TFF_VISIBLE;
		df = TVDIR_FRONT;

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if (BrVector3Dot((br_vector3 *)&fp->eqn, &scache.eye_m) < fp->eqn.v[3]) {
			tfp->flag |= TFF_REVERSED;
			df = TVDIR_BACK;
		}

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

		rend.d3dhvertices[fp->vertices[0]].dwFlags |= df;
		rend.d3dhvertices[fp->vertices[1]].dwFlags |= df;
		rend.d3dhvertices[fp->vertices[2]].dwFlags |= df;
	}

	rend.nvisible_faces = rend.nfaces;
}
#endif

#ifndef V1Face_OS_CullTwoSidedPerspective
#define V1Face_OS_CullTwoSidedPerspective V1Face_CullTwoSidedPerspective
#endif

#ifndef V1Face_CullTwoSidedParallel
static void GEOMETRY_CALL V1Face_CullTwoSidedParallel(struct br_geometry *self, struct br_renderer *renderer)
{
	int f, df;
	struct v11face *fp = rend.faces;
	struct temp_face *tfp = rend.temp_faces;

	for (f = 0; f < rend.nfaces; f++, fp++, tfp++) {

		tfp->flag = TFF_VISIBLE;
		df = TVDIR_FRONT;

		/*
		 * if Plane_Eqn . Eye <= 0, face is away from eye
		 */
		if (BrVector3Dot((br_vector3 *)&fp->eqn, &scache.eye_m) < S0) {
			tfp->flag |= TFF_REVERSED;
			df = TVDIR_BACK;
		}

		rend.vertex_counts[fp->vertices[0]]++;
		rend.vertex_counts[fp->vertices[1]]++;
		rend.vertex_counts[fp->vertices[2]]++;

		rend.d3dhvertices[fp->vertices[0]].dwFlags |= df;
		rend.d3dhvertices[fp->vertices[1]].dwFlags |= df;
		rend.d3dhvertices[fp->vertices[2]].dwFlags |= df;
	}

	rend.nvisible_faces = rend.nfaces;
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

	for (f = 0; f < rend.nfaces; f++, fp++, tfp++) {

		if (!tfp->flag)
			continue;

		/*
		 * Work out AND/OR of outcodes
		 */
		combined_codes = rend.d3dhvertices[fp->vertices[0]].dwFlags |
						 rend.d3dhvertices[fp->vertices[1]].dwFlags |
						 rend.d3dhvertices[fp->vertices[2]].dwFlags;

		/*
		 * If completely of one edge of view volume (by outcodes)
		 *	mark as not visible
		 *	continue
		 */
		if ((combined_codes & OUTCODES_NOT) != OUTCODES_NOT) {
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
  		if (combined_codes & OUTCODES_ALL) {
			tfp->flag |= TFF_CLIPPED;
			tfp->codes = (br_uint_16)(combined_codes & OUTCODES_ALL);
			rend.faces_clipped = BR_TRUE;
		}
	}
}
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
	for (f = 0; f < rend.nfaces; f++, fp++, tfp++) {

		if (!(tfp->flag & TFF_VISIBLE))
			continue;

		if (tfp->flag & TFF_CLIPPED)
			clipped->render_indexed(clipped, &fp->vertices, BR_FALSE, fp, tfp);
		else
			unclipped->render_indexed(unclipped, &fp->vertices, BR_FALSE, fp, tfp);
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
	for (f = 0; f < rend.nfaces; f++, fp++, tfp++)
		if (tfp->flag & TFF_VISIBLE)
			unclipped->render_indexed(unclipped, &fp->vertices, BR_FALSE, fp, tfp);
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
	for (f = 0; f < rend.nfaces; f++, fp++, tfp++)
		unclipped->render_indexed(unclipped, &fp->vertices, BR_FALSE, fp, tfp);
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
	if (renderer->state.cull.type == BRT_TWO_SIDED)
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

	/*
	 * Rendering
	 */
	GeometryFunctionAdd(renderer, V1Face_Render);

	if (renderer->state.cull.type == BRT_ONE_SIDED)
		GeometryFunctionOnScreenAdd(renderer, V1Face_OS_Render);
	else
		GeometryFunctionOnScreenAdd(renderer, V1Face_OSV_Render);

	/*
	 * Clean up
	 */
#if REDACTEDGAME && DEBUG
	GeometryFunctionBothAdd(renderer, ScratchFree);
#endif

	/**
	 ** Generate primitive output chain
	 **/

	/*
	 * Divert primitives to order table if required
	 */
#if 0
	if (divert) {
		switch(rend.block->type) {
		case BRT_POINT:
			PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddPoint, (brp_render_fn *)OpHeapAddPointIndexed);
			break;
		case BRT_LINE:
			PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddLine, (brp_render_fn *)OpHeapAddLineIndexed);
			break;
		case BRT_TRIANGLE:
			PrimBlockAddBoth(renderer, (brp_render_fn *)OpHeapAddTriangle, (brp_render_fn *)OpHeapAddTriangleIndexed);
			break;
		}
	}
#endif

	/*
	 * Clipping and constant parameter evalation, and optional fragmentation of triangles into lines or points
	 */
	switch(rend.block->type) {
	case BRT_POINT:
		PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleToPoints, (brp_render_fn *)OpTriangleToPointsIndexed);
		PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleToPoints_OS, OpTriangleToPointsIndexed_OS);

		if (renderer->state.cache.nconstant_fns) {

			if (renderer->state.cull.type == BRT_TWO_SIDED)
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleTwoSidedConstantSurf, (brp_render_fn *)OpTriangleTwoSidedConstantSurfIndexed);
			else
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleConstantSurf, (brp_render_fn *)OpTriangleConstantSurfIndexed);
		} 
		break;

	case BRT_LINE:

		PrimBlockAdd(renderer, (brp_render_fn *)OpLineClip, (brp_render_fn *)OpLineClipIndexed);
		PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleToLines, (brp_render_fn *)OpTriangleToLinesIndexed);

		if (renderer->state.cache.nconstant_fns) {

			if (renderer->state.cull.type == BRT_TWO_SIDED)
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleTwoSidedConstantSurf, (brp_render_fn *)OpTriangleTwoSidedConstantSurfIndexed);
			else
				PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleConstantSurf, (brp_render_fn *)OpTriangleConstantSurfIndexed);
		}
		break;

	case BRT_TRIANGLE:

		if (renderer->state.cache.nconstant_fns) {

			PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleClipConstantSurf, (brp_render_fn *)OpTriangleClipConstantSurfIndexed);

			if (renderer->state.cull.type == BRT_TWO_SIDED)
				PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleTwoSidedConstantSurf, (brp_render_fn *)OpTriangleTwoSidedConstantSurfIndexed);
			else
				PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleConstantSurf, (brp_render_fn *)OpTriangleConstantSurfIndexed);
		} else
		   PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleClip, (brp_render_fn *)OpTriangleClipIndexed);

        /*
         * Optional subdivision if the primitive requests it
         */
#if 0	// Won't work very well with Direct3D
        if (rend.block->flags & BR_PRIMF_SUBDIVIDE) {
    		PrimBlockAdd(renderer, (brp_render_fn *)OpTriangleSubdivide, (brp_render_fn *)OpTriangleSubdivideIndexed);
	    	PrimBlockOnScreenAdd(renderer, (brp_render_fn *)OpTriangleSubdivideOnScreen, (brp_render_fn *)OpTriangleSubdivideOnScreenIndexed);
			SubdivideSetThreshold(rend.block->subdivide_tolerance);
        }
#endif
		break;
	}

	/*
	 * Two-sided relighting of vertices
	 */
	if ((renderer->state.cull.type == BRT_TWO_SIDED) && renderer->state.cache.nvertex_fns)
	   PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleRelightTwoSided, (brp_render_fn *)OpTriangleRelightTwoSidedIndexed);

	/*
	 * Environment wrapping fixup
	 */
	if ((renderer->state.surface.mapping_source == BRT_ENVIRONMENT_INFINITE)||
	   (renderer->state.surface.mapping_source == BRT_ENVIRONMENT_LOCAL))
	   PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleMappingWrapFix, (brp_render_fn *)OpTriangleMappingWrapFixIndexed);

	/*
	 * Quad based texture mapping
	 */
	if ((renderer->state.surface.mapping_source == BRT_QUAD))
	   PrimBlockAddBoth(renderer, (brp_render_fn *)OpTriangleMapQuad, (brp_render_fn *)OpTriangleMapQuadIndexed);

	/*
	 * Update cache comparison info.
	 */
	renderer->state.cache.format = self;
}


static br_error V1Model_Render
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
		struct br_renderer_state_stored *default_state,
		br_token type, br_boolean on_screen)
{
	int i, g;
	struct br_renderer_state_stored *state;
#if ENABLE_ZSORT
	br_boolean z_sort;
#endif
	br_error r;

	/*
	 * Remember current renderer and format in use
	 */
	rend.geometry = self;
	rend.renderer = renderer;

	rend.model_onscreen = on_screen;

	/*
	 * Make local copy of bounds
	 */
	scache.min = renderer->state.bounds.min;
	scache.max = renderer->state.bounds.max;

	/*
	 * Do per-scene and per-model updates if necessary
	 */
	if (!scache.valid_per_scene) {
		StaticCacheUpdate_PerScene(renderer);
		scache.valid_per_scene = BR_TRUE;
	}

	if (!scache.valid_per_model) {
		StaticCacheUpdate_PerModel(renderer);
		scache.valid_per_model = BR_TRUE;
	}

	/*
	 * Begin the scene if necessary
	 */
	if (!renderer->scene_started) {

		r = RendererSceneBegin(renderer);

		if (r != BRE_OK)
			return r;

		/*
		 * TEMPORARY - Set up D3D output state
		 */
		r = SetD3DOutputState(renderer);

		if (r != BRE_OK)
			return r;
	}

	/*
	 * Go through each group
	 */
	for (g = 0; g < model->ngroups; g++) {

		rend.faces = model->groups[g].faces;
		rend.vertices = model->groups[g].vertices;
		rend.face_colours = model->groups[g].face_colours;
		rend.vertex_colours = model->groups[g].vertex_colours;
		rend.face_flags = model->groups[g].face_flags;
		rend.nfaces = model->groups[g].nfaces;
		rend.nvertices = model->groups[g].nvertices;
		rend.nedges = model->groups[g].nedges;

		/*
	 	 * Restore state for group
		 */
		state = model->groups[g].stored?model->groups[g].stored:default_state;

		/*
		 * TEMPORARY - Set up D3D primitive state
		 */
		if (SetD3DPrimitiveState(renderer, state != NULL? &state->prim:
			&renderer->default_state->prim) != BRE_OK)

			continue;
				 
		if (state)
			RendererStateRestore(renderer, state, (br_uint_32)BR_STATE_ALL);

#if ENABLE_ZSORT
		z_sort = (renderer->state.hidden.type == BRT_BUCKET_SORT &&
				 renderer->state.hidden.order_table != NULL &&
				 renderer->state.hidden.heap != NULL);
#endif

		/*
		 * REALLY TEMPORARY - get brp_block etc.
		 */
		rend.block = GetBrpBlock(renderer, type);
		rend.block_changed = BR_FALSE;//BR_TRUE;
		rend.range_changed = BR_FALSE;//BR_TRUE;
		
		/*
		 * Calculate working data
		 */

		/*
		 * Merge opacity into alpha byte of colour
		 */
		scache.colour = renderer->state.surface.colour & 0xFFFFFF;
		scache.colour |= BrScalarToInt(BR_CONST_MUL(renderer->state.surface.opacity, 256)) << 24;

		/*
		 * Make sure base primitive block is hooked up to the right place
		 *
		 * Will add draw routine as normal eventually
		 */
		renderer->state.cache.face_blocks[0].chain = rend.block;
		renderer->state.cache.face_blocks_onscreen[0].chain = rend.block;

		if (rend.block_changed || rend.range_changed || !renderer->state.cache.valid) {
			CacheUpdate(renderer);
#if ENABLE_ZSORT
			V1Faces_GeometryFnsUpdate(self, renderer, z_sort);
#else
			V1Faces_GeometryFnsUpdate(self, renderer, BR_FALSE);
#endif
			renderer->state.cache.valid = BR_TRUE;
		}

#if 0
#if BASED_FIXED
		if (on_screen)
			ModelToViewportUpdate_A();
#endif
#endif

		/*
		 * Invoke the current set of renderer functions on the group
		 */
		if (on_screen) {
			for (i = 0; i < renderer->state.cache.ngeometry_fns_onscreen; i++)
				renderer->state.cache.geometry_fns_onscreen[i](self, renderer);
		} else {
			for (i = 0; i < renderer->state.cache.ngeometry_fns; i++)
				renderer->state.cache.geometry_fns[i](self, renderer);
		}

		/*
		 * Save any cached state
		 */
		if (state) {
			RendererStateSave(renderer, state, BR_STATE_CACHE);
			state->cache.valid = BR_TRUE;
		}
	}

	/*
	 * Put updated bounds back
	 */
	renderer->state.bounds.min = scache.min;
	renderer->state.bounds.max = scache.max;

	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, render)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
		struct br_renderer_state_stored *default_state,
		br_token type)
{
	br_error r;

	r = V1Model_Render(self, renderer, model, default_state, type, BR_FALSE);

	return r;
}

br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, renderOnScreen)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
		struct br_renderer_state_stored *default_state,
		br_token type)
{
	br_error r;

	r = V1Model_Render(self, renderer, model, default_state, type, BR_TRUE);

	return r;
}

