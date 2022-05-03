/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: light.h 2.4 1997/03/04 11:22:27 johng Exp $
 * $Locker: $
 *
 * Definitons for a light
 */
#ifndef _LIGHT_H_
#define _LIGHT_H_

enum {
	/*
	 * Type of light
	 */
	BR_LIGHT_TYPE  = 0x0003,
	 BR_LIGHT_POINT  = 0x0000,
 	 BR_LIGHT_DIRECT = 0x0001,
	 BR_LIGHT_SPOT   = 0x0002,

	/*
     * Flag indicating that calculations are done in view space
     */ 
	BR_LIGHT_VIEW  = 0x0004
};

typedef struct br_light {
	/*
	 * Optional identifier
	 */
	char *identifier;

	/*
	 * Type of light
	 */
	br_uint_8 type;

	/*
	 * Colour of light (if renderer supports it)
	 */
	br_colour colour;

	/*
	 * Attenuation of light with distance - constant, linear, and quadratic
	 * l & q only apply to point and spot lights
	 */
	br_scalar attenuation_c;
	br_scalar attenuation_l;
	br_scalar attenuation_q;

	/*
	 * Cone angles for spot light
	 */
	br_angle cone_outer;
	br_angle cone_inner;

        void * user;

} br_light;

extern br_scalar globalAmbientRed;
extern br_scalar globalAmbientGreen;
extern br_scalar globalAmbientBlue;
extern br_scalar globalAmbient;


#endif
