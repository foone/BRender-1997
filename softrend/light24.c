/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light24.c 2.5 1996/11/20 14:11:53 sam Exp $
 * $Locker: $
 *
 * Bits of the lighting model for coloured pixels
 */

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

#include "math_ip.h"
#include "lightmac.h"

BR_RCS_ID("$Id: light24.c 2.5 1996/11/20 14:11:53 sam Exp $");

br_scalar globalAmbientRed;
br_scalar globalAmbientGreen;
br_scalar globalAmbientBlue;

static br_scalar colourRed;
static br_scalar colourGreen;
static br_scalar colourBlue;

/*
 * Lighting function for unlit coloured 
 */
void SURFACE_CALL SurfaceColourZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	comp[C_R] = self->state.cache.comp_offsets[C_R];
	comp[C_G] = self->state.cache.comp_offsets[C_G];
	comp[C_B] = self->state.cache.comp_offsets[C_B];
}

/*
 * Lighting function for prelit coloured 
 */
void SURFACE_CALL SurfaceColourUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	comp[C_R] = BR_MUL(self->state.cache.comp_scales[C_R],
					   BR_CONST_DIV(BrIntToScalar(BR_RED(colour)),256)) +
					self->state.cache.comp_offsets[C_R];

	comp[C_G] = BR_MUL(self->state.cache.comp_scales[C_G],
					   BR_CONST_DIV(BrIntToScalar(BR_GRN(colour)),256)) +
					self->state.cache.comp_offsets[C_G];

	comp[C_B] = BR_MUL(self->state.cache.comp_scales[C_B],
					   BR_CONST_DIV(BrIntToScalar(BR_BLU(colour)),256)) +
					self->state.cache.comp_offsets[C_B];
}

/*
 * Accumulate lighting for multiple active lights by calling the
 * appropriate sub-function for each light
 *
 * Write the results into comp[C_R,G,B]
 */
void SURFACE_CALL SurfaceColourLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	int i;
	struct active_light *alp;
	br_vector3 vp;
	br_vector3 vn;
	br_vector3 fvn;

	/*
	 * Ambient component
	 */
	comp[C_R] = self->state.surface.ambient_r;
	comp[C_G] = self->state.surface.ambient_g;
	comp[C_B] = self->state.surface.ambient_b;

// set up underlying material colour

	colourRed=BrFixedToScalar(BR_RED(colour) << 8);
	colourGreen=BrFixedToScalar(BR_GRN(colour) << 8);
	colourBlue=BrFixedToScalar(BR_BLU(colour) << 8);
	/*
	 * Accumulate intensities for each active light in model space
	 */
	rend.eye_l = scache.eye_m_normalised;

	alp = scache.lights;

	for(i=0; i < scache.nlights_model; i++, alp++) 
		alp->accumulate_colour(self, p, n, colour, alp, comp);

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
			alp->accumulate_colour(self, &vp, &fvn, colour, alp, comp);
	}

	/*
	 * Scale and clamp to final range
	 */
	CLAMP_SCALE(C_R);
	CLAMP_SCALE(C_G);
	CLAMP_SCALE(C_B);
}

#define CALCULATE_DIRN_NORM_GEOM() do {					\
	dist = BrVector3Length(&dirn);					\
	if(dist <= 2 * BR_SCALAR_EPSILON)				\
		continue;									\
	s = BR_RCP(dist);								\
	BrVector3Scale(&dirn_norm,&dirn,s);				\
} while(0)

#define CALCULATE_ATTENUATION_GEOM() do {				\
	if(dist>alp->s->attenuation_q){												\
		attn=BR_SCALAR(1);														\
	}else{																		\
		if(dist>alp->s->attenuation_c){											\
			attn=BR_MUL((dist-alp->s->attenuation_c),alp->s->attenuation_l);		\
		}else{																	\
			attn=BR_SCALAR(0);													\
		}																		\
	}																			\
	attn=BR_SCALAR(1)-attn;														\
} while(0)

#define DIFFUSE_DOT_GEOM() do { \
	dot = BrVector3Dot(&rend.vertices[v].n,&dirn_norm);				\
	if(dot <= BR_SCALAR(0.0))						\
		continue;										\
} while(0)

const float epsilonX2=2 * BR_SCALAR_EPSILON;
const float unity=1;

void GEOMETRY_CALL GeometryColourLit(struct br_geometry *self, struct br_renderer *renderer)
{
	// initialise vertices to ambient values
	int i,v;
	struct active_light *alp;
	brp_vertex *tvp;
	br_scalar s,attn,dist,l,dot,r,g,b,materialRed,materialGreen,materialBlue;
	br_scalar ambientRed,ambientGreen,ambientBlue;
	br_vector3 dirn,dirn_norm;

// potentialy speedup pre-compute the following
	materialRed=BrFixedToScalar(BR_RED(scache.colour) << 8);
	materialGreen=BrFixedToScalar(BR_GRN(scache.colour) << 8);
	materialBlue=BrFixedToScalar(BR_BLU(scache.colour) << 8);
	
	for(v=0,tvp = rend.temp_vertices; v < rend.nvertices; v++, tvp++) {
		
		if(rend.vertex_counts[v] == 0)
			continue;
		
		tvp->comp[C_R] = globalAmbientRed;
		tvp->comp[C_G] = globalAmbientGreen;
		tvp->comp[C_B] = globalAmbientBlue;
	}
	
	
	alp = scache.lights;
	// accumulate active lights
	for(i=0; i < scache.nlights_model; i++, alp++){
		switch(alp->type){
		case BRT_DIRECT:
			for(v=0,tvp = rend.temp_vertices; v < rend.nvertices; v++, tvp++) {
				
				if(rend.vertex_counts[v] == 0)
					continue;
				
				dot = BrVector3Dot(&rend.vertices[v].n,&alp->direction);
				
				if(FP_TO_UINT(dot)&FP_NEG)
					continue;
				
				tvp->comp[C_R] += BR_MUL(dot, alp->s->colour_r);
				tvp->comp[C_G] += BR_MUL(dot, alp->s->colour_g);
				tvp->comp[C_B] += BR_MUL(dot, alp->s->colour_b);
			}
			break;
			
		case BRT_POINT:
			if(useLight[i]){
				for(v=0,tvp = rend.temp_vertices; v < rend.nvertices; v++, tvp++) {

					if(rend.vertex_counts[v] == 0)
						continue;
					
					BrVector3Sub(&dirn,&alp->position,&rend.vertices[v].p);
					
					//	CALCULATE_DIRN_NORM_GEOM();
					dist = BrVector3Length(&dirn);					
					if(FP_TO_UINT(dist) <= FP_TO_UINT(epsilonX2))
						continue;
					s = BR_RCP(dist);								
					BrVector3Scale(&dirn_norm,&dirn,s);				
					
					//	CALCULATE_ATTENUATION_GEOM();
					if(FP_TO_UINT(dist)>FP_TO_UINT(alp->s->attenuation_q)){											
						continue;
					}else{																	
						if(FP_TO_UINT(dist)>FP_TO_UINT(alp->s->attenuation_c)){										
							attn=BR_MUL((dist-alp->s->attenuation_c),alp->s->attenuation_l);
						}else{																
							attn=BR_SCALAR(0);												
						}																	
					}																		
					attn=BR_SCALAR(1)-attn;
				
					//	DIFFUSE_DOT_GEOM();
					dot = BrVector3Dot(&rend.vertices[v].n,&dirn_norm);
					if(FP_TO_UINT(dot)&FP_NEG)
						continue;									
					
					l = BR_MUL(dot, attn);
					
					tvp->comp[C_R] += BR_MUL(l,alp->s->colour_r);
					tvp->comp[C_G] += BR_MUL(l,alp->s->colour_g);
					tvp->comp[C_B] += BR_MUL(l,alp->s->colour_b);
				}
			}
			break;
		}
		
	}
	// clamp and scale
	for(v=0,tvp = rend.temp_vertices; v < rend.nvertices; v++, tvp++) {
		
		if(rend.vertex_counts[v] == 0)
			continue;
		CLAMP_MUL_SCALE_GEOM(C_R, materialRed);
		CLAMP_MUL_SCALE_GEOM(C_G, materialGreen);
		CLAMP_MUL_SCALE_GEOM(C_B, materialBlue);
	}
}


void SURFACE_CALL VertexColourLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	int i;
	struct active_light *alp;
	br_scalar s,attn,dist,l,dot,r,g,b,materialRed,materialGreen,materialBlue;
	br_scalar ambientRed,ambientGreen,ambientBlue;
	br_vector3 dirn,dirn_norm;

	// potentialy speedup pre-compute the following
	materialRed=BrFixedToScalar(BR_RED(scache.colour) << 8);
	materialGreen=BrFixedToScalar(BR_GRN(scache.colour) << 8);
	materialBlue=BrFixedToScalar(BR_BLU(scache.colour) << 8);
	
	comp[C_R] = globalAmbientRed;
	comp[C_G] = globalAmbientGreen;
	comp[C_B] = globalAmbientBlue;
	
	alp = scache.lights;
	// accumulate active lights
	for(i=0; i < scache.nlights_model; i++, alp++){
		switch(alp->type){
		case BRT_DIRECT:
			
			dot = BrVector3Dot(n,&alp->direction);
			
			if(FP_TO_UINT(dot)&FP_NEG)
				continue;
			
			comp[C_R] += BR_MUL(dot, alp->s->colour_r);
			comp[C_G] += BR_MUL(dot, alp->s->colour_g);
			comp[C_B] += BR_MUL(dot, alp->s->colour_b);

			break;
			
		case BRT_POINT:
			if(useLight[i]){
					
				BrVector3Sub(&dirn,&alp->position,p);
				
				//	CALCULATE_DIRN_NORM_GEOM();
				dist = BrVector3Length(&dirn);					
				if(FP_TO_UINT(dist) <= FP_TO_UINT(epsilonX2))
					continue;
				s = BR_RCP(dist);								
				BrVector3Scale(&dirn_norm,&dirn,s);				
				
				//	CALCULATE_ATTENUATION_GEOM();
				if(FP_TO_UINT(dist)>FP_TO_UINT(alp->s->attenuation_q)){											
					continue;
				}else{																	
					if(FP_TO_UINT(dist)>FP_TO_UINT(alp->s->attenuation_c)){										
						attn=BR_MUL((dist-alp->s->attenuation_c),alp->s->attenuation_l);
					}else{																
						attn=BR_SCALAR(0);												
					}																	
				}																		
				attn=BR_SCALAR(1)-attn;
			
				//	DIFFUSE_DOT_GEOM();
				dot = BrVector3Dot(n,&dirn_norm);
				if(FP_TO_UINT(dot)&FP_NEG)
					continue;									
				
				l = BR_MUL(dot, attn);
				
				comp[C_R] += BR_MUL(l,alp->s->colour_r);
				comp[C_G] += BR_MUL(l,alp->s->colour_g);
				comp[C_B] += BR_MUL(l,alp->s->colour_b);
			}
			break;
		}
		
	}
	// clamp and scale
	CLAMP_MUL_SCALE(C_R, materialRed);
	CLAMP_MUL_SCALE(C_G, materialGreen);
	CLAMP_MUL_SCALE(C_B, materialBlue);
}


void BR_ASM_CALL FaceColourLit(struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp)
{
	struct fmt_vertex *vp;
	int i;
	struct active_light *alp;
	brp_vertex *tvp = v0;
	br_scalar s,attn,dist,l,dot,r,g,b,materialRed,materialGreen,materialBlue;
	br_scalar ambientRed,ambientGreen,ambientBlue;
	br_vector3 dirn,dirn_norm;
	br_renderer *renderer = rend.renderer;

	vp = rend.vertices + fp->vertices[0];

	// potentialy speedup pre-compute the following
	materialRed=BrFixedToScalar(BR_RED(scache.colour) << 8);
	materialGreen=BrFixedToScalar(BR_GRN(scache.colour) << 8);
	materialBlue=BrFixedToScalar(BR_BLU(scache.colour) << 8);
	
	tvp->comp[C_R] = globalAmbientRed;
	tvp->comp[C_G] = globalAmbientGreen;
	tvp->comp[C_B] = globalAmbientBlue;
	
	alp = scache.lights;
	// accumulate active lights
	for(i=0; i < scache.nlights_model; i++, alp++){
		switch(alp->type){
		case BRT_DIRECT:
			
			dot = BrVector3Dot((br_vector3 *)&fp->eqn,&alp->direction);
			
			if(FP_TO_UINT(dot)&FP_NEG)
				continue;
			
			tvp->comp[C_R] += BR_MUL(dot, alp->s->colour_r);
			tvp->comp[C_G] += BR_MUL(dot, alp->s->colour_g);
			tvp->comp[C_B] += BR_MUL(dot, alp->s->colour_b);

			break;
			
		case BRT_POINT:
			if(useLight[i]){
					
				BrVector3Sub(&dirn,&alp->position,&vp->p);
				
				//	CALCULATE_DIRN_NORM_GEOM();
				dist = BrVector3Length(&dirn);					
				if(FP_TO_UINT(dist) <= FP_TO_UINT(epsilonX2))
					continue;
				s = BR_RCP(dist);								
				BrVector3Scale(&dirn_norm,&dirn,s);				
				
				//	CALCULATE_ATTENUATION_GEOM();
				if(FP_TO_UINT(dist)>FP_TO_UINT(alp->s->attenuation_q)){											
					continue;
				}else{																	
					if(FP_TO_UINT(dist)>FP_TO_UINT(alp->s->attenuation_c)){										
						attn=BR_MUL((dist-alp->s->attenuation_c),alp->s->attenuation_l);
					}else{																
						attn=BR_SCALAR(0);												
					}																	
				}																		
				attn=BR_SCALAR(1)-attn;
			
				//	DIFFUSE_DOT_GEOM();
				dot = BrVector3Dot((br_vector3 *)&fp->eqn,&dirn_norm);
				if(FP_TO_UINT(dot)&FP_NEG)
					continue;									
				
				l = BR_MUL(dot, attn);
				
				tvp->comp[C_R] += BR_MUL(l,alp->s->colour_r);
				tvp->comp[C_G] += BR_MUL(l,alp->s->colour_g);
				tvp->comp[C_B] += BR_MUL(l,alp->s->colour_b);
			}
			break;
		}
		
	}
	// clamp and scale
	CLAMP_MUL_SCALE_GEOM(C_R, materialRed);
	CLAMP_MUL_SCALE_GEOM(C_G, materialGreen);
	CLAMP_MUL_SCALE_GEOM(C_B, materialBlue);

	block->chain->render(block->chain, v0, v1, v2, fp, tfp);
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

	r = BR_MUL(l, colourRed);
	g = BR_MUL(l, colourGreen);
	b = BR_MUL(l, colourBlue);

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

	comp[C_R] += BR_MUL(r, BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[C_G] += BR_MUL(g, BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[C_B] += BR_MUL(b, BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
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

	r = BR_MUL(l, colourRed);
	g = BR_MUL(l, colourGreen);
	b = BR_MUL(l, colourBlue);

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

	comp[C_R] += BR_MUL(BR_MUL(r,alp->intensity), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[C_G] += BR_MUL(BR_MUL(g,alp->intensity), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[C_B] += BR_MUL(BR_MUL(b,alp->intensity), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
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

	r = BR_MUL(l, colourRed);
	g = BR_MUL(l, colourGreen);
	b = BR_MUL(l, colourBlue);

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

	comp[C_R] += BR_MUL(BR_MUL(r,attn), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[C_G] += BR_MUL(BR_MUL(g,attn), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[C_B] += BR_MUL(BR_MUL(b,attn), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
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

	r = BR_MUL(l, colourRed);
	g = BR_MUL(l, colourGreen);
	b = BR_MUL(l, colourBlue);

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

	comp[C_R] += BR_MUL(BR_MUL(r,attn), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[C_G] += BR_MUL(BR_MUL(g,attn), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[C_B] += BR_MUL(BR_MUL(b,attn), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
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

	r = BR_MUL(l, colourRed);
	g = BR_MUL(l, colourGreen);
	b = BR_MUL(l, colourBlue);

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

	comp[C_R] += BR_MUL(BR_MUL(r,attn), BrFixedToScalar(BR_RED(alp->s->colour) << 8));
	comp[C_G] += BR_MUL(BR_MUL(g,attn), BrFixedToScalar(BR_GRN(alp->s->colour) << 8));
	comp[C_B] += BR_MUL(BR_MUL(b,attn), BrFixedToScalar(BR_BLU(alp->s->colour) << 8));
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

