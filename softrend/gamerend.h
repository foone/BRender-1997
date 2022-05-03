#ifndef __GAMEREND_H
#define __GAMEREND_H
#include "brender.h"

#define ASSUME_PERSPECTIVE_CAMERA 1

#define ENABLE_ZSORT 0
#define ENABLE_FACE_GROUP_COUNT 1
#define TEXTURE_GROUPS_HACK 1
#define FAST_LIGHTING 1
#if FAST_LIGHTING
extern int useLight[BR_MAX_LIGHTS];
#endif

/*
 * Enable copying around of primitive states
 *
 * Currently disabled until I get to the bottom of the access violations
 */
#define COPY_PRIMITIVE_STATES 0

#endif //__GAMEREND_H
