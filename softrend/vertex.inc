;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: vertex.inc 2.1 1996/03/21 11:11:16 sam Exp $
;; $Locker: $
;;
;; Vertex structure sizes
;;

BRP_VERTEX_SHIFT	equ 6

FMT_VERTEX_SHIFT	equ 5

BRP_VERTEX_SIZE		equ 1 shl BRP_VERTEX_SHIFT
FMT_VERTEX_SIZE 	equ 1 shl FMT_VERTEX_SHIFT

SHOW		macro x,y
		%out x = y
		endm


	if (sizeof brp_vertex) NE BRP_VERTEX_SIZE
	.err <sizeof(brp_vertex) is assumed to be 2^BRP_VERTEX_SHIFT bytes>
	SHOW	<Size of brp_vertex     >,<%(sizeof brp_vertex)>
	endif

	if (sizeof fmt_vertex) NE FMT_VERTEX_SIZE
	.err <sizeof(fmt_vertex) is assumed to be 2^FMT_VERTEX_SHIFT bytes>
	SHOW	<Size of fmt_vertex     >,<%(sizeof fmt_vertex)>
	endif

