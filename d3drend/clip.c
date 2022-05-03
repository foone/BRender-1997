/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: clip.c 1.1 1997/07/11 16:13:57 jon Exp JON $
 * $Locker: JON $
 *
 * Mesh rendering to produce faces
 */
#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

BR_RCS_ID("$Id: clip.c 1.1 1997/07/11 16:13:57 jon Exp JON $");


/*
 * Unified structure containing combined all vertex information
 */
typedef struct clipping_vertex {

	D3DHVERTEX hv;
	D3DVALUE dvHW;
	long index;
	D3DVALUE pad[2];
	D3DTLVERTEX tlv;

} clipping_vertex;


/*
 * Macro to interpolate a vertex a certain fraction of the way between two vertices
 */
#define CLIP_FLOAT(a, b, t) ((a) + (t) * ((b) - (a)))

#if 0	// To do this, t must be rounded to a 0.8 fraction
#define CLIP_COLOUR(a, b, t)\
	((DWORD)(((a) & 0x00ff0ff) * t + ((b) & 0x00ff00ff) * (1 - t)) & 0x00ff00ff +\
	(DWORD)((a) & 0xff00ff00) * t + ((b) & 0xff00ff00) * (1 - t)) & 0xff00ff00)
#else
#define CLIP_COLOUR(a, b, t) D3DRGBA(\
	(DWORD)CLIP_FLOAT((float)RGBA_GETRED(a), (float)RGBA_GETRED(b), t),\
	(DWORD)CLIP_FLOAT((float)RGBA_GETGREEN(a), (float)RGBA_GETGREEN(b), t),\
	(DWORD)CLIP_FLOAT((float)RGBA_GETBLUE(a), (float)RGBA_GETBLUE(b), t),\
	(DWORD)CLIP_FLOAT((float)RGBA_GETALPHA(a), (float)RGBA_GETALPHA(b), t))
#endif

#define CLIP_EDGE(wp, vp, up, tv, tu, clip_x, clip_y, clip_z, hw, flags)\
{\
	(t) = (tv) / ((tv) - (tu));\
\
	(wp)->hv.dwFlags = TV_CLIPPED;\
	(wp)->dvHW = CLIP_FLOAT((vp)->dvHW, (up)->dvHW, (t));\
\
	(wp)->hv.dvHX = (clip_x)? (hw): CLIP_FLOAT((vp)->hv.dvHX, (up)->hv.dvHX, (t));\
	(wp)->hv.dvHY = (clip_y)? (hw): CLIP_FLOAT((vp)->hv.dvHY, (up)->hv.dvHY, (t));\
	(wp)->hv.dvHZ = (clip_z)? (hw): CLIP_FLOAT((vp)->hv.dvHZ, (up)->hv.dvHZ, (t));\
\
	(wp)->index = -1;\
\
	if ((flags) & CLIP_COLOR)\
		(wp)->tlv.dcColor = CLIP_COLOUR((vp)->tlv.dcColor, (up)->tlv.dcColor, (t));\
\
	if ((flags) & CLIP_SPECULAR)\
		(wp)->tlv.dcSpecular = CLIP_COLOUR((vp)->tlv.dcSpecular, (up)->tlv.dcSpecular, (t));\
\
	if ((flags) & CLIP_MAPPING) {\
		(wp)->tlv.dvTU = CLIP_FLOAT((vp)->tlv.dvTU, (up)->tlv.dvTU, (t));\
		(wp)->tlv.dvTV = CLIP_FLOAT((vp)->tlv.dvTV, (up)->tlv.dvTV, (t));\
	}\
}


/*
 * Core of 3D homogenous clipper - loosly based on Paul Heckbert's
 * implemetationin Graphics Gems I.
 *
 * Clip a polygon to an arbitary plane eqn.
 */
STATIC int ClipFaceToPlane(
		clipping_vertex *vp,
		clipping_vertex *verts_out,
		int num_in,
		br_vector4 *plane,
		int flags)
{
	clipping_vertex *wp = verts_out;
	int num_out = 0;

	clipping_vertex *up;
	float t,tu,tv;

	/*
	 * Go round face, an edge at a time
	 */
	up = vp+num_in-1;

	tu = -(BrScalarToFloat(plane->v[0]) * up->hv.dvHX +
	       BrScalarToFloat(plane->v[1]) * up->hv.dvHY +
	       BrScalarToFloat(plane->v[2]) * up->hv.dvHZ +
	       BrScalarToFloat(plane->v[3]) * up->dvHW);

	for( ; num_in-- ; up = vp, tu = tv, vp++) {

		tv = -(BrScalarToFloat(plane->v[0]) * vp->hv.dvHX +
			   BrScalarToFloat(plane->v[1]) * vp->hv.dvHY +
			   BrScalarToFloat(plane->v[2]) * vp->hv.dvHZ +
			   BrScalarToFloat(plane->v[3]) * vp->dvHW);

		if (tv <= S0) {

			/*
			 * This vertex is inside clip space
			 */
			if (tu <= S0) {

				/*
				 * last vertex was as well - add this vertex
				 */
				*wp++ = *vp;
				num_out++;
				continue;
			}

			if (tv < S0) {

				/*
				 * Edge crosses out to in, add intersection	and this vertex
				 */
				CLIP_EDGE(wp, vp, up, tv, tu, BR_FALSE, BR_FALSE, BR_FALSE, 0, flags);
				wp++;
				num_out++;
			}

			/*
			 * Copy next vertex
			 */
			*wp++ = *vp;
			num_out++;

		} else {

			/*
			 * This vertex is outside clip space
			 */
			if (tu >= S0)

				/*
				 * last vertex was as well - don't do anything
				 */
				continue;

			/*
			 * Edge crosses in to out, add intersection 
			 */
			CLIP_EDGE(wp, up, vp, tu, tv, BR_FALSE, BR_FALSE, BR_FALSE, 0, flags);
			wp++;
			num_out++;
		}
	}

	return num_out;
}

/*
 * Special case of sign = 1, k = 1.0
 */
STATIC int ClipFaceToPlus1(
		clipping_vertex *vp,
		clipping_vertex *verts_out,
		int num_in,
		int axis,
		int flags)
{
	clipping_vertex *wp = verts_out;
	int num_out = 0;

	clipping_vertex *up;
	float t,tu,tv;

	/*
	 * Go round face, an edge at a time
	 */
	up = vp+num_in-1;

	tu = (&up->hv.dvHX)[axis] - up->dvHW;

	for( ; num_in-- ; up = vp, tu = tv, vp++) {

		tv = (&vp->hv.dvHX)[axis] - vp->dvHW;

		if (tv < S0) {

			/*
			 * This vertex is inside clip space
			 */
			if (tu <= S0) {

				/*
				 * last vertex was as well - add this vertex
				 */
				*wp++ = *vp;
				num_out++;
			
			} else {

				/*
				 * Edge crosses out to in, add intersection	and this vertex
				 */
#if 1
	t = tv / (tv-tu);

	wp->hv.dwFlags = TV_CLIPPED;
	wp->dvHW = vp->dvHW + t * (up->dvHW - vp->dvHW);

	wp->hv.dvHX = (axis == 0)? wp->dvHW: (vp->hv.dvHX + t * (up->hv.dvHX - vp->hv.dvHX));
	wp->hv.dvHY = (axis == 1)? wp->dvHW: (vp->hv.dvHY + t * (up->hv.dvHY - vp->hv.dvHY));
	wp->hv.dvHZ = (axis == 2)? wp->dvHW: (vp->hv.dvHZ + t * (up->hv.dvHZ - vp->hv.dvHZ));

//	if (flags & CLIP_COLOR)
		wp->tlv.dcColor = D3DRGBA(
			RGBA_GETRED(vp->tlv.dcColor) + t * (RGBA_GETRED(up->tlv.dcColor) - RGBA_GETRED(vp->tlv.dcColor)),
			RGBA_GETGREEN(vp->tlv.dcColor) + t * (RGBA_GETGREEN(up->tlv.dcColor) - RGBA_GETGREEN(vp->tlv.dcColor)),
			RGBA_GETBLUE(vp->tlv.dcColor) + t * (RGBA_GETBLUE(up->tlv.dcColor) - RGBA_GETBLUE(vp->tlv.dcColor)),
			RGBA_GETALPHA(vp->tlv.dcColor) + t * (RGBA_GETALPHA(up->tlv.dcColor) - RGBA_GETALPHA(vp->tlv.dcColor)));

//	if (flags & CLIP_MAPPING) {
		wp->tlv.dvTU = vp->tlv.dvTU + t * (up->tlv.dvTU - vp->tlv.dvTU);
		wp->tlv.dvTV = vp->tlv.dvTV + t * (up->tlv.dvTV - vp->tlv.dvTV);
//	}
#else
				CLIP_EDGE(wp, vp, up, tv, tu, axis == 0, axis == 1, axis == 2, wp->dvHW, flags);
#endif
				wp++;

				*wp++ = *vp;

				num_out += 2;
			}

		} else if (tv == S0) {

			/*
			 * This vertex is on the edge of clip space, add it
			 */
			*wp++ = *vp;
			num_out++;

		} else {

			/*
			 * This vertex is outside clip space
			 */
			if (tu < S0) {

				/*
				 * Edge crosses in to out, add intersection 
				 */
#if 1
	t = tu / (tu-tv);

	wp->hv.dwFlags = TV_CLIPPED;
	wp->dvHW = up->dvHW + t * (vp->dvHW - up->dvHW);

	wp->hv.dvHX = (axis == 0)? wp->dvHW: (up->hv.dvHX + t * (vp->hv.dvHX - up->hv.dvHX));
	wp->hv.dvHY = (axis == 1)? wp->dvHW: (up->hv.dvHY + t * (vp->hv.dvHY - up->hv.dvHY));
	wp->hv.dvHZ = (axis == 2)? wp->dvHW: (up->hv.dvHZ + t * (vp->hv.dvHZ - up->hv.dvHZ));

//	if (flags & CLIP_COLOR)
		wp->tlv.dcColor = D3DRGBA(
			RGBA_GETRED(up->tlv.dcColor) + t * (RGBA_GETRED(vp->tlv.dcColor) - RGBA_GETRED(up->tlv.dcColor)),
			RGBA_GETGREEN(up->tlv.dcColor) + t * (RGBA_GETGREEN(vp->tlv.dcColor) - RGBA_GETGREEN(up->tlv.dcColor)),
			RGBA_GETBLUE(up->tlv.dcColor) + t * (RGBA_GETBLUE(vp->tlv.dcColor) - RGBA_GETBLUE(up->tlv.dcColor)),
			RGBA_GETALPHA(up->tlv.dcColor) + t * (RGBA_GETALPHA(vp->tlv.dcColor) - RGBA_GETALPHA(up->tlv.dcColor)));

//	if (flags & CLIP_MAPPING) {
		wp->tlv.dvTU = up->tlv.dvTU + t * (vp->tlv.dvTU - up->tlv.dvTU);
		wp->tlv.dvTV = up->tlv.dvTV + t * (vp->tlv.dvTV - up->tlv.dvTV);
//	}
#else
				CLIP_EDGE(wp, up, vp, tu, tv, axis == 0, axis == 1, axis == 2, wp->dvHW, flags);
#endif
				wp++;
				num_out++;
			}
		}
	}

	return num_out;
}

/*
 * Special case of sign = -1, k = 1.0
 */
STATIC int ClipFaceToMinus1(
		clipping_vertex *vp,
		clipping_vertex *verts_out,
		int num_in,
		int axis,
		int flags)
{
	clipping_vertex *wp = verts_out;
	int num_out = 0;

	clipping_vertex *up;
	float t,tu,tv;

	/*
	 * Go round face, an edge at a time
	 */
	up = vp+num_in-1;

	tu = -(&up->hv.dvHX)[axis] - up->dvHW;

	for( ; num_in-- ; up = vp, tu = tv, vp++) {

		tv = -(&vp->hv.dvHX)[axis] - vp->dvHW;

		if (tv < S0) {

			/*
			 * This vertex is inside clip space
			 */
			if (tu <= S0) {

				/*
				 * last vertex was as well - add this vertex, moving it to within the clip region
				 */
				*wp++ = *vp;
				(&vp->hv.dvHX)[axis] += 2 * FLT_EPSILON;
				num_out++;

			} else {

				/*
				 * Edge crosses out to in, add intersection	and this vertex
				 */
#if 1
	t = tv / (tv-tu);

	wp->hv.dwFlags = TV_CLIPPED;
	wp->dvHW = vp->dvHW + t * (up->dvHW - vp->dvHW);

	wp->hv.dvHX = (axis == 0)? -wp->dvHW + 2 * FLT_EPSILON: (vp->hv.dvHX + t * (up->hv.dvHX - vp->hv.dvHX));
	wp->hv.dvHY = (axis == 1)? -wp->dvHW + 2 * FLT_EPSILON: (vp->hv.dvHY + t * (up->hv.dvHY - vp->hv.dvHY));
	wp->hv.dvHZ = (axis == 2)? -wp->dvHW + 2 * FLT_EPSILON: (vp->hv.dvHZ + t * (up->hv.dvHZ - vp->hv.dvHZ));

//	if (flags & CLIP_COLOR)
		wp->tlv.dcColor = D3DRGBA(
			RGBA_GETRED(vp->tlv.dcColor) + t * (RGBA_GETRED(up->tlv.dcColor) - RGBA_GETRED(vp->tlv.dcColor)),
			RGBA_GETGREEN(vp->tlv.dcColor) + t * (RGBA_GETGREEN(up->tlv.dcColor) - RGBA_GETGREEN(vp->tlv.dcColor)),
			RGBA_GETBLUE(vp->tlv.dcColor) + t * (RGBA_GETBLUE(up->tlv.dcColor) - RGBA_GETBLUE(vp->tlv.dcColor)),
			RGBA_GETALPHA(vp->tlv.dcColor) + t * (RGBA_GETALPHA(up->tlv.dcColor) - RGBA_GETALPHA(vp->tlv.dcColor)));

//	if (flags & CLIP_MAPPING) {
		wp->tlv.dvTU = vp->tlv.dvTU + t * (up->tlv.dvTU - vp->tlv.dvTU);
		wp->tlv.dvTV = vp->tlv.dvTV + t * (up->tlv.dvTV - vp->tlv.dvTV);
//	}
#else
				CLIP_EDGE(wp, vp, up, tv, tu, axis == 0, axis == 1, axis == 2, -wp->dvHW + 2 * FLT_EPSILON, flags);
#endif
				wp++;

				*wp++ = *vp;
				(&vp->hv.dvHX)[axis] += 2 * FLT_EPSILON;

				num_out += 2;
			}

		} else if (tv == S0) {

			/*
			 * This vertex is on the edge of clip space, add it
			 */
			*wp++ = *vp;
			num_out++;

		} else {

			/*
			 * This vertex is outside clip space
			 */
			if (tu < S0) {

				/*
				 * Edge crosses in to out, add intersection 
				 */
#if 1
	t = tu / (tu-tv);

	wp->hv.dwFlags = TV_CLIPPED;
	wp->dvHW = up->dvHW + t * (vp->dvHW - up->dvHW);

	wp->hv.dvHX = (axis == 0)? -wp->dvHW + 2 * FLT_EPSILON: (up->hv.dvHX + t * (vp->hv.dvHX - up->hv.dvHX));
	wp->hv.dvHY = (axis == 1)? -wp->dvHW + 2 * FLT_EPSILON: (up->hv.dvHY + t * (vp->hv.dvHY - up->hv.dvHY));
	wp->hv.dvHZ = (axis == 2)? -wp->dvHW + 2 * FLT_EPSILON: (up->hv.dvHZ + t * (vp->hv.dvHZ - up->hv.dvHZ));

//	if (flags & CLIP_COLOR)
		wp->tlv.dcColor = D3DRGBA(
			RGBA_GETRED(up->tlv.dcColor) + t * (RGBA_GETRED(vp->tlv.dcColor) - RGBA_GETRED(up->tlv.dcColor)),
			RGBA_GETGREEN(up->tlv.dcColor) + t * (RGBA_GETGREEN(vp->tlv.dcColor) - RGBA_GETGREEN(up->tlv.dcColor)),
			RGBA_GETBLUE(up->tlv.dcColor) + t * (RGBA_GETBLUE(vp->tlv.dcColor) - RGBA_GETBLUE(up->tlv.dcColor)),
			RGBA_GETALPHA(up->tlv.dcColor) + t * (RGBA_GETALPHA(vp->tlv.dcColor) - RGBA_GETALPHA(up->tlv.dcColor)));

//	if (flags & CLIP_MAPPING) {
		wp->tlv.dvTU = up->tlv.dvTU + t * (vp->tlv.dvTU - up->tlv.dvTU);
		wp->tlv.dvTV = up->tlv.dvTV + t * (vp->tlv.dvTV - up->tlv.dvTV);
//	}
#else
				CLIP_EDGE(wp, up, vp, tu, tv, axis == 0, axis == 1, axis == 2, -wp->dvHW + 2 * FLT_EPSILON, flags);
#endif
				wp++;
				num_out++;
			}
		}
	}

	return num_out;
}


#define CLIP_TOGGLE 			\
	if (toggle = !toggle) {		\
		cp_in = clip_poly_2;	\
		cp_out = clip_poly_1;	\
	} else {					\
		cp_in = clip_poly_1;	\
		cp_out = clip_poly_2;	\
	}							\


/*
 * Clip a face to the view volume
 */
int FaceClip(br_renderer *self, WORD **index_out, D3DTLVERTEX **clip_out, WORD *index_in, D3DTLVERTEX *clip_in, int n, br_uint_32 mask, br_uint_32 codes)
{
	static clipping_vertex clip_poly_1[16];
	static clipping_vertex clip_poly_2[16];
	static D3DTLVERTEX vertex[16];
	static WORD index[16];
	clipping_vertex *cp_in = clip_poly_1, *cp_out = clip_poly_2;
	int c, i;
	br_boolean toggle = BR_FALSE;
#if 1
	br_boolean found_force_back = BR_FALSE;
#endif
#if 1
	int flags = 0;
	if (mask & (CM_R | CM_G | CM_B)) flags |= CLIP_COLOR;
	if (mask & (CM_U | CM_V)) flags |= CLIP_MAPPING;
#endif

	/*
	 * Construct an initial array of clipping vertices
	 */
	for (i = 0; i < n; i++) {

		cp_in[i].hv = rend.d3dhvertices[index_in[i]];
		cp_in[i].dvHW = rend.d3dhw[index_in[i]];
		cp_in[i].index = index_in[i];
		cp_in[i].tlv = clip_in[i];
	}

	/*
	 * Clip against each plane - if necessary
	 * After each plane, swap polygon buffers, and quit if
	 * polygon is completely clipped away
	 */
	if (codes & OUTCODE_HITHER) {
		n = ClipFaceToPlus1(cp_in, cp_out, n, 2, flags);
		if (n < 3) return 0;
		CLIP_TOGGLE;
	}

	if (codes & OUTCODE_YON) {
		n = ClipFaceToMinus1(cp_in, cp_out, n, 2, flags);
		if (n < 3) return 0;
		CLIP_TOGGLE;
	}

	if (codes & OUTCODE_RIGHT) {
		n = ClipFaceToPlus1(cp_in, cp_out, n, 0, flags);
		if (n < 3) return 0;
		CLIP_TOGGLE;
	}

	if (codes & OUTCODE_LEFT) {
		n = ClipFaceToMinus1(cp_in, cp_out, n, 0, flags);
		if (n < 3) return 0;
		CLIP_TOGGLE;
	}

	if (codes & OUTCODE_TOP) {
		n = ClipFaceToPlus1(cp_in, cp_out, n, 1, flags);
		if (n < 3) return 0;
		CLIP_TOGGLE;
	}

	if (codes & OUTCODE_BOTTOM) {
		n = ClipFaceToMinus1(cp_in, cp_out, n, 1, flags);
		if (n < 3) return 0;
		CLIP_TOGGLE;
	}

	/*
	 * User-defined clip plane
	 */
	if (scache.user_clip_active) {
			for (c = 0; c < MAX_STATE_CLIP_PLANES; c++) {

			if (self->state.clip[c].type != BRT_PLANE)
				continue;

		 	if (!(codes & (OUTCODE_USER << c)))
				continue;

			n = ClipFaceToPlane(cp_in, cp_out, n, &self->state.clip[c].plane, flags);
			if (n < 3) return 0;
			CLIP_TOGGLE;
		}
	}

	/*
	 * Copy the final vertex list into the destination array, projecting all the new vertices
	 */
	for (i = 0; i < n; i++) {

		vertex[i] = cp_in[i].tlv;
		index[i] = (WORD)cp_in[i].index;

		if (cp_in[i].hv.dwFlags & (TV_CLIPPED | OUTCODES_ALL)) {

			vertex[i].dvRHW = 1.0f / cp_in[i].dvHW;
			PROJECT_VERTEX(&cp_in[i].hv, &vertex[i]);
			UPDATE_BOUNDS(&vertex[i]);
		}
	}

	*clip_out = vertex;
	*index_out = index;

	return n;
}


/*
 * Render a clipped face using the current block
 */
void ClippedRenderTriangles(struct br_renderer *renderer, brp_block *block, WORD *index, D3DTLVERTEX *vertex, int n,
	struct v11face *fp, struct temp_face *tfp)
{
	int i;
	WORD triangle_index[3];
	D3DTLVERTEX triangle_vertex[3];

	/*
	 * Triangulate polygon
	 */
	triangle_vertex[0] = vertex[0];
	triangle_index[0] = index[0];

	for (i = 2; i < n; i++) {

		triangle_vertex[1] = vertex[i-1];
		triangle_vertex[2] = vertex[i];
		triangle_index[1] = index[i-1];
		triangle_index[2] = index[i];

		block->render(block, triangle_index, triangle_vertex, fp, tfp);
	}
}

/*
 * Clip a line to an arbitary plane eqn. Return true if any part
 * of the line remains
 */
br_boolean ClipLineToPlane(
		union brp_vertex *in,
		union brp_vertex *out,
		br_vector4 *plane,
		int cmask)
{
	br_scalar t,tu,tv;
	int m;
	br_scalar *usp,*vsp,*wsp;

	tu =-BR_MAC4(
			plane->v[0],in[0].comp[C_X],
			plane->v[1],in[0].comp[C_Y],
			plane->v[2],in[0].comp[C_Z],
			plane->v[3],in[0].comp[C_W]);

	tv =-BR_MAC4(
			plane->v[0],in[1].comp[C_X],
			plane->v[1],in[1].comp[C_Y],
			plane->v[2],in[1].comp[C_Z],
			plane->v[3],in[1].comp[C_W]);

	out[0] = in[0];
	out[1] = in[1];

	if(tu <= S0) {
		/*
		 * First vertex is inside clip space
		 */

		if(tv <= S0) {
			/*
			 * last vertex was as well - return whole line
			 */
			out[1] = in[1];
			return BR_TRUE;
		}

		/*
		 * Line crosses in to out, truncate to intersection
		 */
		t = BR_DIVR(tu,(tu-tv));

		usp = in[0].comp;
		vsp = in[1].comp;
		wsp = out[1].comp;

		for(m = cmask ; m ; m >>=1, usp++,vsp++,wsp++)
			if(m & 1)
				*wsp = *usp + BR_MUL(t,(*vsp-*usp));
	} else {
		/*
		 * First vertex is outside clip space
		 */
		if(tv > S0)
			/*
			 * last vertex was as well - return false
			 */
			return BR_FALSE;

		/*
		 * Line crosses out to in, truncate to intersection
		 */

		t = BR_DIVR(tv,(tv-tu));

		usp = in[0].comp;
		vsp = in[1].comp;
		wsp = out[0].comp;

		for(m = cmask ; m ; m >>=1, usp++,vsp++,wsp++)
			if(m & 1)
				*wsp = *vsp + BR_MUL(t,(*usp-*vsp));
	}

	return BR_TRUE;
}

br_boolean ClipLine(br_renderer *self, union brp_vertex *out, union brp_vertex *v0, union brp_vertex *v1, br_uint_32 mask, br_uint_32 codes)
{
	static br_vector4 plane_px = BR_VECTOR4(-1, 0, 0,1);
	static br_vector4 plane_nx = BR_VECTOR4( 1, 0, 0,1);
	static br_vector4 plane_py = BR_VECTOR4( 0,-1, 0,1);
	static br_vector4 plane_ny = BR_VECTOR4( 0, 1, 0,1);
	static br_vector4 plane_pz = BR_VECTOR4( 0, 0,-1,1);
	static br_vector4 plane_nz = BR_VECTOR4( 0, 0, 1,1);

	union brp_vertex cv0[2],cv1[2];
	union brp_vertex  *cp_in = cv0,*cp_out = cv1,*cp_tmp;
	int c;

	cp_in[0] = *v0;
	cp_in[1] = *v1;

	if(codes & OUTCODE_LEFT) {
		if(!ClipLineToPlane(cp_in,cp_out,&plane_nx,mask))
			return BR_FALSE;

		cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
	}

	if(codes & OUTCODE_RIGHT) {
		if(!ClipLineToPlane(cp_in,cp_out,&plane_px,mask))
			return BR_FALSE;

		cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
	}

	if(codes & OUTCODE_TOP) {
		if(!ClipLineToPlane(cp_in,cp_out,&plane_py,mask))
			return BR_FALSE;

		cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
	}

	if(codes & OUTCODE_BOTTOM) {
		if(!ClipLineToPlane(cp_in,cp_out,&plane_ny,mask))
			return BR_FALSE;

		cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
	}

	if(codes & OUTCODE_HITHER) {
		if(!ClipLineToPlane(cp_in,cp_out,&plane_pz,mask))
			return BR_FALSE;

		cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
	}

	if(codes & OUTCODE_YON) {
		if(!ClipLineToPlane(cp_in,cp_out,&plane_nz,mask))
			return BR_FALSE;

		cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
	}

	/*
	 * User-defined clip plane
	 */

	if(scache.user_clip_active) {
			for(c = 0; c < MAX_STATE_CLIP_PLANES; c++) {

			if(self->state.clip[c].type != BRT_PLANE)
				continue;

		 	if(!(codes & (OUTCODE_USER << c)))
				continue;

			if(ClipLineToPlane(cp_in, cp_out, &self->state.clip[c].plane,mask) == 0)
				return BR_FALSE;

			cp_tmp = cp_in; cp_in = cp_out; cp_out = cp_tmp;
		}
	}

	out[0] = cp_in[0];
	out[1] = cp_in[1];

	return BR_TRUE;
}
void ClippedRenderLine(struct br_renderer *renderer, brp_block *block, union brp_vertex *cp_in)
{
	int i;
	union brp_vertex *tvp;

	/*
	 * Re-project all the vertices
	 */
#if 0
	for(i=0, tvp = cp_in; i < 2; i++, tvp++) {
		OLD_PROJECT_VERTEX(tvp,tvp->comp[C_X],tvp->comp[C_Y],tvp->comp[C_Z],tvp->comp[C_W]);
		OLD_UPDATE_BOUNDS(tvp);
	}
#endif

	/*
	 * Render the line
	 */
	block->render(block,&cp_in[0],&cp_in[1]);
}

