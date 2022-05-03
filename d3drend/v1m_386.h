/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: v1m_386.h 1.1 1997/07/11 16:16:04 jon Exp JON $
 * $Locker: JON $
 *
 * Prototypes for 386 downcoded geometry support that is specific to V1 models
 */
#ifndef _V1M_386_H_
#define _V1M_386_H_

#ifdef __cplusplus
extern "C" {
#endif

void GEOMETRY_CALL V1Face_CullOneSidedPerspective_A(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL V1Face_OS_CullOneSidedPerspective_A(struct br_geometry *self, struct br_renderer *renderer);

#if BASED_FIXED
#define V1Face_CullOneSidedPerspective	V1Face_CullOneSidedPerspective_A
#define V1Face_OS_CullOneSidedPerspective	V1Face_OS_CullOneSidedPerspective_A
#endif

#ifdef __cplusplus
};
#endif
#endif
