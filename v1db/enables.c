/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: enables.c 2.7 1996/10/09 15:55:18 NEELA Exp $
 * $Locker: $
 *
 * Enabling/disabling of 'special' places in actor hierarchy
 */
#include "v1db.h"
#include "shortcut.h"
#include "brassert.h"
#include "assert.h"
#include "math_ip.h"

BR_RCS_ID("$Id: enables.c 2.7 1996/10/09 15:55:18 NEELA Exp $")


static void actorEnable(br_v1db_enable *e, br_actor *a)
{
	int i;

	ASSERT_MESSAGE("actorEnable NULL pointer to an actor", a != NULL);
	ASSERT(a->type == e->type);

	if(e->enabled == NULL)
		e->enabled = BrResAllocate(v1db.res, e->max * sizeof(*e->enabled), BR_MEMORY_ENABLED_ACTORS);

	/*
	 * Look to see if actor is already enabled
	 */
	for(i=0; i < e->max; i++)
		if(e->enabled[i] == a)
			return;

	/*
	 * Find a blank slot
	 */
	for(i=0; i < e->max; i++) {
		if(e->enabled[i] == NULL) {
			e->enabled[i] = a;
			e->count++;
			return;
		}
	}

	BR_ERROR1("too many enabled %ss",e->name);
}

static void actorDisable(br_v1db_enable *e, br_actor *a)
{
	int i;

	ASSERT_MESSAGE("actorDisable NULL pointer to an actor", a != NULL);
//	ASSERT(a->type == e->type);
	assert(a->type == e->type);

	if(e->enabled == NULL)
		return;

	/*
	 * Find actor in table and remove it
	 */
	for(i=0; i < e->max; i++) {
		if(e->enabled[i] == a) {
			e->enabled[i] = NULL;
			e->count--;
			return;
		}
	}
}

/*
 * Add a light to the set that will illuminate the world
 */
void BR_PUBLIC_ENTRY BrLightEnable(br_actor *l)
{
    UASSERT_MESSAGE("BrLightEnable NULL pointer to a light actor", l != NULL);

	actorEnable(&v1db.enabled_lights,l);
}

/*
 * Remove a light from the set that will illuminate the world
 */
void BR_PUBLIC_ENTRY BrLightDisable(br_actor *l)
{

    UASSERT_MESSAGE("BrLightDisable NULL pointer to a light actor", l != NULL);
	actorDisable(&v1db.enabled_lights,l);
}

/*
 * Add a clip plane to world
 */
void BR_PUBLIC_ENTRY BrClipPlaneEnable(br_actor *c)
{

    UASSERT_MESSAGE("BrClipPlaneEnable NULL pointer to a clip plane actor", c != NULL);
	actorEnable(&v1db.enabled_clip_planes,c);
}

/*
 * Remove a clip plane 
 */
void BR_PUBLIC_ENTRY BrClipPlaneDisable(br_actor *c)
{

    UASSERT_MESSAGE("BrClipPlaneDisable NULL pointer to a clip plane actor", c != NULL);
	
	actorDisable(&v1db.enabled_clip_planes,c);
}

/*
 * Add a horizon plane to world
 */
void BR_PUBLIC_ENTRY BrHorizonPlaneEnable(br_actor *h)
{
    UASSERT_MESSAGE("BrHorizonPlaneEnable NULL pointer to a horizon plane actor", h != NULL);
	    
	actorEnable(&v1db.enabled_horizon_planes,h);
}

/*
 * Remove a horizon plane 
 */
void BR_PUBLIC_ENTRY BrHorizonPlaneDisable(br_actor *h)
{

    UASSERT_MESSAGE("BrHorizonPlaneDisable NULL pointer to a horizon plane actor", h != NULL);


	actorDisable(&v1db.enabled_horizon_planes,h);
}

/*
 * Sets the new environment anchor
 *
 * Returns the previous value
 */
br_actor * BR_PUBLIC_ENTRY BrEnvironmentSet(br_actor *a)
{

	br_actor *old_a = v1db.enabled_environment;

	v1db.enabled_environment = a;

	return old_a;
}

/*
 * Build transforms between the view and a given actor
 */
static br_boolean setupView(
	br_matrix34 *view_to_this,
	br_matrix34 *this_to_view,
	br_matrix34 *world_to_view, 
	br_int_32 w2vt,
	br_actor *world, br_actor *a)
{
	br_matrix34 this_to_world;
	br_int_32 root_t,t;

    ASSERT_MESSAGE("setupView NULL pointer", view_to_this != NULL);
    ASSERT_MESSAGE("setupView NULL pointer", this_to_view != NULL);
    ASSERT_MESSAGE("setupView NULL pointer", world_to_view != NULL);
    ASSERT_MESSAGE("setupView NULL pointer", world != NULL);

	/*
	 * Find this->world, fail if not in world
	 */
	if(!ActorToRootTyped(a, world, &this_to_world, &root_t))
		return BR_FALSE;

	/*
	 * Make this->view and invert it
	 */
	BrMatrix34Mul(this_to_view, &this_to_world, world_to_view);
	t = BrTransformCombineTypes(root_t, w2vt);

	/*
	 * Build view->light
	 */
	if(BrTransformTypeIsLP(t))
		BrMatrix34LPInverse(view_to_this,this_to_view);
	else
		BrMatrix34Inverse(view_to_this,this_to_view);

	return BR_TRUE;
}

/*
 * Configure renderer with current enabled lights
 */
void BrSetupLights(br_actor *world, br_matrix34 *world_to_view, br_int_32 w2vt)
{
	br_matrix34 this_to_view, view_to_this;
	int light_part = 0,i;
	br_token_value tv[16],*tvp;
	br_vector3 view_position, view_direction;
	br_light *light;

    ASSERT_MESSAGE("BrSetupLights NULL pointer to actor", world != NULL);
    ASSERT_MESSAGE("BrSetupLights NULL pointer", world_to_view != NULL);

	if(v1db.enabled_lights.enabled == NULL)
		return;

	for(i=0; i < v1db.enabled_lights.max; i++) {
		tvp = tv;

		if(v1db.enabled_lights.enabled[i] == NULL) {
			continue;
		}

		light = v1db.enabled_lights.enabled[i]->type_data;

		ASSERT_MESSAGE("Invalid light data", light != NULL);

		/*
		 * Work out view<->light transforms - ignore light if not part of current hierachy
		 */
		if(!setupView(&view_to_this, & this_to_view, world_to_view, w2vt,
			world, v1db.enabled_lights.enabled[i]))
			continue;

		/*
		 * Construct generic parts of light
		 */
		tvp->t = BRT_COLOUR_RGB;
		tvp->v.rgb = light->colour;
		tvp++;

		tvp->t = BRT_AS_SCALAR(COLOUR_R);
		tvp->v.s = BrFixedToScalar(BR_RED(light->colour)<<8);
		tvp++;
		
		tvp->t = BRT_AS_SCALAR(COLOUR_G);
		tvp->v.s = BrFixedToScalar(BR_GRN(light->colour)<<8);
		tvp++;

		tvp->t = BRT_AS_SCALAR(COLOUR_B);
		tvp->v.s = BrFixedToScalar(BR_BLU(light->colour)<<8);
		tvp++;

		tvp->t = BRT_AS_SCALAR(ATTENUATION_C);
		tvp->v.s = light->attenuation_c;
		tvp++;

		tvp->t = BRT_AS_SCALAR(ATTENUATION_L);
		tvp->v.s = light->attenuation_l;
		tvp++;

		tvp->t = BRT_AS_SCALAR(ATTENUATION_Q);
		tvp->v.s = light->attenuation_q;
		tvp++;

		tvp->t = BRT_SPACE_T;
		tvp->v.t = (light->type & BR_LIGHT_VIEW)?BRT_VIEW:BRT_MODEL;
		tvp++;

		tvp->t = BRT_TYPE_T;

		switch(light->type & BR_LIGHT_TYPE) {

		case BR_LIGHT_POINT:
			tvp->v.t = BRT_POINT;
			tvp++;

			/*
			 * Transform position (0,0,0,1) into view space
			 */
			BrVector3CopyMat34Row(&view_position,&this_to_view,3);
			tvp->t = BRT_AS_VECTOR3_SCALAR(POSITION);
			tvp->v.p = &view_position;
			tvp++;

			break;

		case BR_LIGHT_DIRECT:
			tvp->v.t = BRT_DIRECT;
			tvp++;

			/*
			 * Transform direction (0,0,1,0) into view space -
			 * use T(I(l_to_v)) - or column 2 of view_to_this
			 */
			BrVector3CopyMat34Col(&view_direction,&view_to_this,2);
			tvp->t = BRT_AS_VECTOR3_SCALAR(DIRECTION);
			tvp->v.p = &view_direction;
			tvp++;

			break;

		case BR_LIGHT_SPOT:
			tvp->v.t = BRT_SPOT;
			tvp++;

			/*
			 * Transform position and direction into view space
			 */
			BrVector3CopyMat34Row(&view_position,&this_to_view,3);
			tvp->t = BRT_AS_VECTOR3_SCALAR(POSITION);
			tvp->v.p = &view_position;
			tvp++;

			BrVector3CopyMat34Col(&view_direction,&view_to_this,2);
			tvp->t = BRT_AS_VECTOR3_SCALAR(DIRECTION);
			tvp->v.p = &view_direction;
			tvp++;

			tvp->t = BRT_AS_SCALAR(SPOT_OUTER);
			tvp->v.s = BR_COS(light->cone_outer);
			tvp++;

			tvp->t = BRT_AS_SCALAR(SPOT_INNER);
			tvp->v.s = BR_COS(light->cone_inner);
			tvp++;

			break;

		default:
			continue;
		}

		/*
		 * Send finished token list off to renderer
		 */
		tvp->t = BR_NULL_TOKEN;
		RendererPartSetMany(v1db.renderer, BRT_LIGHT, light_part, tv, NULL);
		light_part++;
	}

	/*
	 * Disable any previous remaining lights
	 */
	tv[0].t = BRT_TYPE_T;
	tv[0].v.t = BRT_NONE;
	tv[1].t = BR_NULL_TOKEN;

	for( ; light_part < v1db.max_light; light_part++)
		RendererPartSetMany(v1db.renderer, BRT_LIGHT, light_part, tv, NULL);

	v1db.max_light = light_part;
}

/*
 * Configure renderer with current enabled clip planes
 */
void BrSetupClipPlanes(br_actor *world, br_matrix34 *world_to_view, br_int_32 w2vt, br_matrix4 *view_to_screen)
{
	br_matrix34 this_to_view, view_to_this;
	br_matrix4 screen_to_view, tmp4, screen_to_this;
	int light_part = 0,i;
	br_int_32 clip_part;
	br_token_value tv[3];
	br_vector4 sp;

    ASSERT_MESSAGE("BrSetupClipPlanes NULL pointer to actor", world != NULL);
    ASSERT_MESSAGE("BrSetupClipPlanes NULL pointer", world_to_view != NULL);
    ASSERT_MESSAGE("BrSetupClipPlanes NULL pointer", view_to_screen != NULL);

	tv[0].t = BRT_TYPE_T;
	tv[0].v.t = BRT_PLANE;
	tv[1].t = BRT_AS_VECTOR4_SCALAR(PLANE);
	tv[1].v.p = &sp;
	tv[2].t = BR_NULL_TOKEN;

	clip_part = 0;

	if(v1db.enabled_clip_planes.enabled == NULL)
		return;

	if(v1db.enabled_clip_planes.count)
		BrMatrix4Inverse(&screen_to_view, view_to_screen);

	for(i=0; i < v1db.enabled_clip_planes.max; i++) {
		if(v1db.enabled_clip_planes.enabled[i] == NULL) {
			continue;
		}

		if(!setupView(&view_to_this, & this_to_view, world_to_view, w2vt,
			world, v1db.enabled_clip_planes.enabled[i]))
			continue;

		/*
		 * Make screen->plane
		 */
		BrMatrix4Copy34(&tmp4,&view_to_this);
		
		BrMatrix4Mul(&screen_to_this,&screen_to_view, &tmp4);

		/*
		 * Push plane through to screen space
		 */
		BrMatrix4TApply(&sp, v1db.enabled_clip_planes.enabled[i]->type_data, &screen_to_this);

		RendererPartSetMany(v1db.renderer, BRT_CLIP, clip_part, tv, NULL);
		clip_part++;
	}	

	tv[0].t = BRT_TYPE_T;
	tv[0].v.t = BRT_NONE;
	tv[1].t = BR_NULL_TOKEN;
	
	for( ; clip_part < v1db.max_clip; clip_part++)
		RendererPartSetMany(v1db.renderer, BRT_CLIP, clip_part, tv, NULL);

	v1db.max_clip = clip_part;
}

/*
 * Configure renderer with current environment
 */
void BrSetupEnvironment(br_actor *world, br_matrix34 *world_to_view, br_int_32 w2vt)
{
	br_matrix34 view_to_this, this_to_view;
	br_token h = BRT_DONT_CARE;

    ASSERT_MESSAGE("BrSetupEnvironment NULL pointer to actor", world != NULL);
    ASSERT_MESSAGE("BrSetupEnvironment NULL pointer", world_to_view != NULL);

	if(v1db.enabled_environment) {

		if(v1db.enabled_environment != world) {
			if(setupView(&view_to_this, & this_to_view, world_to_view, w2vt,
				world, v1db.enabled_environment))
				h = BRT_NONE;

		} else {
			if(BrTransformTypeIsLP(w2vt))
				BrMatrix34LPInverse(&view_to_this, world_to_view);
			else
				BrMatrix34Inverse(&view_to_this, world_to_view);

			h = BRT_NONE;
		}

	} 

	/*
	 * Send to renderer
	 */
	if(h != BRT_DONT_CARE)
		RendererPartSet(v1db.renderer, BRT_MATRIX, 0, BRT_AS_MATRIX34_SCALAR(VIEW_TO_ENVIRONMENT), (br_uint_32)&view_to_this);
	RendererPartSet(v1db.renderer, BRT_MATRIX, 0, BRT_VIEW_TO_ENVIRONMENT_HINT_T, h);
}

/*
 * Generate the current horizon planes
 */
void BrSetupHorizons(br_actor *world, br_matrix34 *world_to_view, br_int_32 w2vt)
{
	/* XXX */
}

/*
 * See if this is an 'enabled' actor
 */

/*
 * Check if an actor is enabled, prior to freeing
 *
 * This is not per the manual, which states that the app.
 * should disable actors before freeing them, but it does let
 * code that 'just' worked under 1.1 to function
 */
void BrActorEnableCheck(br_actor *a)
{
	/*
	 * Is the actor the environment anchor?
	 */
	if(a == v1db.enabled_environment)
		v1db.enabled_environment = NULL;

	/*
	 * See if actor of any specific type
	 */
	switch(a->type) {
	case BR_ACTOR_LIGHT:
		actorDisable(&v1db.enabled_lights,a);
		break;
	case BR_ACTOR_CLIP_PLANE:
		actorDisable(&v1db.enabled_clip_planes,a);
		break;
	case BR_ACTOR_HORIZON_PLANE:
		actorDisable(&v1db.enabled_horizon_planes,a);
		break;
	}
}

