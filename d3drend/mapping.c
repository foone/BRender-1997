/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: mapping.c 1.2 1997/08/11 14:25:53 jon Exp JON $
 * $Locker: JON $
 *
 * Generation of U,V components
 */
#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

#include "math_ip.h"

BR_RCS_ID("$Id: mapping.c 1.2 1997/08/11 14:25:53 jon Exp JON $");

#define APPLY_UV(du,dv,su,sv) do { \
	du = BrScalarToFloat(BR_ADD(									\
			BR_MAC2(self->state.surface.map_transform.m[0][0],su,	\
					self->state.surface.map_transform.m[1][0],sv),	\
			self->state.surface.map_transform.m[2][0]));			\
	dv = BrScalarToFloat(BR_ADD(									\
			BR_MAC2(self->state.surface.map_transform.m[0][1],su,	\
			self->state.surface.map_transform.m[1][1],sv),			\
			self->state.surface.map_transform.m[2][1]));			\
} while (0)

/*
 * Generate U,V for environment assuming infinite eye
 */
void SURFACE_CALL SurfaceMapEnvironmentInfinite(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *normal, br_colour colour, D3DTLVERTEX *d3dtlv)
{
    br_vector3 r,wr;
	br_scalar d,cu,cv;

	/*
	 * Generate reflected vector
	 *
	 * -    - - -  -
	 * R = 2N(N.E)-E
	 */
    d = BR_CONST_MUL(BrVector3Dot(&scache.eye_m_normalised,normal),2);
	BrVector3Scale(&r,normal,d);
    BrVector3Sub(&r,&r,&scache.eye_m_normalised);

	/*
	 * If there is an environment frame, rotate vector into it
	 */
	if(self->state.matrix.view_to_environment_hint != BRT_DONT_CARE) {
		BrMatrix34ApplyV(&wr, &r, &scache.model_to_environment);
		BrVector3Normalise(&wr, &wr);
	} else
		wr = r;

	/*
	 * Convert vector to environment coordinates
	 */
	cu = BrAngleToScalar(BR_ATAN2(wr.v[0],-wr.v[2]));

#if 0
	cv = BrAngleToScalar(BR_ASIN(-wr.v[1]/2+BR_SCALAR(0.5)));
#else
	cv = -wr.v[1]/2+BR_SCALAR(0.5);
#endif

	APPLY_UV(d3dtlv->dvTU,d3dtlv->dvTV,cu,cv);
}

/*
 * Generate U,V for environment assuming local eye
 */
void SURFACE_CALL SurfaceMapEnvironmentLocal(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *normal, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	br_vector3 eye;
	br_vector3 r,wr;
	br_scalar d,cu,cv;

	/*
	 * Generate eye vector - 
	 */
	BrVector3Sub(&eye,&scache.eye_m,p);
	BrVector3Normalise(&eye, &eye);
	
	/*
	 * Generate reflected vector
	 *
	 * -    - - -  -
	 * R = 2N(N.E)-E
	 */
	d = BR_CONST_MUL(BrVector3Dot(&eye,normal),2);
	BrVector3Scale(&r,normal,d);
	BrVector3Sub(&r,&r,&eye);

	/*
	 * If there is an environment frame, rotate vector into it
	 */
	if(self->state.matrix.view_to_environment_hint != BRT_DONT_CARE) {
		BrMatrix34ApplyV(&wr, &r, &scache.model_to_environment);
		BrVector3Normalise(&wr, &wr);
	} else
		wr = r;

	/*
	 * Convert vector to environment coordinates
	 */
	cu = BrAngleToScalar(BR_ATAN2(wr.v[0],-wr.v[2]));

#if 0
	cv = BrAngleToScalar(BR_ASIN(-wr.v[1]/2+BR_SCALAR(0.5)));
#else
	cv = -wr.v[1]/2+BR_SCALAR(0.5);
#endif

	APPLY_UV(d3dtlv->dvTU,d3dtlv->dvTV,cu,cv);
}

/*
 * Take U,V from vertex y,z
 */
void SURFACE_CALL SurfaceMapGeometryX(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	APPLY_UV(d3dtlv->dvTU,d3dtlv->dvTV,p->v[1],p->v[2]);
}

void SURFACE_CALL SurfaceMapGeometryY(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	APPLY_UV(d3dtlv->dvTU,d3dtlv->dvTV,p->v[2],p->v[0]);
}

void SURFACE_CALL SurfaceMapGeometryZ(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	APPLY_UV(d3dtlv->dvTU,d3dtlv->dvTV,p->v[0],p->v[1]);
}

/*
 * Take U,V from vertex u,v
 */
void SURFACE_CALL SurfaceMapGeometryMap(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	APPLY_UV(d3dtlv->dvTU,d3dtlv->dvTV,map->v[0],map->v[1]);
}

/*
 * Scale only
 */
void SURFACE_CALL SurfaceMapGeometryMapScale(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
    d3dtlv->dvTU = BrScalarToFloat(BR_MUL(self->state.surface.map_transform.m[0][0],map->v[0]));
    d3dtlv->dvTV = BrScalarToFloat(BR_MUL(self->state.surface.map_transform.m[1][1],map->v[1]));
}

/*
 * Scale and translate only
 */
void SURFACE_CALL SurfaceMapGeometryMapScaleTranslate(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
    d3dtlv->dvTU = BrScalarToFloat(BR_ADD(BR_MUL(self->state.surface.map_transform.m[0][0],map->v[0]),self->state.surface.map_transform.m[2][0]));
    d3dtlv->dvTV = BrScalarToFloat(BR_ADD(BR_MUL(self->state.surface.map_transform.m[1][1],map->v[1]),self->state.surface.map_transform.m[2][1]));
}

void SURFACE_CALL SurfaceMapGeometryMapCopy(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	d3dtlv->dvTU = BrScalarToFloat(map->v[0]);
	d3dtlv->dvTV = BrScalarToFloat(map->v[1]);
}

#if BASED_FIXED
/*
 * Take U,V from vertex, assumeing offset == 0 and scales are a power of 2
 */
void SURFACE_CALL SurfaceMapGeometryMapShift(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	d3dtlv->dvTU = BrScalarToFloat((map->v[0] << self->state.cache.u_shift) + self->state.surface.map_transform.m[2][0]);
	d3dtlv->dvTV = BrScalarToFloat((map->v[1] << self->state.cache.v_shift) + self->state.surface.map_transform.m[2][1]);
}
#endif

void BR_ASM_CALL OpTriangleMapQuadIndexed(struct brp_block *block, WORD *index, br_boolean modified, struct v11face *fp, struct temp_face *tfp)
{
	br_vector2 *qv = (br_vector2*)&rend.renderer->state.cache.quad_transformed;

	switch (rend.face_flags[fp-rend.faces] & BR_FACEF_QUAD_MASK) {

	case BR_FACEF_QUAD_012:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[0].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[0].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[1].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[1].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[2].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[2].v[1]);
		break;

	case BR_FACEF_QUAD_123:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[1].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[1].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[2].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[2].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[3].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[3].v[1]);
		break;

	case BR_FACEF_QUAD_230:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[2].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[2].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[3].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[3].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[0].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[0].v[1]);
		break;

	case BR_FACEF_QUAD_301:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[3].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[3].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[0].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[0].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[1].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[1].v[1]);
		break;

	case BR_FACEF_QUAD_032:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[0].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[0].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[3].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[3].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[2].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[2].v[1]);
		break;

	case BR_FACEF_QUAD_103:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[1].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[1].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[0].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[0].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[3].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[3].v[1]);
		break;

	case BR_FACEF_QUAD_210:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[2].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[2].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[1].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[1].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[0].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[0].v[1]);
		break;

	case BR_FACEF_QUAD_321:

		rend.d3dtlvertices[index[0]].dvTU = BrScalarToFloat(qv[3].v[0]);
		rend.d3dtlvertices[index[0]].dvTV = BrScalarToFloat(qv[3].v[1]);
		rend.d3dtlvertices[index[1]].dvTU = BrScalarToFloat(qv[2].v[0]);
		rend.d3dtlvertices[index[1]].dvTV = BrScalarToFloat(qv[2].v[1]);
		rend.d3dtlvertices[index[2]].dvTU = BrScalarToFloat(qv[1].v[0]);
		rend.d3dtlvertices[index[2]].dvTV = BrScalarToFloat(qv[1].v[1]);
		break;
	}

	block->chain->render_indexed(block->chain, index, BR_TRUE, fp, tfp);
}

void BR_ASM_CALL OpTriangleMapQuad(struct brp_block *block, WORD *index, D3DTLVERTEX *vertex,
	struct v11face *fp, struct temp_face *tfp)
{
	br_vector2 *qv = (br_vector2*)&rend.renderer->state.cache.quad_transformed;

	switch (rend.face_flags[fp-rend.faces] & BR_FACEF_QUAD_MASK) {

	case BR_FACEF_QUAD_012:

		vertex[0].dvTU = BrScalarToFloat(qv[0].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[0].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[1].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[1].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[2].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[2].v[1]);
		break;

	case BR_FACEF_QUAD_123:

		vertex[0].dvTU = BrScalarToFloat(qv[1].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[1].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[2].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[2].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[3].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[3].v[1]);
		break;

	case BR_FACEF_QUAD_230:

		vertex[0].dvTU = BrScalarToFloat(qv[2].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[2].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[3].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[3].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[0].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[0].v[1]);
		break;

	case BR_FACEF_QUAD_301:

		vertex[0].dvTU = BrScalarToFloat(qv[3].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[3].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[0].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[0].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[1].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[1].v[1]);
		break;

	case BR_FACEF_QUAD_032:

		vertex[0].dvTU = BrScalarToFloat(qv[0].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[0].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[3].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[3].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[2].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[2].v[1]);
		break;

	case BR_FACEF_QUAD_103:

		vertex[0].dvTU = BrScalarToFloat(qv[1].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[1].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[0].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[0].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[3].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[3].v[1]);
		break;

	case BR_FACEF_QUAD_210:

		vertex[0].dvTU = BrScalarToFloat(qv[2].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[2].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[1].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[1].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[0].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[0].v[1]);
		break;

	case BR_FACEF_QUAD_321:

		vertex[0].dvTU = BrScalarToFloat(qv[3].v[0]);
		vertex[0].dvTV = BrScalarToFloat(qv[3].v[1]);
		vertex[1].dvTU = BrScalarToFloat(qv[2].v[0]);
		vertex[1].dvTV = BrScalarToFloat(qv[2].v[1]);
		vertex[2].dvTU = BrScalarToFloat(qv[1].v[0]);
		vertex[2].dvTV = BrScalarToFloat(qv[1].v[1]);
		break;
	}

	if (block->chain != NULL)
		block->chain->render(block->chain, index, vertex, fp, tfp);
}
