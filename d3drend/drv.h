/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: Drv.h 1.4 1997/08/11 14:25:03 jon Exp JON $
 * $Locker: JON $
 *
 * Private device driver structure
 */
#ifndef _DRV_H_
#define _DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gamerend.h"


#ifdef __H2INC__

#define FAR

typedef void *FARPROC;

typedef void *HWND;
typedef void *HMODULE;

typedef void GUID;

typedef void *LPSTR;

typedef void *LPDIRECTDRAW2;
typedef void *LPDIRECT3D2;
typedef void *LPDIRECTDRAWSURFACE2;
typedef void *LPDIRECTDRAWPALETTE;
typedef void *LPDIRECTDRAWCLIPPER;
typedef void *LPDIRECT3DDEVICE2;
typedef void *LPDIRECT3DVIEWPORT2;
typedef void *LPDIRECT3DTEXTURE2;

typedef long D3DTEXTUREHANDLE;

typedef void *LPDDCAPS;
typedef void *LPDDSURFACEDESC;
typedef void *LPDDPIXELFORMAT;
typedef void *LPD3DDEVICEDESC;

typedef void *LPD3DTLVERTEX;
typedef void *LPD3DHVERTEX;
typedef void *LPD3DVALUE;

#else

#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <d3d.h>

#endif

#define BR_OBJECT_PRIVATE
#define BR_DEVICE_PRIVATE
#define BR_OUTPUT_FACILITY_PRIVATE
#define BR_DEVICE_PIXELMAP_PRIVATE
#define BR_RENDERER_FACILITY_PRIVATE
#define BR_RENDERER_STATE_PRIVATE
#define BR_RENDERER_STATE_STORED_PRIVATE
#define BR_BUFFER_STORED_PRIVATE
#define BR_RENDERER_PRIVATE
#define BR_GEOMETRY_V1_MODEL_PRIVATE
#define BR_GEOMETRY_V1_BUCKETS_PRIVATE
#define BR_GEOMETRY_PRIMITIVES_PRIVATE
#define BR_GEOMETRY_LIGHTING_PRIVATE

#ifndef _BRDDI_H_
#include "brddi.h"
#endif

#ifndef _HOST_H_
#include "host.h"
#endif

#ifndef _TIMESTMP_H_
#include "timestmp.h"
#endif

#ifndef _PRIMINFO_H_
#include "priminfo.h"
#endif

#ifndef _OBJECT_H_
#include "object.h"
#endif

#ifndef _TEMPLATE_H_
#include "template.h"
#endif

#ifndef _DEVICE_H_
#include "device.h"
#endif

#ifndef _OUTFCTY_H_
#include "outfcty.h"
#endif

#ifndef _DEVPIXMP_H_
#include "devpixmp.h"
#endif

#ifndef _STATE_H_
#include "state.h"
#endif

#ifndef _RENDFCTY_H_
#include "rendfcty.h"
#endif

#ifndef _RENDERER_H_
#include "renderer.h"
#endif

#ifndef _SSTATE_H_
#include "sstate.h"
#endif

#ifndef _SBUFFER_H_
#include "sbuffer.h"
#endif

#ifndef _GV1MODEL_H_
#include "gv1model.h"
#endif

#ifndef _GV1BUCKT_H_
#include "gv1buckt.h"
#endif

#ifndef _GLIGHTING_H_
#include "lighting.h"
#endif

#ifndef _GPRIM_H_
#include "gprim.h"
#endif

#ifndef _FORMATS_H_
#include "formats.h"
#endif

#ifndef _REND_H_
#include "rend.h"
#endif

#ifndef _SURFTYPE_H_
#include "surftype.h"
#endif

#ifndef _CLIP_H_
#include "clip.h"
#endif

/*
 * Macros that exapnd to the first two arguments of a template entry
 * Builtin or device token
 */
#define BRT(t)	BRT_##t,0
#define DEV(t)	0,#t

/*
 * Pull in private prototypes
 */
#ifndef _NO_PROTOTYPES

#ifndef _DRV_IP_H_
#include "drv_ip.h"
#endif

#endif

#ifdef __cplusplus
};
#endif
#endif

