/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: rndstate.c 1.3 1997/08/11 14:26:01 jon Exp JON $
 * $Locker: JON $
 *
 * State manipulation methods
 */
#include <stddef.h>
#include <string.h>

#include "drv.h"
#include "brassert.h"

BR_RCS_ID("$Id: rndstate.c 1.3 1997/08/11 14:26:01 jon Exp JON $");

/*
 * Setting current state
 */
br_error BR_CMETHOD_DECL(br_renderer_d3d,partSet)
		(struct br_renderer *self, br_token part, br_int_32 index, br_token t, br_uint_32 value)
{
	br_error r;
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);
	br_uint_32 m = 0;

	if(tp != NULL) {
		r = BrTokenValueSet(sp, &m, t, value, tp);
		if(m)
			TemplateActions(&self->state, part, index, m);
	}

	return r;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,partSetMany)
		(struct br_renderer *self, br_token part, br_int_32 index, br_token_value * tv, br_int_32 *pcount)
{
	br_error r;
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);
	br_uint_32 m = 0;

	if(tp != NULL) {
		r = BrTokenValueSetMany(sp, pcount, &m, tv, tp);
		if(m)
			TemplateActions(&self->state, part, index, m);
	}

	return r;
}

/*
 * Reading current state
 */
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQuery)
		(struct br_renderer *self, br_token part, br_int_32 index, br_uint_32 *pvalue, br_token t)
{
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);

	if(tp != NULL) {
		return BrTokenValueQuery(pvalue, NULL, 0, t,  sp, tp);
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryBuffer)
		(struct br_renderer *self, br_token part, br_int_32 index, br_uint_32 *pvalue, br_uint_32 *buffer, br_size_t buffer_size, br_token t)
{
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);

	if(tp != NULL) {
		return BrTokenValueQuery(pvalue, buffer, buffer_size, t,  sp, tp);
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryMany)(struct br_renderer *self,
		br_token part, br_int_32 index,
		br_token_value *tv, void *extra, br_size_t extra_size, br_int_32 *pcount)
{
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);

	if(tp != NULL) {
		return BrTokenValueQueryMany(tv, extra, extra_size, pcount,  sp, tp);
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryManySize)
		(struct br_renderer *self, br_token part, br_int_32 index, br_size_t *pextra_size, br_token_value *tv)
{
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);

	if(tp != NULL) {
		return BrTokenValueQueryManySize(pextra_size, tv, sp, tp);
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryAll)
		(struct br_renderer *self, br_token part, br_int_32 index, br_token_value *buffer, br_size_t buffer_size)
{
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);

	if(tp != NULL) {
		return BrTokenValueQueryAll(buffer, buffer_size, sp, tp);
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryAllSize)
		(struct br_renderer *self, br_token part, br_int_32 index, br_size_t *psize)
{
	struct state_all *sp = &self->state;
	struct br_tv_template *tp = FindStateTemplate(self,&sp,part,index);

	if(tp != NULL) {
		return BrTokenValueQueryAllSize(psize, sp, tp);
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d, partQueryCapability)(
        struct br_renderer *self,
        br_token part,
        br_int_32 index,
        br_token_value *buffer,
        br_size_t buffer_size)
{
    return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d, partIndexQuery)(
		struct br_renderer *self,
		br_token part,
		br_int_32 *pnindex)
{
	br_int_32 n;

	switch(part) {
	case BRT_CULL:
	case BRT_SURFACE:
	case BRT_MATRIX:
	case BRT_ENABLE:
		n = 1;
		break;

	case BRT_LIGHT:
		n = BR_ASIZE(self->state.light);
		break;

	case BRT_CLIP:
		n = BR_ASIZE(self->state.clip);
		break;

	// Add parts from primitive library
	
	default:
		return BRE_FAIL;
	}

	if(pnindex) {
		*pnindex = n;
		return BRE_OK;
	} else {
		return BRE_FAIL;
	}
}

br_error BR_CMETHOD_DECL(br_renderer_d3d, stateQueryPerformance)(
        struct br_renderer *self,
        br_fixed_lu *speed)
{
    return BRE_FAIL;
}

/*
 * Utility function to copy all or part of a state from one place to another
 *
 * The destination recalc. bitmasks are updated appropiately
 */
br_error StateCopy(struct state_all *dest, struct state_all *src, br_uint_32 copy_mask, void *res)
{
	int i;

#if 1
	if(copy_mask & (MASK_STATE_SURFACE | MASK_STATE_CULL))
		copy_mask |= MASK_STATE_CACHE;
#endif

	/*
	 * Restrict copy_mask to valid source parts
	 */
	copy_mask &= src->valid;

	/*
	 * Merge valid mask into destination
	 */
	dest->valid |= copy_mask;

	if(copy_mask & MASK_STATE_CULL)
		dest->cull = src->cull;

	if(copy_mask & MASK_STATE_SURFACE)
		dest->surface = src->surface;

	if(copy_mask & MASK_STATE_MATRIX)
		dest->matrix = src->matrix;

	if(copy_mask & MASK_STATE_ENABLE)
		dest->enable = src->enable;

	if(copy_mask & MASK_STATE_BOUNDS)
		dest->bounds = src->bounds;

	if(copy_mask & MASK_STATE_LIGHT) {
		for(i=0; i < BR_ASIZE(src->light); i++) {
			dest->light[i] = src->light[i];
		}
		dest->timestamp_lights = src->timestamp_lights;
	}

	if(copy_mask & MASK_STATE_CLIP) {
		for(i=0; i < BR_ASIZE(src->clip); i++) {
			dest->clip[i] = src->clip[i];
		}
		dest->timestamp_clips = src->timestamp_clips;
	}

	if(copy_mask & MASK_STATE_CACHE)
		dest->cache = src->cache;

	// Add parts from primitive state

#if 1
	/*
	 * Mark destination's cache as invalid if necessary
	 */
	if((copy_mask & (MASK_STATE_SURFACE | MASK_STATE_CULL)) != (MASK_STATE_SURFACE | MASK_STATE_CULL))
		dest->cache.valid = BR_FALSE;
#endif

	return BRE_OK;
}

/*
 * Convertions routines for matrices fixed<->float
 */
#if BASED_FIXED
static void convertM34FloatToFixed(br_matrix34_x *dest, br_matrix34_f *src)
{
	int i,j;

	for(i=0; i < 4; i++)
		for(j=0; j < 3; j++)
			dest->m[i][j] = BrFloatToFixed(src->m[i][j]);
}
#endif

#if BASED_FLOAT
static void convertM34FixedToFloat(br_matrix34_f *dest, br_matrix34_x *src)
{
	int i,j;

	for(i=0; i < 4; i++)
		for(j=0; j < 3; j++)
			dest->m[i][j] = BrFixedToFloat(src->m[i][j]);
}
#endif

/*
 * Special case state manipulation for ease of use
 */

#if BASED_FIXED
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelMulX)
		(struct br_renderer *self, br_matrix34_x *m)
#endif
#if BASED_FLOAT
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelMulF)
		(struct br_renderer *self, br_matrix34_f *m)
#endif
{
	br_matrix34 om = self->state.matrix.model_to_view;

	BrMatrix34Mul(&self->state.matrix.model_to_view, (br_matrix34 *)m, &om);

	self->state.matrix.model_to_view_hint = BRT_NONE;
	
	TouchModelToView(self);

	return BRE_OK;
}

#if BASED_FIXED
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelMulF)
		(struct br_renderer *self, br_matrix34_f *m)
#define CONV(d,s) convertM34FloatToFixed((br_matrix34_x *)(d),(br_matrix34_f *)(s))
#endif

#if BASED_FLOAT
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelMulX)
		(struct br_renderer *self, br_matrix34_x *m)
#define CONV(d,s) convertM34FixedToFloat((br_matrix34_f *)(d),(br_matrix34_x *)(s))
#endif
{
	br_matrix34 om = self->state.matrix.model_to_view;
	br_matrix34 cm;
	
	CONV(&cm,m);
	
	BrMatrix34Mul(&self->state.matrix.model_to_view, &cm, &om);

	self->state.matrix.model_to_view_hint = BRT_NONE;

	TouchModelToView(self);

	return BRE_OK;
}

#if BASED_FIXED
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelPopPushMulF)
		(struct br_renderer *self, br_matrix34_f *m)
#endif
#if BASED_FLOAT
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelPopPushMulX)
		(struct br_renderer *self, br_matrix34_x *m)
#endif
{
	br_matrix34 cm;
	
	if(self->stack_top == 0)
		return BRE_UNDERFLOW;

	CONV(&cm,m);

	BrMatrix34Mul(
		&self->state.matrix.model_to_view,
		&cm,
		&self->state_stack[0].matrix.model_to_view);

	self->state.matrix.model_to_view_hint = BRT_NONE;

	TouchModelToView(self);

	return BRE_OK;
}

#if BASED_FIXED
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelPopPushMulX)
		(struct br_renderer *self, br_matrix34_x *m)
#endif
#if BASED_FLOAT
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelPopPushMulF)
		(struct br_renderer *self, br_matrix34_f *m)
#endif
{
	if(self->stack_top == 0)
		return BRE_UNDERFLOW;

	BrMatrix34Mul(
		&self->state.matrix.model_to_view,
		(br_matrix34 *)m,
		&self->state_stack[0].matrix.model_to_view);

	self->state.matrix.model_to_view_hint = BRT_NONE;

	TouchModelToView(self);

	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,modelInvert)
		(struct br_renderer *self)
{
	br_matrix34 old;

	BrMatrix34Copy(&old,&self->state.matrix.model_to_view);

	if(self->state.matrix.model_to_view_hint == BRT_LENGTH_PRESERVING)
		BrMatrix34LPInverse(&self->state.matrix.model_to_view, &old);
	else
		BrMatrix34Inverse(&self->state.matrix.model_to_view, &old);

	TouchModelToView(self);

	return BRE_OK;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,statePush)
		(struct br_renderer *self, br_uint_32 mask)
{
	struct state_all *sp;

	if(self->stack_top >= MAX_STATE_STACK)
		return BRE_OVERFLOW;

	sp = self->state_stack + self->stack_top;

	self->stack_top++;

	return StateCopy(sp, &self->state, mask, self);
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,statePop)
		(struct br_renderer *self, br_uint_32 mask)
{
	struct state_all *sp;
	br_error r;

	if(self->stack_top <= 0)
		return BRE_UNDERFLOW;

	self->stack_top--;

	sp = self->state_stack + self->stack_top;

	r = StateCopy(&self->state, sp, mask, self);
	sp->valid = 0;
	return r;
}

/*
 * Convertions routines for bounds fixed<->float
 */
#if BASED_FIXED
static void convertBounds3FloatToFixed(br_bounds3_x *dest, br_bounds3_f *src)
{
	int i;

	for(i=0; i < 3; i++) {
		dest->min.v[i] = BrFloatToFixed(src->min.v[i]);
		dest->max.v[i] = BrFloatToFixed(src->max.v[i]);
	}
}
#endif

#if BASED_FLOAT
static void convertBounds3FixedToFloat(br_bounds3_f *dest, br_bounds3_x *src)
{
	int i;

	for(i=0; i < 3; i++) {
		dest->min.v[i] = BrFixedToFloat(src->min.v[i]);
		dest->max.v[i] = BrFixedToFloat(src->max.v[i]);
	}
}
#endif

#if BASED_FIXED
br_error BR_CMETHOD_DECL(br_renderer_d3d,boundsTestF)
	(struct br_renderer *self, br_token *r, br_bounds3_f *bounds_in)
#define BOUNDS_CONV(a,b) convertBounds3FloatToFixed((struct br_bounds3_x *)(a),(struct br_bounds3_f *)(b))
#endif
#if BASED_FLOAT
br_error BR_CMETHOD_DECL(br_renderer_d3d,boundsTestX)
	(struct br_renderer *self, br_token *r, br_bounds3_x *bounds_in)
#define BOUNDS_CONV(a,b) convertBounds3FixedToFloat((struct br_bounds3_f *)(a),(struct br_bounds3_x *)(b))
#endif
{
	br_bounds3 bounds;

	BOUNDS_CONV(&bounds, bounds_in);
	/*
	 * XXX cache check
	 */
	ModelToScreenUpdate(self);
	
	*r = OnScreenCheck(self, &scache.model_to_screen, &bounds);

	return BRE_OK;
}

#if BASED_FIXED
br_error BR_CMETHOD_DECL(br_renderer_d3d,boundsTestX)
	(struct br_renderer *self, br_token *r, br_bounds3_x *bounds)
#endif
#if BASED_FLOAT
br_error BR_CMETHOD_DECL(br_renderer_d3d,boundsTestF)
	(struct br_renderer *self, br_token *r, br_bounds3_f *bounds)
#endif
{
	/*
	 * XXX Cache check
	 */
	if(!scache.valid_m2s) {
		ModelToScreenUpdate(self);
		scache.valid_m2s = BR_TRUE;
	}
	
	*r = OnScreenCheck(self, &scache.model_to_screen, (br_bounds3 *)bounds);

	return BRE_OK;
}


br_error BR_CMETHOD_DECL(br_renderer_d3d, coverageTestF)
	(struct br_renderer *self, br_float *r, br_bounds3_f *bounds)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d, coverageTestX)
	(struct br_renderer *self, br_fixed_ls *r, br_bounds3_x *bounds)
{
	return BRE_FAIL;
}


br_error BR_CMETHOD_DECL(br_renderer_d3d,viewDistanceF)
	(struct br_renderer *self, br_float *r)
{
	return BRE_FAIL;
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,viewDistanceX)
	(struct br_renderer *self, br_fixed_ls *r)
{
	return BRE_FAIL;
}


br_error BR_CMETHOD_DECL(br_renderer_d3d,stateSave)
		(struct br_renderer *self, struct br_renderer_state_stored *save, br_uint_32 mask)
{
	return StateCopyToStored(save, &self->state, mask, save);
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,stateRestore)
		(struct br_renderer *self, struct br_renderer_state_stored *save, br_uint_32 mask)
{
	/*
	 * Remember the state
	 */
	self->last_restored = save;

	return StateCopyFromStored(&self->state, save, mask, self);
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,stateDefault)
		(struct br_renderer *self, br_uint_32 mask)
{
	/*
	 * Copy our default entries
	 */
	return StateCopy(&self->state, self->default_state, mask & MASK_STATE_LOCAL, self);
}

br_error BR_CMETHOD_DECL(br_renderer_d3d,stateMask)
		(struct br_renderer *self, br_uint_32 *mask, br_token *parts, int n_parts)
{
	int i;
	br_uint_32 m = 0;

	/*
	 * Add out own bits
	 */
	for(i=0; i < n_parts; i++) {
		switch(parts[i]) {
		case BRT_CULL:
			m |= MASK_STATE_CULL;
			break;

		case BRT_SURFACE:
			m |= MASK_STATE_SURFACE;
			break;

		case BRT_MATRIX:
			m |= MASK_STATE_MATRIX;
			break;

		case BRT_ENABLE:
			m |= MASK_STATE_ENABLE;
			break;

		case BRT_LIGHT:
			m |= MASK_STATE_LIGHT;
			break;

		case BRT_CLIP:
			m |= MASK_STATE_CLIP;
			break;
		}

		// Add parts from primitive library
	}

	*mask = m;

	return BRE_OK;
}

