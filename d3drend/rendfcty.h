/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: rendfcty.h 1.3 1997/08/11 14:26:00 jon Exp JON $
 * $Locker: JON $
 *
 * Private renderer facility structure
 */
#ifndef _RENDFCTY_H_
#define _RENDFCTY_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Keep a copy of the DirectDraw interface with every renderer facility - prevents reference
 * counting problems
 */
#define RENDERER_FACILITY_KEEP_DDRAW 1

/*
 * Structure containing Direct3D renderer information
 */
typedef struct d3d_info {

	/*
	 * GUID and text describing this Direct3D device
	 */
	GUID FAR *guid;
	LPSTR name;
	LPSTR desc;

	/*
	 * Direct3D interface to the DirectDraw device providing this Direct3D device
	 * Copy of the DirectDraw interface because reference counting is broken?!
	 */
#if RENDERER_FACILITY_KEEP_DDRAW
	LPDIRECTDRAW2 ddraw;
#endif
	LPDIRECT3D2 d3d;
	
	/*
	 * Capabilities of this Direct3D device
	 */
	LPD3DDEVICEDESC hal_desc;
	LPD3DDEVICEDESC hel_desc;

} d3d_info;


#ifdef BR_RENDERER_FACILITY_PRIVATE

/*
 * Private state of renderer facility
 */
typedef struct br_renderer_facility {
	/*
	 * Dispatch table
	 */
	struct br_renderer_facility_dispatch *dispatch;

	/*
	 * Standard object identifier
	 */
	char *identifier;

    /*
     * Pointer to owning device
     */
    struct br_device *device;

	/*
	 * List of objects associated with this device
	 */
	void *object_list;

	/*
	 * Default state
	 */
	struct state_all default_state;

	/*
	 * Number of instances
	 */
	br_int_32	num_instances;

	/*
	 * DirectX information on this Direct3D device
	 */
	struct d3d_info d3d;

} br_renderer_facility;

#endif

#ifdef __cplusplus
};
#endif
#endif

