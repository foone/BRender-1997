/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: faceops.c 1.1 1997/07/11 16:14:02 jon Exp JON $
 * $Locker: JON $
 *
 * Face rendering functions that handle per-face operations, before actual rendering
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

#include "vecifns.h"

BR_RCS_ID("$Id: faceops.c 1.1 1997/07/11 16:14:02 jon Exp JON $");

/*
 * Clip a triangle
 */
void BR_ASM_CALL OpTriangleClipIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	D3DTLVERTEX vertex[3];
	int nclipped;
	D3DTLVERTEX *clipped_vertex;
	WORD *clipped_index;

	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];
	vertex[2] = rend.d3dtlvertices[index[2]];

	nclipped = FaceClip(rend.renderer, &clipped_index, &clipped_vertex, index, vertex, 3, rend.renderer->state.cache.clip_slots, tfp->codes);
	
	if (nclipped != 0 && block->chain->render != NULL)
		ClippedRenderTriangles(rend.renderer, block->chain, clipped_index, clipped_vertex, nclipped, fp, tfp);
}

void BR_ASM_CALL OpTriangleClip(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
	int nclipped;
	D3DTLVERTEX *clipped_vertex;
	WORD *clipped_index;

	nclipped = FaceClip(rend.renderer, &clipped_index, &clipped_vertex, index, vertex, 3, rend.renderer->state.cache.clip_slots, tfp->codes);
	
	if (nclipped != 0 && block->chain->render != NULL)
		ClippedRenderTriangles(rend.renderer, block->chain, clipped_index, clipped_vertex, nclipped, fp, tfp);
}

/*
 * Clip a triangle and generate per primitive components
 */
void BR_ASM_CALL OpTriangleClipConstantSurfIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	D3DTLVERTEX vertex[3];
	int nclipped, i;
	D3DTLVERTEX *clipped_vertex;
	WORD *clipped_index;
	struct fmt_vertex *vp;
	br_colour colour = scache.colour;
	br_vector3 rev_normal;
	
	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];
	vertex[2] = rend.d3dtlvertices[index[2]];

	/*
     * Face needs to be clipped
     */
	nclipped = FaceClip(rend.renderer, &clipped_index, &clipped_vertex, index, vertex, 3, rend.renderer->state.cache.clip_slots, tfp->codes);
	

	if (nclipped != 0) {

		vp = rend.vertices + fp->vertices[0];

		if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
			colour = rend.face_colours[fp-rend.faces];


		if (tfp->flag & TFF_REVERSED) {

			BrVector3Negate(&rev_normal, (br_vector3 *)&fp->eqn);

			for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
				rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, &rev_normal, colour, &clipped_vertex[0]);

		} else {
			for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
				rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, (br_vector3 *)&fp->eqn, colour, &clipped_vertex[0]);
		}

		if (block->chain->render != NULL)
			ClippedRenderTriangles(rend.renderer, block->chain, clipped_index, clipped_vertex, nclipped, fp, tfp);
	}
}

void BR_ASM_CALL OpTriangleClipConstantSurf(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
	int nclipped, i;
	D3DTLVERTEX *clipped_vertex;
	WORD *clipped_index;
	struct fmt_vertex *vp;
	br_colour colour = scache.colour;
	br_vector3 rev_normal;

	/*
     * Face needs to be clipped
     */
	nclipped = FaceClip(rend.renderer, &clipped_index, &clipped_vertex, index, vertex, 3, rend.renderer->state.cache.clip_slots, tfp->codes);
	
	if (nclipped != 0) {

		vp = rend.vertices + fp->vertices[0];

		if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
			colour = rend.face_colours[fp-rend.faces];


		if (tfp->flag & TFF_REVERSED) {

			BrVector3Negate(&rev_normal, (br_vector3 *)&fp->eqn);

			for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
				rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, &rev_normal, colour, &clipped_vertex[0]);

		} else {
			for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
				rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, (br_vector3 *)&fp->eqn, colour, &clipped_vertex[0]);
		}

		if (block->chain->render != NULL)
			ClippedRenderTriangles(rend.renderer, block->chain, clipped_index, clipped_vertex, nclipped, fp, tfp);
	}
}

/*
 * Generate per primitive components
 */
void BR_ASM_CALL OpTriangleConstantSurfIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	struct fmt_vertex *vp;
	br_colour colour = scache.colour;
	int i;

	vp = rend.vertices + fp->vertices[0];

	if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
		colour = rend.face_colours[fp-rend.faces];

	for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
		rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, (br_vector3 *)&fp->eqn, colour, &rend.d3dtlvertices[index[0]]);

	block->chain->render_indexed(block->chain, index, BR_TRUE, fp, tfp);
}

void BR_ASM_CALL OpTriangleConstantSurf(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
	struct fmt_vertex *vp;
	br_colour colour = scache.colour;
	int i;

	vp = rend.vertices + fp->vertices[0];

	if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
		colour = rend.face_colours[fp-rend.faces];

	for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
		rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, (br_vector3 *)&fp->eqn, colour, &vertex[0]);

	if (block->chain->render != NULL)
		block->chain->render(block->chain, index, vertex, fp, tfp);
}

void BR_ASM_CALL OpTriangleTwoSidedConstantSurfIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	struct fmt_vertex *vp;
	br_colour colour = scache.colour;
	br_vector3 rev_normal;
	int i;

	vp = rend.vertices + fp->vertices[0];

	if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
		colour = rend.face_colours[fp-rend.faces];

	if (tfp->flag & TFF_REVERSED) {

		BrVector3Negate(&rev_normal, (br_vector3 *)&fp->eqn);

		for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
			rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, &rev_normal, colour, &rend.d3dtlvertices[index[0]]);

	} else {
		for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
			rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, (br_vector3 *)&fp->eqn, colour, &rend.d3dtlvertices[index[0]]);
	}

	block->chain->render_indexed(block->chain, index, BR_TRUE, fp, tfp);
}

void BR_ASM_CALL OpTriangleTwoSidedConstantSurf(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
	struct fmt_vertex *vp;
	br_colour colour = scache.colour;
	br_vector3 rev_normal;
	int i;

	vp = rend.vertices + fp->vertices[0];

	if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
		colour = rend.face_colours[fp-rend.faces];

	if (tfp->flag & TFF_REVERSED) {

		BrVector3Negate(&rev_normal, (br_vector3 *)&fp->eqn);

		for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
			rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, &rev_normal, colour, &vertex[0]);

	} else {
		for (i = 0; i < rend.renderer->state.cache.nconstant_fns; i++)
			rend.renderer->state.cache.constant_fns[i](rend.renderer, &vp->p, &vp->map, (br_vector3 *)&fp->eqn, colour, &vertex[0]);
	}

	if (block->chain->render != NULL)
		block->chain->render(block->chain, index, vertex, fp, tfp);
}

/*
 * Fix up ugly seams in environment mapping
 */
void BR_ASM_CALL OpTriangleMappingWrapFixIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	D3DTLVERTEX vertex[3];
	
	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];
	vertex[2] = rend.d3dtlvertices[index[2]];

	OpTriangleMappingWrapFix(block, index, vertex, fp, tfp);
}

void BR_ASM_CALL OpTriangleMappingWrapFix(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
	D3DVALUE d0, d1, d2;
	D3DTLVERTEX fixed[3];
	
	/*
	 * If change in U along any edge is > 0.5, then adjust one of the vertices to bring it
	 * into range (by adding 1)
	 */
	d0 = vertex[1].dvTU - vertex[0].dvTU;
	d1 = vertex[2].dvTU - vertex[1].dvTU;
	d2 = vertex[0].dvTU - vertex[2].dvTU;


	if ((d0 > 0.5) || (d0 < -0.5) ||
		(d1 > 0.5) || (d1 < -0.5) ||
		(d2 > 0.5) || (d2 < -0.5)) {

		fixed[0] = vertex[0];
		fixed[1] = vertex[2];
		fixed[2] = vertex[2];
		
		if (d0 > 0.5 || d2 < -0.5)
			fixed[0].dvTU += 1.0;
	
		if (d0 < -0.5 || d1 > 0.5)
			fixed[1].dvTU += 1.0;

		if (d1 < -0.5 || d2 > 0.5)
			fixed[2].dvTU += 1.0;

		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, fixed, fp, tfp);

	} else

		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, vertex, fp, tfp);
}

/*
 * handle relighting of vertices if a triangle is two-sided
 */
void BR_ASM_CALL OpTriangleRelightTwoSidedIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	int i, v;
	D3DTLVERTEX vertex[3];
	struct fmt_vertex *vp;
   	br_vector3 rev_normal;
	br_colour colour = scache.colour;

	if (tfp->flag & TFF_REVERSED) {

		vertex[0] = rend.d3dtlvertices[index[0]];
		vertex[1] = rend.d3dtlvertices[index[1]];
		vertex[2] = rend.d3dtlvertices[index[2]];

		for (v = 0; v < 3; v++)

			if ((rend.d3dhvertices[index[v]].dwFlags & (TVDIR_FRONT | TVDIR_BACK)) == (TVDIR_FRONT | TVDIR_BACK)) {

				vp = rend.vertices+index[v]; 

				if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
					colour = rend.vertex_colours[vp - rend.vertices];

				BrVector3Negate(&rev_normal, &vp->n);

				for (i = 0; i < rend.renderer->state.cache.nvertex_fns; i++)
					rend.renderer->state.cache.vertex_fns[i](rend.renderer, &vp->p, &vp->map, &rev_normal, colour, &vertex[v]);
			}

		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, vertex, fp, tfp);
	
	} else

		block->chain->render_indexed(block->chain, index, modified, fp, tfp);
}

void BR_ASM_CALL OpTriangleRelightTwoSided(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
	int i, v;
	struct fmt_vertex *vp;
   	br_vector3 rev_normal;
	br_colour colour = scache.colour;

	if (tfp->flag & TFF_REVERSED)

		for (v = 0; v < 3; v++)

			if ((rend.d3dhvertices[index[v]].dwFlags & (TVDIR_FRONT | TVDIR_BACK)) == (TVDIR_FRONT | TVDIR_BACK)) {

				vp = rend.vertices+index[v]; 

				if (rend.renderer->state.surface.colour_source == BRT_GEOMETRY)
					colour = rend.vertex_colours[vp - rend.vertices];

				BrVector3Negate(&rev_normal, &vp->n);

				for (i = 0; i < rend.renderer->state.cache.nvertex_fns; i++)
					rend.renderer->state.cache.vertex_fns[i](rend.renderer, &vp->p, &vp->map, &rev_normal, colour, &vertex[v]);
			}

	if (block->chain->render != NULL)
		block->chain->render(block->chain, index, vertex, fp, tfp);
}

/*
 * Convert a triangle to lines
 */
void BR_ASM_CALL OpTriangleToLinesIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	D3DTLVERTEX vertex[3];
	
	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];
	vertex[2] = rend.d3dtlvertices[index[2]];

	OpTriangleToLines(block, index, vertex, fp, tfp);
}

void BR_ASM_CALL OpTriangleToLines(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
#if 0
	/*
	 * Generate a line for each unrendered edge
	 */
	if (!rend.edge_flags[fp->edges[0]]) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v0, v1);
		rend.edge_flags[fp->edges[0]] = 1;
	}

	if (!rend.edge_flags[fp->edges[1]]) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v1, v2);
		rend.edge_flags[fp->edges[1]] = 1;
	}

	if (!rend.edge_flags[fp->edges[2]]) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v2, v0);
		rend.edge_flags[fp->edges[2]] = 1;
	}
#endif
}

void BR_ASM_CALL OpTriangleToPointsIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	D3DTLVERTEX vertex[3];
	
	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];
	vertex[2] = rend.d3dtlvertices[index[2]];

	OpTriangleToPoints(block, index, vertex, fp, tfp);
}

void BR_ASM_CALL OpTriangleToPoints(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex, struct v11face *fp, struct temp_face *tfp)
{
#if 0
	/*
	 * Generate a point for each unrendered vertex
	 */
	if (!rend.vertex_flags[fp->vertices[0]] && !(v0->flags & OUTCODES_ALL)) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v0);
		rend.vertex_flags[fp->vertices[0]] = 1;
	}

	if (!rend.vertex_flags[fp->vertices[1]] && !(v1->flags & OUTCODES_ALL)) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v1);
		rend.vertex_flags[fp->vertices[1]] = 1;
	}

	if (!rend.vertex_flags[fp->vertices[2]] && !(v2->flags & OUTCODES_ALL)) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v2);
		rend.vertex_flags[fp->vertices[2]] = 1;
	}
#endif
}

void BR_ASM_CALL OpTriangleToPointsIndexed_OS(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	D3DTLVERTEX vertex[3];
	
	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];
	vertex[2] = rend.d3dtlvertices[index[2]];

	OpTriangleToPoints_OS(block, index, vertex, fp, tfp);
}

void BR_ASM_CALL OpTriangleToPoints_OS(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex,	struct v11face *fp, struct temp_face *tfp)
{
#if 0
	/*
	 * Generate a point for each unrendered vertex
	 */
	if (!rend.vertex_flags[fp->vertices[0]]) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v0);
		rend.vertex_flags[fp->vertices[0]] = 1;
	}

	if (!rend.vertex_flags[fp->vertices[1]]) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v1);
		rend.vertex_flags[fp->vertices[1]] = 1;
	}

	if (!rend.vertex_flags[fp->vertices[2]]) {
		if (block->chain->render != NULL)
			block->chain->render(block->chain, index, v2);
		rend.vertex_flags[fp->vertices[2]] = 1;
	}
#endif
}

void BR_ASM_CALL OpLineClipIndexed(struct brp_block *block, WORD *index, br_boolean modified)
{
	D3DTLVERTEX vertex[2];
	
	vertex[0] = rend.d3dtlvertices[index[0]];
	vertex[1] = rend.d3dtlvertices[index[1]];

	OpLineClip(block, index, vertex);
}

void BR_ASM_CALL OpLineClip(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex)
{
#if 0
	brp_vertex clipped[2];

	if (ClipLine(rend.renderer, clipped, v0, v1, rend.renderer->state.cache.clip_slots, v0->flags | v1->flags))
		ClippedRenderLine(rend.renderer, block->chain, clipped);
#endif
}

#if 0	// Won't work too well with Direct3D because it stores 1/W
/**
 ** Triangle subdivision
 **/
#if BASED_FIXED
void BR_ASM_CALL averageVertices(struct br_renderer *renderer, brp_vertex *dest, brp_vertex *src1, brp_vertex *src2);
void BR_ASM_CALL averageVerticesOnScreen(struct br_renderer *renderer, brp_vertex *dest, brp_vertex *src1, brp_vertex *src2);
#else
void BR_ASM_CALL averageVerticesOnScreen(struct br_renderer *renderer, brp_vertex *dest1, brp_vertex *dest2, brp_vertex *dest3, brp_vertex *src1, brp_vertex *src2, brp_vertex *src3);
void BR_ASM_CALL averageVertices(struct br_renderer *renderer, brp_vertex *dest1, brp_vertex *dest2, brp_vertex *dest3, brp_vertex *src1, brp_vertex *src2, brp_vertex *src3);
#endif
br_boolean BR_ASM_CALL subdivideCheck(brp_vertex *v0, brp_vertex *v1, brp_vertex *v2);
/*
 * Find and project midpoint of two vertices
 */

#if BASED_FIXED
void BR_ASM_CALL averageVertices(struct br_renderer *renderer, brp_vertex *dest, brp_vertex *src1, brp_vertex *src2)
{
    dest->comp[C_X] = BR_CONST_DIV(BR_ADD(src1->comp[C_X], src2->comp[C_X]),2);
    dest->comp[C_Y] = BR_CONST_DIV(BR_ADD(src1->comp[C_Y], src2->comp[C_Y]),2);

    dest->comp[C_Z] = BR_CONST_DIV(BR_ADD(src1->comp[C_Z], src2->comp[C_Z]),2);
    dest->comp[C_W] = BR_CONST_DIV(BR_ADD(src1->comp[C_W], src2->comp[C_W]),2);

    dest->comp[C_U] = BR_CONST_DIV(BR_ADD(src1->comp[C_U], src2->comp[C_U]),2);
    dest->comp[C_V] = BR_CONST_DIV(BR_ADD(src1->comp[C_V], src2->comp[C_V]),2);

    dest->comp[C_I] = BR_CONST_DIV(BR_ADD(src1->comp[C_I], src2->comp[C_I]),2);
    dest->comp[C_R] = BR_CONST_DIV(BR_ADD(src1->comp[C_R], src2->comp[C_R]),2);

    dest->comp[C_G] = BR_CONST_DIV(BR_ADD(src1->comp[C_G], src2->comp[C_G]),2);
    dest->comp[C_B] = BR_CONST_DIV(BR_ADD(src1->comp[C_B], src2->comp[C_B]),2);

    dest->flags = OUTCODES_NOT;

   	OUTCODE_POINT(dest->flags, (br_vector4 *)(dest->comp+C_X));

	if (!(dest->flags & OUTCODES_ALL)) {
        PROJECT_VERTEX(dest, dest->comp[C_X], dest->comp[C_Y], dest->comp[C_Z], dest->comp[C_W]);
    }
}
#endif

#if BASED_FIXED
/*
 * Find and project midpoint of two vertices - on screen case
 */
STATIC void BR_ASM_CALL averageVerticesOnScreen(struct br_renderer *renderer, brp_vertex *dest, brp_vertex *src1, brp_vertex *src2)
{
    dest->comp[C_X] = BR_CONST_DIV(BR_ADD(src1->comp[C_X], src2->comp[C_X]),2);
    dest->comp[C_Y] = BR_CONST_DIV(BR_ADD(src1->comp[C_Y], src2->comp[C_Y]),2);

    dest->comp[C_Z] = BR_CONST_DIV(BR_ADD(src1->comp[C_Z], src2->comp[C_Z]),2);
    dest->comp[C_W] = BR_CONST_DIV(BR_ADD(src1->comp[C_W], src2->comp[C_W]),2);

    dest->comp[C_U] = BR_CONST_DIV(BR_ADD(src1->comp[C_U], src2->comp[C_U]),2);
    dest->comp[C_V] = BR_CONST_DIV(BR_ADD(src1->comp[C_V], src2->comp[C_V]),2);

    dest->comp[C_I] = BR_CONST_DIV(BR_ADD(src1->comp[C_I], src2->comp[C_I]),2);
    dest->comp[C_R] = BR_CONST_DIV(BR_ADD(src1->comp[C_R], src2->comp[C_R]),2);

    dest->comp[C_G] = BR_CONST_DIV(BR_ADD(src1->comp[C_G], src2->comp[C_G]),2);
    dest->comp[C_B] = BR_CONST_DIV(BR_ADD(src1->comp[C_B], src2->comp[C_B]),2);

    PROJECT_VERTEX(dest, dest->comp[C_X], dest->comp[C_Y], dest->comp[C_Z], dest->comp[C_W]);
}
#endif

/*
 * Test for whether a face should be subdivided
 */
#define SUBDIVIDE_THRESHOLD BR_SCALAR(0.9)


#if 0
STATIC br_boolean subdivideCheck(brp_vertex *v0, brp_vertex *v1, brp_vertex *v2)
{
#if 0
    br_scalar lu, hu, lv, hv, q0, q1, q2;

    /*
     * Work out linear and hyperbolic interpolations of U and V at centre of triangle
     */
    lu = BR_CONST_DIV(v0->comp[C_U] + v1->comp[C_U] + v2->comp[C_U],3);
    lv = BR_CONST_DIV(v0->comp[C_V] + v1->comp[C_V] + v2->comp[C_V],3);

    q0 = BR_RCP(v0->comp[C_W]);
    q1 = BR_RCP(v1->comp[C_W]);
    q2 = BR_RCP(v2->comp[C_W]);

    hu = BR_MAC3DIV(v0->comp[C_U], q0, v1->comp[C_U], q1, v2->comp[C_U], q2, q0+q1+q2) ;
    hv = BR_MAC3DIV(v0->comp[C_V], q0, v1->comp[C_V], q1, v2->comp[C_V], q2, q0+q1+q2) ;

    /*
     * If error is greater than threshold, then don't subdivide
     */
    if (BR_ABS(lu-hu) < BR_SCALAR(2.0) && BR_ABS(lv-hv) < BR_SCALAR(2.0)) {
        return BR_FALSE;
    }
#endif

#if 1
    br_scalar z0, z1, z2, zt;

    /*
     * If the ratio of the smallest and largest Z is greater
     * than a threshold, then subdivide
     */
    z0 = BR_ABS(v0->comp[C_Z]);
    z1 = BR_ABS(v1->comp[C_Z]);
    z2 = BR_ABS(v2->comp[C_Z]);

    if (z0 > z1) {
        zt = z0; z0 = z1; z1 = zt;
    }

    if (z0 > z2) {
        zt = z0; z0 = z2; z2 = zt;
    }

    if (z1 > z2) {
        zt = z1; z1 = z2; z2 = zt;
    }

    if (z2 == BR_SCALAR(0.0))
        return BR_FALSE;

    if (z0 > BR_MUL(rend.subdivide_threshold, z2))
        return BR_FALSE;
#endif

    return BR_TRUE;
}
#endif


/*
 * Convert a generic tolerance spec. into a threshold for the above test
 */
void SubdivideSetThreshold(br_int_32 subdivide_tolerance)
{
	/*
	 * Range from 1/(1.05) to 1/(1.45) 0.95 -> 0.68
	 */
	rend.subdivide_threshold = BR_RCP(BR_MUL(BrIntToScalar(subdivide_tolerance), BR_SCALAR(-0.002)) + BR_SCALAR(1.25));
}


/*
 * Recursive function to subdivied a triangle - looks at outcode to see whether triangle should
 * be processed, and whether it is completely on screen
 */
STATIC void triangleSubdivideCheck(int depth, struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp);
STATIC void triangleSubdivideOnScreen(int depth, struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
    struct v11face *fp, struct temp_face *tfp);


STATIC void triangleSubdivide(int depth, struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp)
{
	br_uint_32 combined_codes;

    /*
     * If triangle is not visible - then ignore it
     */
	combined_codes = v0->flags | v1->flags | v2->flags;

    if ((combined_codes & OUTCODES_NOT) != OUTCODES_NOT)
        return;

	if (combined_codes & OUTCODES_ALL) {
        triangleSubdivideCheck(depth, block, v0, v1, v2, fp, tfp);
    } else{
        triangleSubdivideOnScreen(depth, block, v0, v1, v2, fp, tfp);
    }
}

/*
 * Special case of subdivision for on screen triangles
 */
STATIC void triangleSubdivideOnScreen(int depth, struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
    struct v11face *fp, struct temp_face *tfp)
{
    brp_vertex mid0, mid1, mid2;

    if (depth > 0 && subdivideCheck(v0, v1, v2)) {
        /*
         * Subdivide edges and reproject
         */
#if BASED_FIXED
		averageVerticesOnScreen(rend.renderer, &mid0, v0, v1);
        averageVerticesOnScreen(rend.renderer, &mid1, v1, v2);
        averageVerticesOnScreen(rend.renderer, &mid2, v2, v0);
#else
		averageVerticesOnScreen(rend.renderer,&mid1,&mid2,&mid0, v0, v1, v2);
#endif
        triangleSubdivideOnScreen(depth-1, block, &mid0, &mid1, &mid2, fp, tfp);
        triangleSubdivideOnScreen(depth-1, block, v0, &mid0, &mid2, fp, tfp);
        triangleSubdivideOnScreen(depth-1, block, v1, &mid1, &mid0, fp, tfp);
        triangleSubdivideOnScreen(depth-1, block, v2, &mid2, &mid1, fp, tfp);
    } else {
        block->chain->render(block->chain, index, v0, v1, v2, fp, tfp);
    }
}

/*
 * Subdivide a triangle that might be off screen
 */

STATIC void triangleSubdivideCheck(int depth, struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp)
{
    brp_vertex mid0, mid1, mid2;

    if (depth > 0 && subdivideCheck(v0, v1, v2)) {
        /*
         * Subdivide edges and reproject
         */
#if BASED_FIXED
		averageVertices(rend.renderer, &mid0, v0, v1);
        averageVertices(rend.renderer, &mid1, v1, v2);
        averageVertices(rend.renderer, &mid2, v2, v0);
#else
		averageVertices(rend.renderer,&mid1,&mid2,&mid0, v0, v1, v2);
#endif

        triangleSubdivide(depth-1, block, &mid0, &mid1, &mid2, fp, tfp);
        triangleSubdivide(depth-1, block, v0, &mid0, &mid2, fp, tfp);
        triangleSubdivide(depth-1, block, v1, &mid1, &mid0, fp, tfp);
        triangleSubdivide(depth-1, block, v2, &mid2, &mid1, fp, tfp);
    } else {
        block->chain->render(block->chain, index, v0, v1, v2, fp, tfp);
    }
}

/*
 * Faces ops for subdivision
 */
void BR_ASM_CALL OpTriangleSubdivide(struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp)
{
    triangleSubdivide(6, block, v0, v1, v2, fp, tfp);
}

void BR_ASM_CALL OpTriangleSubdivideOnScreen(struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp)
{
    triangleSubdivideOnScreen(6, block, v0, v1, v2, fp, tfp);
}

#endif
