/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: gen_386.h 1.1 1997/07/11 16:15:24 jon Exp JON $
 * $Locker: JON $
 *
 * Prototypes for Pentium and Pentium II downcoded geometry support
 */
#ifndef _GEN_586_H_
#define _GEN_586_H_

#ifdef __cplusplus
extern "C" {
#endif

void GEOMETRY_CALL Vertex_TransformProjectOutcode_CMOV(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL Vertex_OS_TransformProject_A(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL Vertex_OS_TransformProjectSurf_A(struct br_geometry *self, struct br_renderer *renderer);

#if BASED_FLOAT
//#define Vertex_OS_TransformProject Vertex_OS_TransformProject_A
//#define Vertex_OS_TransformProjectSurf Vertex_OS_TransformProjectSurf_A
#endif

#ifdef __cplusplus
};
#endif
#endif

