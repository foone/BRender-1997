/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: formats.h 2.6 1997/05/27 12:30:35 jon Exp $
 * $Locker: $
 *
 * All the geometry formats understood by the renderer
 *
 */
#ifndef _FORMATS_H_
#define _FORMATS_H_

#if EVAL
#define PAD_SIZE (2048-32)
#endif

/**
 ** Generic vertex structure used for geometry formats
 **/
struct fmt_vertex {
	br_vector3 p;			/* Point in model space				*/
	br_vector2 map;			/* Mapping coordinates				*/
	br_vector3 n;			/* Surface normal at vertex			*/
#if EVAL
	br_int_8 _pad[PAD_SIZE];
#endif
};

struct fmt_vertex_x {
	br_vector3_x p;			/* Point in model space				*/
	br_vector2_x map;		/* Mapping coordinates				*/
	br_vector3_x n;		    /* Surface normal at vertex			*/
#if EVAL
	br_int_8 _pad[PAD_SIZE];
#endif
};
		  
struct fmt_vertex_f {
	br_vector3_f p;			/* Point in model space				*/
	br_vector2_f map;		/* Mapping coordinates				*/
	br_vector3_f n;		    /* Surface normal at vertex			*/
#if EVAL
	br_int_8 _pad[PAD_SIZE];
#endif
};

/**
 ** Version 1.1 prepared model format
 **/
struct v11face {
	br_uint_16 vertices[3];		/* Vertices around face 		            */
	br_uint_16 edges[3];		/* Edges around face			            */
    br_vector4 eqn;             /* Plane equation as a 4 vector (Nx,Ny,Nz,D)*/
};

struct v11face_x {
	br_uint_16 vertices[3];		/* Vertices around face 		            */
	br_uint_16 edges[3];		/* Edges around face			            */
    br_vector4_x eqn;           /* Plane equation as a 4 vector (Nx,Ny,Nz,D)*/
};

struct v11face_f {
	br_uint_16 vertices[3];		/* Vertices around face 		            */
	br_uint_16 edges[3];		/* Edges around face			            */
    br_vector4_f eqn;           /* Plane equation as a 4 vector (Nx,Ny,Nz,D)*/
};

struct v11group {
	void *stored;					/* Group material (or NULL) 			*/

	struct v11face *faces;  	 	/* faces in group						*/
    br_colour *face_colours;   		/* Colour for geometry				    */
    br_uint_16 *face_user;          /* Per face user data                   */
	br_uint_8 *face_flags;			/* Per face flags                       */

	struct fmt_vertex *vertices;	/* vertices in group					*/
    br_colour *vertex_colours;   	/* Colour for geometry				    */
    br_uint_16 *vertex_user;        /* Per vertex user data                 */

	br_uint_16 nfaces;				/* Number of faces in this group		*/
	br_uint_16 nvertices;			/* Number of vertices in this group		*/
	br_uint_16 nedges;				/* Number of edges in this group		*/
};

struct v11group_x {
	void *stored;					/* Group material (or NULL) 			*/

	struct v11face_x *faces;	 	/* faces in group						*/
    br_colour *face_colours;   		/* Colour for geometry				    */
    br_uint_16 *face_user;   		/* Per face user data                   */
	br_uint_8 *face_flags;			/* Per face flags                       */

	struct fmt_vertex_x *vertices;	/* vertices in group					*/
    br_colour *vertex_colours;   	/* Colour for geometry				    */
    br_uint_16 *vertex_user;       	/* Per vertex user data                 */

	br_uint_16 nfaces;				/* Number of faces in this group		*/
	br_uint_16 nvertices;			/* Number of vertices in this group		*/
	br_uint_16 nedges;				/* Number of edges in this group		*/
};

struct v11group_f {
	void *stored;					/* Group material (or NULL) 			*/

	struct v11face_f *faces;	 	/* faces in group						*/
    br_colour *face_colours;   		/* Colour for geometry				    */
    br_uint_16 *face_user;   		/* Per face user data                   */
	br_uint_8 *face_flags;			/* Per face flags                       */

	struct fmt_vertex_f *vertices;	/* vertices in group					*/
    br_colour *vertex_colours;   	/* Colour for geometry				    */
    br_uint_16 *vertex_user;        /* Per vertex user data                 */

	br_uint_16 nfaces;				/* Number of faces in this group		*/
	br_uint_16 nvertices;			/* Number of vertices in this group		*/
	br_uint_16 nedges;				/* Number of edges in this group		*/
};

enum{
	V11MODF_LIT=1,
};

struct v11model {
	br_size_t size;
	br_uint_32 flags;
	br_uint_16 ngroups;
	br_vector3 pivot;

	struct v11group *groups;
	br_scalar radius;
};

struct v11model_x {
	br_size_t size;
	br_uint_32 flags;
	br_uint_16 ngroups;
	br_vector3_x pivot;

	struct v11group_x *groups;
	br_scalar radius;
};

struct v11model_f { // why do I get the feeling these aren't used.
	br_size_t size;
	br_uint_32 flags;
	br_uint_16 ngroups;
	br_vector3_f pivot;

	struct v11group_f *groups;
	br_scalar radius;
};
#endif
