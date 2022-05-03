/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: face_386.h 2.4 1996/11/11 16:31:10 sam Exp $
 * $Locker: $
 *
 * Prototypes for 386 downcoded per-face operations
 */
#ifndef _FACE_386_H_
#define _FACE_386_H_


#ifdef __cplusplus
extern "C" {
#endif

void BR_ASM_CALL OpTriangleMapQuad_A(struct brp_block *block, union brp_vertex *v0, union brp_vertex *v1, union brp_vertex *v2,
	struct v11face *fp, struct temp_face *tfp);

#define OpTriangleMapQuad					OpTriangleMapQuad_A

#ifdef __cplusplus
};
#endif
#endif

