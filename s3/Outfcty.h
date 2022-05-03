/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: OUTFCTY.H 1.1 1996/05/13 14:28:45 philip Exp $
 * $Locker: $
 *
 * Private output facility structure
 */
#ifndef _OUTFCTY_H_
#define _OUTFCTY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Private state of output type
 */

typedef struct br_output_facility {
	/* Dispatch table */
	struct br_output_facility_dispatch *dispatch;

	/* Standard object identifier */
	char *identifier;

	/* List of instances associated with type */
	void *object_list;

	/* Size or maximum size of mode in pixels */
	br_int_32	width;
	br_int_32	height;	

	/* Bit depth */
	br_int_32	colour_bits;
	br_int_32	depth_bits;

	/* Pixelmap types */
	br_int_32	colour_type;
	br_int_32	depth_type;

	/* Is there a CLUT? */
	br_boolean indexed;

  /* Fullscreen mode? */
  br_boolean fullscreen;

	/* Number of instances */
	br_int_32	num_instances;

	/* Default CLUT */
	struct br_device_clut *default_clut;

  /* Video memory size
	 */
	br_int_32	video_memory;
	br_int_32	host_memory;

  // Pointer to the primitive library we are going to use for this output
  br_object *prim_lib;

} br_output_facility;

#define OutputFacilityS3Type(c) (((br_output_facility *)c)->colour_type)

#define STATIC_OUTPUT_FACILITY_S3(id,w,h,cb,db,ct,dt,idx, vm, hm)\
	{NULL,  id, NULL, w, h, cb, db, ct, dt, idx, vm, hm}

#ifdef __cplusplus
};
#endif
#endif

