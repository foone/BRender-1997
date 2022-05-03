/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: lightmac.h 2.3 1996/09/06 14:06:59 sam Exp $
 * $Locker: $
 *
 * Macros for building the lighting functions
 */

#define FP_NEG (0x80000000)
#define FP_TO_UINT(a) (*(unsigned int*)&(a))
extern const float epsilonX2;
extern const float unity;

#define CLAMP_SCALE_GEOM(c)	do {														\
	if(FP_TO_UINT(tvp->comp[(c)]) >= FP_TO_UINT(unity))													\
		tvp->comp[(c)] = renderer->state.cache.comp_offsets[(c)] +							\
			renderer->state.cache.comp_scales[(c)]-BR_SCALAR_EPSILON;					\
	else																			\
		tvp->comp[(c)] = BR_MUL(renderer->state.cache.comp_scales[(c)], tvp->comp[(c)]) +			\
			renderer->state.cache.comp_offsets[(c)];									\
} while(0);

#define CLAMP_MUL_SCALE_GEOM(c, m)	do {														\
	if(FP_TO_UINT(tvp->comp[(c)]) >= FP_TO_UINT(unity))													\
		tvp->comp[(c)] = renderer->state.cache.comp_offsets[(c)] +							\
			BR_MUL(renderer->state.cache.comp_scales[(c)]-BR_SCALAR_EPSILON, m);					\
	else																			\
		tvp->comp[(c)] = BR_MUL(renderer->state.cache.comp_scales[(c)], BR_MUL(tvp->comp[(c)], m)) +			\
			renderer->state.cache.comp_offsets[(c)];									\
} while(0);

/*
 * Clamp a component to (0,1], scale and offset
 */

#define CLAMP_SCALE(c)	do {														\
	if(FP_TO_UINT(comp[(c)]) >= FP_TO_UINT(unity))													\
		comp[(c)] = self->state.cache.comp_offsets[(c)] +							\
			self->state.cache.comp_scales[(c)]-BR_SCALAR_EPSILON;					\
	else																			\
		comp[(c)] = BR_MUL(self->state.cache.comp_scales[(c)], comp[(c)]) +			\
			self->state.cache.comp_offsets[(c)];									\
} while(0);

#define CLAMP_MUL_SCALE(c, m)	do {														\
	if(FP_TO_UINT(comp[(c)]) >= FP_TO_UINT(unity))													\
		comp[(c)] = self->state.cache.comp_offsets[(c)] +							\
			BR_MUL(self->state.cache.comp_scales[(c)]-BR_SCALAR_EPSILON, m);					\
	else																			\
		comp[(c)] = BR_MUL(self->state.cache.comp_scales[(c)], BR_MUL(comp[(c)], m)) +			\
			self->state.cache.comp_offsets[(c)];									\
} while(0);


/*
 * Find dot product of reflected vector and eye
 *
 * uses dirn_norm, n, and rend.eye_l
 *
 * produces dot
 */

#define SPECULAR_DOT() do {									\
		br_vector3 r;   									\
		br_scalar rd;										\
															\
		rd = BR_CONST_MUL(BrVector3Dot(&dirn_norm,n),2);	\
		BrVector3Scale(&r,n,rd);							\
		BrVector3Sub(&r,&r,&dirn_norm);						\
															\
		dot = BrVector3Dot(&rend.eye_l,&r);					\
} while(0)

/*
 * Given the dot-product of eye & reflected vector, returns specular component of lighting eqn.
 */
#define SPECULAR_POWER(l) BR_MULDIV(dot,l,self->state.surface.power-BR_MUL(self->state.surface.power,dot)+dot)

/*
 * Given dist, calculates dirn_norm & dist
 */
#define CALCULATE_DIRN_NORM() do {					\
	br_scalar s;									\
	dist = BrVector3Length(&dirn);					\
	if(dist <= 2 * BR_SCALAR_EPSILON)				\
		return;										\
	s = BR_RCP(dist);								\
	BrVector3Scale(&dirn_norm,&dirn,s);				\
} while(0)

/*
 * Given dist, calculates attn
 */
#if 0 
#define CALCULATE_ATTENUATION() do {				\
	br_scalar dist2;								\
													\
	dist2 = BR_MUL(dist,dist);						\
													\
	attn = BR_RCP(alp->s->attenuation_c				\
		+ BR_MUL(dist,alp->s->attenuation_l)		\
		+ BR_MUL(dist2,alp->s->attenuation_q));		\
} while(0)
#else
#define CALCULATE_ATTENUATION() do {				\
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
#endif

#define DIFFUSE_DOT() do { \
	dot = BrVector3Dot(n,&dirn_norm);				\
	if(dot <= BR_SCALAR(0.0))						\
		return;										\
} while(0)


#define SPOT_DOT() do { \
	dot_spot = BrVector3Dot(&dirn_norm,&alp->direction);	\
	if(dot_spot < alp->s->spot_outer)						\
		return;												\
} while(0)

#define SPOT_FALLOFF(v)	((dot_spot < alp->s->spot_inner)?\
	BR_MULDIV((v),alp->s->spot_outer - dot_spot, alp->s->spot_outer - alp->s->spot_inner) : (v))
