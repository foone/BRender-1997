/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: lightmac.h 1.1 1997/07/11 16:15:30 jon Exp JON $
 * $Locker: JON $
 *
 * Macros for building the lighting functions
 */

/*
 * Clamp a component to (0,1], scale and offset
 */
#define CLAMP_SCALE(c)	do {							\
	if ((c) >= BR_SCALAR(1.0))							\
		(c) = BR_SCALAR(256.0) - BR_SCALAR_EPSILON;		\
	else if ((c) <= BR_SCALAR(0.0))						\
		(c) = BR_SCALAR(0.0);							\
	else												\
		(c) = BR_MUL(BR_SCALAR(256.0), (c));		\
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
#define CALCULATE_ATTENUATION() do {				\
	br_scalar dist2;								\
	if(dist >= BR_SCALAR(180.0))					\
		dist2 = BR_SCALAR(32767.0);					\
	else											\
		dist2 = BR_MUL(dist,dist);					\
													\
	attn = BR_RCP(alp->s->attenuation_c				\
		+ BR_MUL(dist,alp->s->attenuation_l)		\
		+ BR_MUL(dist2,alp->s->attenuation_q));		\
} while(0)

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
