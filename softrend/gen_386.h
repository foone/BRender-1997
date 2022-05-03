/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gen_386.h 2.4 1996/11/11 16:31:10 sam Exp $
 * $Locker: $
 *
 * Prototypes for 386 downcoded geometry support
 */
#ifndef _GEN_386_H_
#define _GEN_386_H_


#ifdef __cplusplus
extern "C" {
#endif

br_boolean BR_ASM_CALL ModelToViewportUpdate_A(void);

void GEOMETRY_CALL Vertex_OS_TransformProjectBounds_A(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL Vertex_OS_TransformProject_A(struct br_geometry *self, struct br_renderer *renderer);

void GEOMETRY_CALL Vertex_OS_TransformProjectBoundsSurf_A(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL Vertex_OS_TransformProjectSurf_A(struct br_geometry *self, struct br_renderer *renderer);

void GEOMETRY_CALL Vertex_OS_TransformProjectBoundsGeom_A(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL Vertex_OS_TransformProjectSurfGeom_A(struct br_geometry *self, struct br_renderer *renderer);

#if 0  // BASED_FIXED
#define Vertex_OS_TransformProjectBounds	   	Vertex_OS_TransformProjectBounds_A
#define Vertex_OS_TransformProject				Vertex_OS_TransformProject_A
#define Vertex_OS_TransformProjectBoundsSurf	Vertex_OS_TransformProjectBoundsSurf_A
#define Vertex_OS_TransformProjectSurf			Vertex_OS_TransformProjectSurf_A
#define Vertex_OS_TransformProjectBoundsGeom	Vertex_OS_TransformProjectBoundsGeom_A
#define Vertex_OS_TransformProjectSurfGeom		Vertex_OS_TransformProjectSurfGeom_A
#endif

#ifdef __cplusplus
};
#endif
#endif

