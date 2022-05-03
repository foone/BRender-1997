/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light8.c 2.5 1996/11/11 16:07:54 sam Exp $
 * $Locker: $
 *
 * Bits of the lighting model for indexed pixels
 */

#include "drv.h"
#include "shortcut.h"
#include "brassert.h"

#include "math_ip.h"
#include "lightmac.h"

br_scalar globalAmbient;

BR_RCS_ID("$Id: light8.c 2.5 1996/11/11 16:07:54 sam Exp $");

/*
 * Lighting function for unlit indexed 
 */
void SURFACE_CALL SurfaceIndexZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	comp[C_I] = self->state.cache.comp_offsets[C_I];
}

/*
 * Lighting function for prelit indexed 
 */
void SURFACE_CALL SurfaceIndexUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	comp[C_I] = BR_MUL(self->state.cache.comp_scales[C_I],
					   BR_CONST_DIV(BrIntToScalar(BR_ALPHA(colour)),256)) +
					self->state.cache.comp_offsets[C_I];
}

/*
 * Accumulate lighting for multiple active lights by calling the
 * appropriate sub-function for each light
 *
 * Write the results into comp[C_I]
 */
void SURFACE_CALL SurfaceIndexLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	int i;
	struct active_light *alp = scache.lights;
	br_vector3 vp;
	br_vector3 vn;
	br_vector3 fvn;

	/*
	 * Handle special case lighting of 1 model-space directional light
	 */
	if(scache.light_1md) {
		br_scalar l,dot;

		dot = BrVector3Dot(n,&alp->direction);
	
		if(dot <= BR_SCALAR(0.0)) {
			comp[C_I] = self->state.surface.ka;
			CLAMP_SCALE(C_I);
			return;
		}

		l = BR_MUL(dot, self->state.surface.kd);

		if (self->state.surface.ks != BR_SCALAR(0.0)) {
			/*
			 * Specular
			 */
			dot = BrVector3Dot(n,&alp->half);

			/*
			 * Phong lighting approximation from Gems IV pg. 385
			 */
			if(dot > SPECULARPOW_CUTOFF)
				l += SPECULAR_POWER(BR_MUL(self->state.surface.ks,alp->intensity));
	 	}

		comp[C_I] = l + self->state.surface.ka;

		/*
		 * Scale final intensity to range of indices
		 */
		CLAMP_SCALE(C_I);
		return;
	}

	/*
	 * Ambient component
	 */
	comp[C_I] = self->state.surface.ka;

	/*
	 * Accumulate intensities for each active light in model space
	 */
	rend.eye_l = scache.eye_m_normalised;

	for(i=0; i < scache.nlights_model; i++, alp++) 
		alp->accumulate_index(self, p, n, alp, comp);

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
			alp->accumulate_index(self, &vp, &fvn, alp, comp);
	}

	/*
	 * Scale final intensity to range of indices
	 */
	CLAMP_SCALE(C_I);
}

void GEOMETRY_CALL GeometryIndexLit(struct br_geometry *self, struct br_renderer *renderer)
{
	// initialise vertices to ambient values
	int i, v;
	struct active_light *alp;
	brp_vertex *tvp;
	br_scalar s,attn,dist,l,dot,r,g,b,materialRed,materialGreen,materialBlue;
	br_vector3 dirn,dirn_norm;
	
	for(v=0,tvp = rend.temp_vertices; v < rend.nvertices; v++, tvp++) {
		
		if(rend.vertex_counts[v] == 0)
			continue;
		
		tvp->comp[C_I] = globalAmbient;
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
				
//				if(dot <= BR_SCALAR(0.0))
				if(FP_TO_UINT(dot)&FP_NEG)
					continue;
				
				//				l=BR_MUL(dot, renderer->state.surface.kd);
				
				tvp->comp[C_I] += dot;
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
//					if(dist <= 2 * BR_SCALAR_EPSILON)
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
//					if(dot <= BR_SCALAR(0.0))						
					if(FP_TO_UINT(dot)&FP_NEG)
						continue;									
					
					l = BR_MUL(dot, attn);
					
					tvp->comp[C_I] += l;
				}
			}
			break;
		}
		
	}
	// clamp and scale
	for(v=0,tvp = rend.temp_vertices; v < rend.nvertices; v++, tvp++) {
		
		if(rend.vertex_counts[v] == 0)
			continue;
		CLAMP_SCALE_GEOM(C_I);
	}
}

void SURFACE_CALL VertexIndexLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	int i;
	struct active_light *alp;
	br_scalar s,attn,dist,l,dot,r,g,b,materialRed,materialGreen,materialBlue;
	br_vector3 dirn,dirn_norm;
	
	comp[C_I] = globalAmbient;
	
	alp = scache.lights;
	// accumulate active lights
	for(i=0; i < scache.nlights_model; i++, alp++){
		switch(alp->type){
		case BRT_DIRECT:
				
			dot = BrVector3Dot(n,&alp->direction);
			
//			if(dot <= BR_SCALAR(0.0))
			if(FP_TO_UINT(dot)&FP_NEG)
				continue;
			
//			l=BR_MUL(dot, renderer->state.surface.kd);
			
			comp[C_I] += dot;
			break;
		
		case BRT_POINT:
			if(useLight[i]){
				BrVector3Sub(&dirn,&alp->position,p);
				
				//	CALCULATE_DIRN_NORM_GEOM();
				dist = BrVector3Length(&dirn);					
//				if(dist <= 2 * BR_SCALAR_EPSILON)
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
//				if(dot <= BR_SCALAR(0.0))						
				if(FP_TO_UINT(dot)&FP_NEG)
					continue;									
				
				l = BR_MUL(dot, attn);
				
				comp[C_I] += l;
			}
			break;
		}
		
	}
	// clamp and scale
	CLAMP_SCALE(C_I);
}

void BR_ASM_CALL FaceIndexLit(struct brp_block *block, brp_vertex *v0, brp_vertex *v1, brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp)
{
	struct fmt_vertex *vp;
	int i;
	struct active_light *alp;
	brp_vertex *tvp = v0;
	br_scalar s,attn,dist,l,dot,r,g,b,materialRed,materialGreen,materialBlue;
	br_vector3 dirn,dirn_norm;
	br_renderer *renderer = rend.renderer;
	
	vp = rend.vertices + fp->vertices[0];

	tvp->comp[C_I] = globalAmbient;
	
	alp = scache.lights;
	// accumulate active lights
	for(i=0; i < scache.nlights_model; i++, alp++){
		switch(alp->type){
		case BRT_DIRECT:
				
			dot = BrVector3Dot((br_vector3 *)&fp->eqn,&alp->direction);
			
//			if(dot <= BR_SCALAR(0.0))
			if(FP_TO_UINT(dot)&FP_NEG)
				continue;
			
//			l=BR_MUL(dot, renderer->state.surface.kd);
			
			tvp->comp[C_I] += dot;
			break;
		
		case BRT_POINT:
			if(useLight[i]){
				BrVector3Sub(&dirn,&alp->position,&vp->p);
				
				//	CALCULATE_DIRN_NORM_GEOM();
				dist = BrVector3Length(&dirn);					
//				if(dist <= 2 * BR_SCALAR_EPSILON)
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
//				if(dot <= BR_SCALAR(0.0))						
				if(FP_TO_UINT(dot)&FP_NEG)
					continue;									
				
				l = BR_MUL(dot, attn);
				
				tvp->comp[C_I] += l;
			}
			break;
		}
		
	}
	// clamp and scale
	CLAMP_SCALE_GEOM(C_I);

	block->chain->render(block->chain, v0, v1, v2, fp, tfp);
}


/*
 * Lighting function that does nothing
 */
static void lightingIndexNull(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp,br_scalar *comp)
{
}

/*
 * Lighting for directional light source in model space
 */
static void lightingIndexDirect(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp,br_scalar *comp)
{
	br_scalar l,dot;

	dot = BrVector3Dot(n,&alp->direction);

	if(dot <= BR_SCALAR(0.0))
		return;

	l = BR_MUL(dot, self->state.surface.kd);

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		dot = BrVector3Dot(n,&alp->half);

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF)
			l += SPECULAR_POWER(BR_MUL(self->state.surface.ks,alp->intensity));
 	}

	comp[C_I] += l;
}

/*
 * Lighting for point light source with attenuation
 * Index
 */
static void lightingIndexPoint(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp, br_scalar *comp)
{
	br_scalar l,dot;
	br_vector3 dirn,dirn_norm;

	/*
	 * Work out vector between point and light source
	 */
	BrVector3Sub(&dirn,&alp->position,p);
	BrVector3Normalise(&dirn_norm,&dirn);

	DIFFUSE_DOT();

	l = BR_MUL(dot,self->state.surface.kd);

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF)
			l += SPECULAR_POWER(self->state.surface.ks);
	}

	comp[C_I] += BR_MUL(l, alp->intensity);
}

/*
 * Lighting for point light source with attenuation
 * Index
 */
static void lightingIndexPointAttn(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp, br_scalar *comp)
{
	br_scalar attn,dot,l,dist;
	br_vector3 dirn,dirn_norm;

	BrVector3Sub(&dirn,&alp->position, p);
	CALCULATE_DIRN_NORM();
	CALCULATE_ATTENUATION();
	DIFFUSE_DOT();

	l = BR_MUL(dot, self->state.surface.kd);

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF)
			l += SPECULAR_POWER(self->state.surface.ks);
	}

	comp[C_I] += BR_MUL(l,attn);
}

/*
 * Lighting for spot light source
 * Index
 */
static void lightingIndexSpot(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp, br_scalar *comp)
{
	br_scalar dot_spot, dot, l, attn;
	br_vector3 dirn,dirn_norm;

	BrVector3Sub(&dirn,&alp->position,p);
	BrVector3Normalise(&dirn_norm,&dirn);
	SPOT_DOT();
	DIFFUSE_DOT();
	attn = SPOT_FALLOFF(alp->intensity);

	l = BR_MUL(dot, self->state.surface.kd);

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF)
			l += SPECULAR_POWER(self->state.surface.ks);
	}

	comp[C_I] += BR_MUL(l, attn);
}

/*
 * Lighting for spot light source with attenuation
 * Index
 */
static void lightingIndexSpotAttn(br_renderer *self, br_vector3 *p, br_vector3 *n, struct active_light *alp,br_scalar *comp)
{
	br_scalar dot,dot_spot,dist,attn,l;
	br_vector3 dirn,dirn_norm;

	BrVector3Sub(&dirn,&alp->position,p);
	CALCULATE_DIRN_NORM();
	SPOT_DOT();
	DIFFUSE_DOT();
	CALCULATE_ATTENUATION();
	attn = SPOT_FALLOFF(attn);

	l = BR_MUL(dot, self->state.surface.kd);

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		SPECULAR_DOT();

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF)
			l += SPECULAR_POWER(self->state.surface.ks);
	}

	comp[C_I] += BR_MUL(l, attn);
}

/*
 * Select a per-light component accumulation function
 */
void ActiveLightAccumulateIndexSet(struct active_light *alp)
{
	switch(alp->type) {
	case BRT_DIRECT:
			alp->accumulate_index = lightingIndexDirect;
		break;

	case BRT_POINT:
		if(alp->s->attenuation_l == BR_SCALAR(0.0) &&
		   alp->s->attenuation_q == BR_SCALAR(0.0))
			alp->accumulate_index = lightingIndexPoint;
		else
			alp->accumulate_index = lightingIndexPointAttn;
		break;

	case BRT_SPOT:
		if(alp->s->attenuation_l == BR_SCALAR(0.0) &&
		   alp->s->attenuation_q == BR_SCALAR(0.0))
			alp->accumulate_index = lightingIndexSpot;
		else
			alp->accumulate_index = lightingIndexSpotAttn;
		break;
	default:
		alp->accumulate_index = lightingIndexNull;
	 }
}

#if 0
/*
 * Special case for 1 directional light in model space
 *
 * Write the results into comp[C_I]
 */
void SURFACE_CALL SurfaceIndexLit1MD(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, br_scalar *comp)
{
	struct active_light *alp = scache.lights;
	br_scalar l,dot;

	dot = BrVector3Dot(n,&alp->direction);

	if(dot <= BR_SCALAR(0.0))
		return;

	l = BR_MUL(dot, self->state.surface.kd);

	if (self->state.surface.ks != BR_SCALAR(0.0)) {
		/*
		 * Specular
		 */
		dot = BrVector3Dot(n,&alp->half);

		/*
		 * Phong lighting approximation from Gems IV pg. 385
		 */
		if(dot > SPECULARPOW_CUTOFF)
			l += SPECULAR_POWER(BR_MUL(self->state.surface.ks,alp->intensity));
 	}

	comp[C_I] = l + self->state.surface.ka;

	/*
	 * Scale final intensity to range of indices
	 */
	CLAMP_SCALE(C_I);
}
#endif

