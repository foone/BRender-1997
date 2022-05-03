/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: genrend.c 1.1 1997/07/11 16:14:05 jon Exp JON $
 * $Locker: JON $
 *
 * Geometry operations that are used for several different formats
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"
#include "blockops.h"

#include "vecifns.h"


/*
 * Transform, project, outcode and update screen bounds of visible vertices 
 */
#ifndef Vertex_TransformProjectOutcodeBounds
static void GEOMETRY_CALL Vertex_TransformProjectOutcodeBounds(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		OUTCODE_POINT(d3dhvp, d3dhwp);

		if (!(d3dhvp->dwFlags & OUTCODES_ALL)) {
			PROJECT_VERTEX(d3dhvp, d3dtlvp);
			UPDATE_BOUNDS(d3dtlvp);
		}
	}
}
#endif

/*
 * Transform, project and outcode of visible vertices 
 */
#ifndef Vertex_TransformProjectOutcode
static void GEOMETRY_CALL Vertex_TransformProjectOutcode(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		OUTCODE_POINT(d3dhvp, d3dhwp);

		if (!(d3dhvp->dwFlags & OUTCODES_ALL))
			PROJECT_VERTEX(d3dhvp, d3dtlvp);
	}
}
#endif

/*
 * Transform, project and update screen bounds of visible vertices 
 */
#ifndef Vertex_OS_TransformProjectBounds
static void GEOMETRY_CALL Vertex_OS_TransformProjectBounds(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
		UPDATE_BOUNDS(d3dtlvp);
	}
}
#endif

/*
 * Transform and project visible vertices 
 */
#ifndef Vertex_OS_TransformProject
static void GEOMETRY_CALL Vertex_OS_TransformProject(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
	}
}
#endif

/*
 * Transform, project, light and update screen bounds of visible vertices 
 */
#ifndef Vertex_OS_TransformProjectBoundsSurf
static void GEOMETRY_CALL Vertex_OS_TransformProjectBoundsSurf(struct br_geometry *self, struct br_renderer *renderer)
{
	int v, i;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	struct fmt_vertex *vp = rend.vertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
		UPDATE_BOUNDS(d3dtlvp);

		for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
			renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, scache.colour, d3dtlvp);
	}
}
#endif

/*
 * Transform, project and light visible vertices 
 */
#ifndef Vertex_OS_TransformProjectSurf
static void GEOMETRY_CALL Vertex_OS_TransformProjectSurf(struct br_geometry *self, struct br_renderer *renderer)
{
	int v, i;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	struct fmt_vertex *vp = rend.vertices;

#if TRANSFORMPROJECT_SEPARATE_LOOP
	Vertex_OS_TransformProject(self,renderer);
#endif

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

#if !TRANSFORMPROJECT_SEPARATE_LOOP
		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
#endif

		for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
			renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, scache.colour, d3dtlvp);
	}
}
#endif

/*
 * Transform, project, light and update screen bounds of visible vertices 
 */
#ifndef Vertex_OS_TransformProjectBoundsGeom
static void GEOMETRY_CALL Vertex_OS_TransformProjectBoundsGeom(struct br_geometry *self, struct br_renderer *renderer)
{
	int v, i;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	struct fmt_vertex *vp = rend.vertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
		UPDATE_BOUNDS(d3dtlvp);

		for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
			renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, rend.vertex_colours[v], d3dtlvp);
	}
}
#endif

/*
 * Transform, project and light visible vertices 
 */
#ifndef Vertex_OS_TransformProjectGeom
static void GEOMETRY_CALL Vertex_OS_TransformProjectGeom(struct br_geometry *self, struct br_renderer *renderer)
{
	int v, i;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	struct fmt_vertex *vp = rend.vertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);

		for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
			renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, rend.vertex_colours[v], d3dtlvp);
	}
}
#endif

#if 0
/*
 * Transform, project and update screen bounds of all vertives
 */
#ifndef Vertex_OSV_TransformProjectBounds
static void GEOMETRY_CALL Vertex_OSV_TransformProjectBounds(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
		UPDATE_BOUNDS(d3dtlvp);
	}
}
#endif

/*
 * Transform and project all vertives
 */
#ifndef Vertex_OSV_TransformProject
static void GEOMETRY_CALL Vertex_OSV_TransformProject(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;

	for (v = 0; v < rend.nvertices; v++, d3dhvp++, d3dhwp++, d3dtlvp++) {

		TRANSFORM_VERTEX(d3dhvp, d3dhwp, d3dtlvp, &rend.vertices[v].p, &scache.model_to_screen);
		PROJECT_VERTEX(d3dhvp, d3dtlvp);
	}
}
#endif
#endif

/*
 * Generate per-vertex surface parmeters, colour from surface
 */
#ifndef Vertex_SurfaceComponentsSurf
static void GEOMETRY_CALL Vertex_SurfaceComponentsSurf(struct br_geometry *self, struct br_renderer *renderer)
{
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	struct fmt_vertex *vp = rend.vertices;
	int v,i;

	/*
	 * Simple hack for mapping
	 */
	for (v = 0; v < rend.nvertices; v++, vp++, d3dtlvp++) {

		if(rend.vertex_counts[v] == 0)
			continue;

		for(i=0; i < renderer->state.cache.nvertex_fns; i++)
			renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, scache.colour, d3dtlvp);
	}
}
#endif

#ifndef Vertex_SurfaceComponentsTwoSidedSurf
static void GEOMETRY_CALL Vertex_SurfaceComponentsTwoSidedSurf(struct br_geometry *self, struct br_renderer *renderer)
{
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	struct fmt_vertex *vp = rend.vertices;
	int v,i;
	br_vector3 rev_normal;

	for (v = 0; v < rend.nvertices; v++, vp++, d3dtlvp++, d3dhvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		if (d3dhvp->dwFlags & TVDIR_FRONT) {
			for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
				renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, scache.colour, d3dtlvp);

		} else {

			BrVector3Negate(&rev_normal, &vp->n);

			for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
				renderer->state.cache.vertex_fns[i](renderer,  &vp->p, &vp->map, &rev_normal, scache.colour, d3dtlvp);
		}
	}
}
#endif

/*
 * Generate per-vertex surface parmeters, colour from geometry
 */
#ifndef Vertex_SurfaceComponentsGeom
static void GEOMETRY_CALL Vertex_SurfaceComponentsGeom(struct br_geometry *self, struct br_renderer *renderer)
{
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	struct fmt_vertex *vp = rend.vertices;
	int v,i;


	for (v = 0; v < rend.nvertices; v++, vp++, d3dtlvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
			renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, rend.vertex_colours[v], d3dtlvp);
	}
}
#endif

#ifndef Vertex_SurfaceComponentsTwoSidedGeom
static void GEOMETRY_CALL Vertex_SurfaceComponentsTwoSidedGeom(struct br_geometry *self, struct br_renderer *renderer)
{
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	struct fmt_vertex *vp = rend.vertices;
	br_vector3 rev_normal;
	int v,i;

	for (v = 0; v < rend.nvertices; v++, vp++, d3dtlvp++, d3dhvp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		if (d3dhvp->dwFlags & TVDIR_FRONT) {

			for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
				renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &vp->n, rend.vertex_colours[v], d3dtlvp);

		} else {

			BrVector3Negate(&rev_normal, &vp->n);

			for (i = 0; i < renderer->state.cache.nvertex_fns; i++)
				renderer->state.cache.vertex_fns[i](renderer, &vp->p, &vp->map, &rev_normal, rend.vertex_colours[v], d3dtlvp);
		}
	}
}
#endif

/*
 * Handles force front by fixing up C_Z and C_SZ
 */
#ifndef Vertex_ForceFront
static void GEOMETRY_CALL Vertex_ForceFront(struct br_geometry *self, struct br_renderer *renderer)
{
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	int v;

	for (v = 0; v < rend.nvertices; v++, d3dtlvp++, d3dhvp++, d3dhwp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		d3dtlvp->dvSZ = 0.0f;
		d3dhvp->dvHZ = *d3dhwp - FLT_EPSILON;
	}
}
#endif

#ifndef Vertex_ForceBack
static void GEOMETRY_CALL Vertex_ForceBack(struct br_geometry *self, struct br_renderer *renderer)
{
	D3DTLVERTEX *d3dtlvp = rend.d3dtlvertices;
	D3DHVERTEX *d3dhvp = rend.d3dhvertices;
	D3DVALUE *d3dhwp = rend.d3dhw;
	int v;

	for (v = 0; v < rend.nvertices; v++, d3dtlvp++, d3dhvp++, d3dhwp++) {

		if (rend.vertex_counts[v] == 0)
			continue;

		d3dtlvp->dvSZ = 1.0f - FLT_EPSILON;
		d3dhvp->dvHZ = FLT_EPSILON - *d3dhwp;
	}
}
#endif

#ifndef ScratchFree
void GEOMETRY_CALL ScratchFree(struct br_geometry *self, struct br_renderer *renderer)
{
		BrScratchFree(rend.scratch);
	
		rend.scratch = NULL;
}
#endif

/*
 * Clear vertex flags
 */
#ifndef Vertex_ClearFlags
void GEOMETRY_CALL Vertex_ClearFlags(struct br_geometry *self, struct br_renderer *renderer)
{
	int v;

	for(v=0; v< rend.nvertices; v++)
		rend.d3dhvertices[v].dwFlags = OUTCODES_NOT;
}
#endif

/*
 * Select per vertex geometry operations
 */
void VertexGeometryFns(struct br_renderer *renderer, geometry_fn *prim_outcode)
{

	/**
 	 ** Partially on screen geometry
	 **/
	if(renderer->state.enable.flags & ENBL_BOUNDS)
		GeometryFunctionAdd(renderer, Vertex_TransformProjectOutcodeBounds);
	else
#if 0
		if (renderer->device->hostInfo.capabilities & HOST_CAPS_CMOV)
			GeometryFunctionAdd(renderer, Vertex_TransformProjectOutcode_CMOV);
		else
#endif
			GeometryFunctionAdd(renderer, Vertex_TransformProjectOutcode);

	/*
	 * Apply any per-primitive outcoding to further reduce the visible primitives
	 * and vertices
	 */
	if(prim_outcode)
		GeometryFunctionAdd(renderer, prim_outcode);

	if(renderer->state.cache.nvertex_fns) {
		if(renderer->state.cull.type == BRT_TWO_SIDED) {
			if(renderer->state.surface.colour_source == BRT_GEOMETRY)
				GeometryFunctionAdd(renderer, Vertex_SurfaceComponentsTwoSidedGeom);
			else
				GeometryFunctionAdd(renderer, Vertex_SurfaceComponentsTwoSidedSurf);
		} else {
			if(renderer->state.surface.colour_source == BRT_GEOMETRY)
				GeometryFunctionAdd(renderer, Vertex_SurfaceComponentsGeom);
			else
				GeometryFunctionAdd(renderer, Vertex_SurfaceComponentsSurf);
		}
	}

	/**
	 ** Fully on screen geometry
	 **/
	if(renderer->state.cache.nvertex_fns) {
		if(renderer->state.cull.type == BRT_TWO_SIDED) {
			if(renderer->state.enable.flags & ENBL_BOUNDS)
				GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProjectBounds);
			else
				GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProject);

			if(renderer->state.surface.colour_source == BRT_GEOMETRY)
				GeometryFunctionOnScreenAdd(renderer, Vertex_SurfaceComponentsTwoSidedGeom);
			else
				GeometryFunctionOnScreenAdd(renderer, Vertex_SurfaceComponentsTwoSidedSurf);

		} else {
			if(renderer->state.enable.flags & ENBL_BOUNDS) {
				if(renderer->state.surface.colour_source == BRT_GEOMETRY)
					GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProjectBoundsGeom);
				else
					GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProjectBoundsSurf);
			} else {
				if(renderer->state.surface.colour_source == BRT_GEOMETRY)
					GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProjectGeom);
				else
					GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProjectSurf);
			}
		}
	} else {
		if(renderer->state.enable.flags & ENBL_BOUNDS)
			GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProjectBounds);
		else
			GeometryFunctionOnScreenAdd(renderer, Vertex_OS_TransformProject);
	}

	/*
	 * Tweak for FORCE_FRONT style
	 */
	if(renderer->state.surface.force_front)
		GeometryFunctionBothAdd(renderer, Vertex_ForceFront);

	if(renderer->state.surface.force_back)
		GeometryFunctionBothAdd(renderer, Vertex_ForceBack);
}
