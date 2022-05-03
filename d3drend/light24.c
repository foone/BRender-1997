/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light24.c 1.1 1997/07/11 16:14:11 jon Exp JON $
 * $Locker: JON $
 *
 * Bits of the lighting model for coloured pixels
 */

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

#include "math_ip.h"
#include "lightmac.h"

BR_RCS_ID("$Id: light24.c 1.1 1997/07/11 16:14:11 jon Exp JON $");

/*
 * Lighting function for unlit coloured 
 */
void SURFACE_CALL SurfaceColourZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	d3dtlv->dcColor = D3DRGBA(0, 0, 0, 255);
	d3dtlv->dcSpecular = D3DRGB(0, 0, 0);
}

/*
 * Lighting function for prelit coloured 
 */
void SURFACE_CALL SurfaceColourUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	d3dtlv->dcColor = colour | D3DRGBA(0, 0, 0, 255);
	d3dtlv->dcSpecular = D3DRGB(0, 0, 0);
}

/*
 * Accumulate lighting for multiple active lights by calling the
 * appropriate sub-function for each light
 */
void SURFACE_CALL SurfaceColourLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	int i;
	struct active_light *alp;
	br_vector3 vp;
	br_vector3 vn;
	br_vector3 fvn;
	br_scalar diffuse[3];

	/*
	 * Ambient component
	 */
	diffuse[0] = BR_MUL(self->state.surface.ka,BrFixedToScalar(BR_RED(colour) << 8));
	diffuse[1] = BR_MUL(self->state.surface.ka,BrFixedToScalar(BR_GRN(colour) << 8));
	diffuse[2] = BR_MUL(self->state.surface.ka,BrFixedToScalar(BR_BLU(colour) << 8));

	/*
	 * Accumulate intensities for each active light in model space
	 */
	rend.eye_l = scache.eye_m_normalised;

	alp = scache.lights;

	for(i=0; i < scache.nlights_model; i++, alp++) 
		alp->accumulate_colour(self, p, n, colour, alp, diffuse);

	/*
	 * See if any lights are to be calculated in view space
	 */
	if(scache.nlights_view) {
		/*
		 * Transform point and normal into view space ...
		 */
		BrMatrix34ApplyP(&vp, p, &self->state.matrix.model_to_view);
		BrMatrix34TApplyV(&vn, n, &scache.view_to_model);
		BrVector3Normalise(&fvn, &vn);

		rend.eye_l.v[0] = BR_SCALAR(0);
		rend.eye_l.v[1] = BR_SCALAR(0);
		rend.eye_l.v[2] = BR_SCALAR(1);

		/*
		 * ... and accumulate
		 */
		for(i=0; i < scache.nlights_view; i++, alp++)
			alp->accumulate_colour(self, &vp, &fvn, colour, alp, diffuse);
	}

	/*
	 * Scale and clamp to final range
	 */
	CLAMP_SCALE(diffuse[0]);
	CLAMP_SCALE(diffuse[1]);
	CLAMP_SCALE(diffuse[2]);

	d3dtlv->dcColor = D3DRGB(diffuse[0], diffuse[1], diffuse[2]);
	d3dtlv->dcSpecular = D3DRGB(0, 0, 0);
}

/*
 * Lighting function for unlit coloured 
 */
void SURFACE_CALL SurfaceColourAlphaZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	d3dtlv->dcColor = colour & D3DRGB(0, 0, 0);
	d3dtlv->dcSpecular = D3DRGB(0, 0, 0);
}

/*
 * Lighting function for prelit coloured 
 */
void SURFACE_CALL SurfaceColourAlphaUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	d3dtlv->dcColor = colour;
	d3dtlv->dcSpecular = D3DRGB(0, 0, 0);
}

/*
 * Accumulate lighting for multiple active lights by calling the
 * appropriate sub-function for each light
 */
void SURFACE_CALL SurfaceColourAlphaLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, D3DTLVERTEX *d3dtlv)
{
	int i;
	struct active_light *alp;
	br_vector3 vp;
	br_vector3 vn;
	br_vector3 fvn;
	br_scalar diffuse[3];

	/*
	 * Ambient component
	 */
	diffuse[0] = BR_MUL(self->state.surface.ka,BrFixedToScalar(BR_RED(colour) << 8));
	diffuse[1] = BR_MUL(self->state.surface.ka,BrFixedToScalar(BR_GRN(colour) << 8));
	diffuse[2] = BR_MUL(self->state.surface.ka,BrFixedToScalar(BR_BLU(colour) << 8));

	/*
	 * Accumulate intensities for each active light in model space
	 */
	rend.eye_l = scache.eye_m_normalised;

	alp = scache.lights;

	for(i=0; i < scache.nlights_model; i++, alp++) 
		alp->accumulate_colour(self, p, n, colour, alp, diffuse);

	/*
	 * See if any lights are to be calculated in view space
	 */
	if(scache.nlights_view) {
		/*
		 * Transform point and normal into view space ...
		 */
		BrMatrix34ApplyP(&vp, p, &self->state.matrix.model_to_view);
		BrMatrix34TApplyV(&vn, n, &scache.view_to_model);
		BrVector3Normalise(&fvn, &vn);

		rend.eye_l.v[0] = BR_SCALAR(0);
		rend.eye_l.v[1] = BR_SCALAR(0);
		rend.eye_l.v[2] = BR_SCALAR(1);

		/*
		 * ... and accumulate
		 */
		for(i=0; i < scache.nlights_view; i++, alp++)
			alp->accumulate_colour(self, &vp, &fvn, colour, alp, diffuse);
	}

	/*
	 * Scale and clamp to final range
	 */
	CLAMP_SCALE(diffuse[0]);
	CLAMP_SCALE(diffuse[1]);
	CLAMP_SCALE(diffuse[2]);

	d3dtlv->dcColor = D3DRGBA(diffuse[0], diffuse[1], diffuse[2], 0) | colour & D3DRGB(0, 0, 0);
	d3dtlv->dcSpecular = D3DRGB(0, 0, 0);
}

/*
 * Lighting function that does nothing
 */
static void lightingColourNull(br_renderer *self,
	br_vector3 *p, br_vector3 *n,
	br_colour colour,
	struct active_light *alp,
	br_scalar *comp)
{
}

/*
 * Lighting for directional light source in model space
 */
static void lightingColourDirect(br_renderer *self,
	br_vector3 *p, br_vector3 *n,
	br_colour colour,
	struct active_light *alp,
	br_scalar *comp)
{
	br_scalar l,dot,r,g,b;

	dot = BrVector3Dot(n,&alp->direction);

	if(dot <= BR_SCALAR(0.0))
		return;

	l = BR_MUL(dot, self->state.surface.kd);

	r = BR_MUL(l, BrFixedToScalar(BR_RED(colour) << 8));
	g = BR_MUL(l, BrFixedToScalar(BR_GRN(colour) << 8));
	b = BR_MUL(l, BrFixedToScalar(BR_BLU(colour) << 8));

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		dot = BrVector3Dot(n,&alp->half);

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF) {
			l = SPECULAR_POWER(BR_MUL(self->state.surface.ks,alp->intensity));
			r += l;
			g += l;
			b += l;
		}
 	}

	comp[0] += BR_MUL(r, BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[1] += BR_MUL(g, BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[2] += BR_MUL(b, BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
}

/*
 * Lighting for point light source with attenuation
 * Colour
 */
static void lightingColourPoint(br_renderer *self,
	br_vector3 *p, br_vector3 *n,
	br_colour colour,
	struct active_light *alp,
	br_scalar *comp)
{
	br_scalar l,dot,r,g,b;
	br_vector3 dirn,dirn_norm;

	/*
	 * Work out vector between point and light source
	 */
	BrVector3Sub(&dirn,&alp->position,p);
	BrVector3Normalise(&dirn_norm,&dirn);

	DIFFUSE_DOT();

	l = BR_MUL(dot, self->state.surface.kd);

	r = BR_MUL(l, BrFixedToScalar(BR_RED(colour) << 8));
	g = BR_MUL(l, BrFixedToScalar(BR_GRN(colour) << 8));
	b = BR_MUL(l, BrFixedToScalar(BR_BLU(colour) << 8));

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF) {
			l = SPECULAR_POWER(self->state.surface.ks);
			r += l;
			g += l;
			b += l;
		}
	}

	comp[0] += BR_MUL(BR_MUL(r,alp->intensity), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[1] += BR_MUL(BR_MUL(g,alp->intensity), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[2] += BR_MUL(BR_MUL(b,alp->intensity), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
}

/*
 * Lighting for point light source with attenuation
 * Colour
 */
static void lightingColourPointAttn(br_renderer *self,
	br_vector3 *p, br_vector3 *n,
	br_colour colour,
	struct active_light *alp,
	br_scalar *comp)
{
	br_scalar attn,dot,l,dist,r,g,b;
	br_vector3 dirn,dirn_norm;

	BrVector3Sub(&dirn,&alp->position, p);
	CALCULATE_DIRN_NORM();
	CALCULATE_ATTENUATION();
	DIFFUSE_DOT();

	l = BR_MUL(dot, self->state.surface.kd);

	r = BR_MUL(l, BrFixedToScalar(BR_RED(colour) << 8));
	g = BR_MUL(l, BrFixedToScalar(BR_GRN(colour) << 8));
	b = BR_MUL(l, BrFixedToScalar(BR_BLU(colour) << 8));

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF) {
			l = SPECULAR_POWER(self->state.surface.ks);
			r += l;
			g += l;
			b += l;
		}
	}

	comp[0] += BR_MUL(BR_MUL(r,attn), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[1] += BR_MUL(BR_MUL(g,attn), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[2] += BR_MUL(BR_MUL(b,attn), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
}

/*
 * Lighting for spot light source
 * Colour
 */
static void lightingColourSpot(br_renderer *self,
	br_vector3 *p, br_vector3 *n,
	br_colour colour,
	struct active_light *alp,
	br_scalar *comp)
{
	br_scalar dot_spot, dot, l, attn,r,g,b;
	br_vector3 dirn,dirn_norm;

	BrVector3Sub(&dirn,&alp->position,p);
	BrVector3Normalise(&dirn_norm,&dirn);
	SPOT_DOT();
	DIFFUSE_DOT();
	attn = SPOT_FALLOFF(alp->intensity);

	l = BR_MUL(dot, self->state.surface.kd);

	r = BR_MUL(l, BrFixedToScalar(BR_RED(colour) << 8));
	g = BR_MUL(l, BrFixedToScalar(BR_GRN(colour) << 8));
	b = BR_MUL(l, BrFixedToScalar(BR_BLU(colour) << 8));

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF) {
			l = SPECULAR_POWER(self->state.surface.ks);
			r += l;
			g += l;
			b += l;
		}
	}

	comp[0] += BR_MUL(BR_MUL(r,attn), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[1] += BR_MUL(BR_MUL(g,attn), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[2] += BR_MUL(BR_MUL(b,attn), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
}

/*
 * Lighting for spot light source with attenuation
 * Colour
 */
static void lightingColourSpotAttn(br_renderer *self,
	br_vector3 *p, br_vector3 *n,
	br_colour colour,
	struct active_light *alp,
	br_scalar *comp)
{
	br_scalar dot,dot_spot,dist,attn,l,r,g,b;
	br_vector3 dirn,dirn_norm;

	BrVector3Sub(&dirn,&alp->position,p);
	CALCULATE_DIRN_NORM();
	SPOT_DOT();
	DIFFUSE_DOT();
	CALCULATE_ATTENUATION();

	/*
	 * Falloff between inner and outer cones
	 */
	attn = SPOT_FALLOFF(attn);

	l = BR_MUL(dot, self->state.surface.kd);

	r = BR_MUL(l, BrFixedToScalar(BR_RED(colour) << 8));
	g = BR_MUL(l, BrFixedToScalar(BR_GRN(colour) << 8));
	b = BR_MUL(l, BrFixedToScalar(BR_BLU(colour) << 8));

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF) {
			l = SPECULAR_POWER(self->state.surface.ks);
			r += l;
			g += l;
			b += l;
		}
	}

	comp[0] += BR_MUL(BR_MUL(r,attn), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[1] += BR_MUL(BR_MUL(g,attn), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[2] += BR_MUL(BR_MUL(b,attn), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
}

/*
 * Select a per-light component accumulation function
 */
void ActiveLightAccumulateColourSet(struct active_light *alp)
{
	switch(alp->type) {
	case BRT_DIRECT:
			alp->accumulate_colour = lightingColourDirect;
		break;

	case BRT_POINT:
		if(alp->s->attenuation_l == BR_SCALAR(0.0) &&
		   alp->s->attenuation_q == BR_SCALAR(0.0))
			alp->accumulate_colour = lightingColourPoint;
		else
			alp->accumulate_colour = lightingColourPointAttn;
		break;

	case BRT_SPOT:
		if(alp->s->attenuation_l == BR_SCALAR(0.0) &&
		   alp->s->attenuation_q == BR_SCALAR(0.0))
			alp->accumulate_colour = lightingColourSpot;
		else
			alp->accumulate_colour = lightingColourSpotAttn;
		break;
	default:
		alp->accumulate_colour = lightingColourNull;
	 }
}

