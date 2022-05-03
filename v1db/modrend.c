/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: modrend.c 2.6 1996/09/06 12:01:00 sam Exp $
 * $Locker: $
 *
 * Model rendering
 */
#include "v1db.h"
#include "shortcut.h"
#include "brassert.h"
#include "formats.h"

BR_RCS_ID("$Id: modrend.c 2.6 1996/09/06 12:01:00 sam Exp $")

/*
 * Render model faces
 */
static void renderFaces(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
	if(model->stored) {
		if(on_screen == BRT_ACCEPT)
			GeometryStoredRenderOnScreen(model->stored, v1db.renderer);
		else
			GeometryStoredRender(model->stored, v1db.renderer);

	} else if(model->prepared) {
		if(on_screen == BRT_ACCEPT)
			GeometryV1ModelRenderOnScreen(v1db.format_model, v1db.renderer, model->prepared, material->stored, BRT_TRIANGLE);
		else
			GeometryV1ModelRender(v1db.format_model, v1db.renderer, model->prepared, material->stored, BRT_TRIANGLE);
	}
}

/*
 * Render model edges
 */
static void renderEdges(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
	if(on_screen == BRT_ACCEPT)
		GeometryV1ModelRenderOnScreen(v1db.format_model, v1db.renderer, model->prepared, material->stored, BRT_LINE);
	else
		GeometryV1ModelRender(v1db.format_model, v1db.renderer, model->prepared, material->stored, BRT_LINE);
}

/*
 * Render model points
 */
static void renderPoints(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
	if(on_screen == BRT_ACCEPT)
		GeometryV1ModelRenderOnScreen(v1db.format_model, v1db.renderer, model->prepared, material->stored, BRT_POINT);
	else
		GeometryV1ModelRender(v1db.format_model, v1db.renderer, model->prepared, material->stored, BRT_POINT);
}

/*
 * Render function for BR_RSTYLE_NONE
 */
static void nullRender(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
}

/*
 * A pre-preocessed static cuboid model that we can fill in with
 * appropriate vertex values to represent a bounding box
 *
 * Another way to do this is to use a cube model, and prepend a scale+xform
 * that matches it to the bounds - this has the disadvantage of making
 * the lighting do funny things unless BR_LIGHT_VIEW is used. I would prefer
 * that the user only has to do this when they themselves have created
 * a funny transform.
 */

/*
 * 8 Vertices
 */
static struct fmt_vertex bounds_vertices[] = {
	{BR_VECTOR3(-1,-1,-1),BR_VECTOR2(0,0),BR_VECTOR3(-0.666,-0.333,-0.666)},	/*    0 */
	{BR_VECTOR3(-1,-1, 1),BR_VECTOR2(0,0),BR_VECTOR3(-0.333,-0.666, 0.666)},	/*    1 */
	{BR_VECTOR3(-1, 1,-1),BR_VECTOR2(0,1),BR_VECTOR3(-0.408, 0.816,-0.408)},	/*    2 */
	{BR_VECTOR3(-1, 1, 1),BR_VECTOR2(0,1),BR_VECTOR3(-0.816, 0.408, 0.408)},	/*    3 */
	{BR_VECTOR3( 1,-1,-1),BR_VECTOR2(1,0),BR_VECTOR3( 0.408,-0.816,-0.408)},	/*    4 */
	{BR_VECTOR3( 1,-1, 1),BR_VECTOR2(1,0),BR_VECTOR3( 0.816,-0.408, 0.408)},	/*    5 */
	{BR_VECTOR3( 1, 1,-1),BR_VECTOR2(1,1),BR_VECTOR3( 0.666, 0.333,-0.666)},	/*    6 */
	{BR_VECTOR3( 1, 1, 1),BR_VECTOR2(1,1),BR_VECTOR3( 0.333, 0.666, 0.666)},	/*    7 */
};

/*
 * 12 Faces
 */
static struct v11face bounds_faces[] = {
	{{5,6,7},{ 0,13,14}, BR_VECTOR4( 1, 0, 0,1)},	/*    6 */
	{{5,4,6},{ 3, 4, 0}, BR_VECTOR4( 1, 0, 0,1)},	/*    1 */
	{{7,6,2},{13, 6, 0}, BR_VECTOR4( 0, 1, 0,1)},	/*    9 */
	{{7,2,3},{ 0, 5,16}, BR_VECTOR4( 0, 1, 0,1)},	/*   10 */
	{{1,5,7},{11,14, 0}, BR_VECTOR4( 0, 0, 1,1)},	/*    7 */
	{{1,7,3},{ 0,16,12}, BR_VECTOR4( 0, 0, 1,1)},	/*    8 */
	{{3,0,1},{ 0, 8,12}, BR_VECTOR4(-1, 0, 0,1)},	/*    5 */
	{{3,2,0},{ 5, 1, 0}, BR_VECTOR4(-1, 0, 0,1)},	/*    0 */
	{{1,0,4},{ 8, 9, 0}, BR_VECTOR4( 0,-1, 0,1)},	/*    3 */
	{{1,4,5},{ 0, 3,11}, BR_VECTOR4( 0,-1, 0,1)},	/*    4 */
	{{0,6,4},{ 0, 4, 9}, BR_VECTOR4( 0, 0,-1,1)},	/*   11 */
	{{0,2,6},{ 1, 6, 0}, BR_VECTOR4( 0, 0,-1,1)},	/*    2 */
};

static br_colour bounds_colours[12];

/*
 * 1 Group
 */
static struct v11group bounds_face_groups[] = {
	{NULL,bounds_faces,bounds_colours, NULL,0, bounds_vertices, bounds_colours, NULL, 12, 8, 18}
};

STATIC struct v11model bounds_prepared = {
	0,0,1,BR_VECTOR3(0,0,0),
	bounds_face_groups
};

STATIC struct br_model bounds_model = {
	0,
	"Bounds",
	NULL,
	NULL,
	0,0,
	BR_VECTOR3(0,0,0),
	0,
	NULL,
	NULL,
	0,
	0,
	0,
	{0},
	&bounds_prepared,
	NULL
};

/*
 * Fills in above cuboid mesh that represents the given bounding box
 */
static struct br_model * makeMeshFromBounds(br_bounds *b)
{
	int i;

	/*
	 * Fill in vertices
	 */
	BrVector3Set(&bounds_vertices[0].p,b->min.v[X],b->min.v[Y],b->min.v[Z]);
	BrVector3Set(&bounds_vertices[1].p,b->min.v[X],b->min.v[Y],b->max.v[Z]);
	BrVector3Set(&bounds_vertices[2].p,b->min.v[X],b->max.v[Y],b->min.v[Z]);
	BrVector3Set(&bounds_vertices[3].p,b->min.v[X],b->max.v[Y],b->max.v[Z]);
	BrVector3Set(&bounds_vertices[4].p,b->max.v[X],b->min.v[Y],b->min.v[Z]);
	BrVector3Set(&bounds_vertices[5].p,b->max.v[X],b->min.v[Y],b->max.v[Z]);
	BrVector3Set(&bounds_vertices[6].p,b->max.v[X],b->max.v[Y],b->min.v[Z]);
	BrVector3Set(&bounds_vertices[7].p,b->max.v[X],b->max.v[Y],b->max.v[Z]);


	/*
	 * Fill in plane equations of faces
	 */
	for(i=0; i< 3; i++) {
		bounds_faces[  i*2].eqn.v[3] = bounds_faces[1+i*2].eqn.v[3] =  b->max.v[i];
		bounds_faces[6+i*2].eqn.v[3] = bounds_faces[7+i*2].eqn.v[3] = -b->min.v[i];
	}

	/*
	 * Fill in bounds
	 */
	bounds_model.bounds = *b;
	
	return &bounds_model;
}

/*
 * Render bounding box points
 */
static void boundingBoxRenderPoints(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
	renderPoints(actor,makeMeshFromBounds(&model->bounds), material, render_data, style, on_screen);
}

/*
 * Render bounding box edges
 */
static void boundingBoxRenderEdges(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
	renderEdges(actor,makeMeshFromBounds(&model->bounds), material, render_data, style, on_screen);
}

/*
 * Render bounding box faces
 */
static void boundingBoxRenderFaces(br_actor *actor,
				  br_model *model,
				  br_material *material,
				  void *render_data,
				  br_uint_8 style,
				  int on_screen)
{
	renderFaces(actor,makeMeshFromBounds(&model->bounds),material, render_data, style,on_screen);
}

/*
 * Table of rendering functions indexed by style
 */
void (*RenderStyleCalls[])(
				br_actor *actor,
				br_model *model,
				br_material *material,
				void *render_data,
				br_uint_8 style,
				int on_screen) = {
	renderFaces,				/* BR_RSTYLE_DEFAULT			*/
	nullRender,					/* BR_RSTYLE_NONE				*/
	renderPoints,				/* BR_RSTYLE_POINTS				*/
	renderEdges,				/* BR_RSTYLE_EDGES				*/
	renderFaces,				/* BR_RSTYLE_FACES				*/
	boundingBoxRenderPoints,	/* BR_RSTYLE_BOUNDING_POINTS	*/
	boundingBoxRenderEdges,		/* BR_RSTYLE_BOUNDING_EDGES		*/
	boundingBoxRenderFaces,		/* BR_RSTYLE_BOUNDING_FACES		*/
};
