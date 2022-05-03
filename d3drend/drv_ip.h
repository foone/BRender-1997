/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: DRV_IP.H 1.5 1997/08/12 17:27:37 jon Exp JON $
 * $Locker: JON $
 *
 * Prototypes for functions internal to driver
 */
#ifndef _DRV_IP_H_
#define _DRV_IP_H_

#ifndef NO_PROTOTYPES

#ifdef __cplusplus
extern "C" {
#endif

/*
 * object.c
 */
char * BR_CMETHOD_DECL(br_object_d3d, identifier)( br_object *self);
br_device *	BR_CMETHOD_DECL(br_object_d3d, device)( br_object *self);

/*
 * device.c
 */
br_device *DeviceD3DAllocate(char *identifier, char *arguments);

/*
 * outtype.c
 */
br_error OutputFacilityD3DInitialise(br_device *dev, ddraw_info *ddraw,
	br_renderer_facility **renderer_facility_list, br_uint_32 renderer_facility_count,
	br_uint_32 *output_facility_count);
br_output_facility *OutputFacilityD3DAllocate(br_device *device,
	br_renderer_facility **renderer_facility_list, br_uint_32 renderer_facility_count,
	ddraw_info *ddraw, br_boolean full_screen);

/*
 * devpixmp.c
 */
br_device_pixelmap *DevicePixelmapD3DAllocate(struct br_device *device, struct br_output_facility *facility, br_token_value *tv);

/*
 * rendtype.c
 */
br_error RendererFacilityD3DInitialise(br_device *dev, ddraw_info *ddraw,
	br_renderer_facility **renderer_facility_list, br_uint_32 renderer_facility_list_size,
	br_uint_32 *renderer_facility_count);
br_renderer_facility *RendererFacilityD3DAllocate(br_device *device, ddraw_info *ddraw, d3d_info *d3d);


/*
 * renderer.c
 */
extern const br_token RendererPartsTokens[];

br_renderer * RendererD3DAllocate(br_device *dev, struct br_renderer_facility *type, struct br_device_pixelmap *dest);

br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModeSet)
	(struct br_renderer *self, br_token mode);
br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModeQuery)
	(struct br_renderer *self, br_token *mode);
br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModeDefault)
	(struct br_renderer *self);
br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModePush)
	(struct br_renderer *self);
br_error BR_CMETHOD_DECL(br_renderer_d3d,commandModePop)
	(struct br_renderer *self);
br_error BR_CMETHOD_DECL(br_renderer_d3d, flush)
	(struct br_renderer *self, br_boolean wait);
br_error BR_CMETHOD_DECL(br_renderer_d3d, synchronise)
	(struct br_renderer *self, br_token sync_type, br_boolean block);

#define RendererSceneBegin(self) ((br_error (*)(struct br_renderer *self))BR_CMETHOD_CALL(br_renderer,_reserved0,self))((br_renderer *)self)
#define RendererSceneEnd(self) ((br_error (*)(struct br_renderer *self))BR_CMETHOD_CALL(br_renderer,_reserved1,self))((br_renderer *)self)

/*
 * rendstat.c
 */
br_error CheckPrimitiveState(struct br_renderer *self);

br_error BR_CMETHOD_DECL(br_renderer_d3d,partSet)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_token t, br_uint_32 value);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partSetMany)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_token_value * tv, br_int_32 *pcount);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQuery)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_uint_32 *pvalue, br_token t);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryBuffer)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_uint_32 *pvalue, br_uint_32 *buffer, br_size_t buffer_size, br_token t);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryMany)(struct br_renderer *self,
		br_token part, br_int_32 index,
		br_token_value *tv, void *extra, br_size_t extra_size, br_int_32 *pcount);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryManySize)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_size_t *pextra_size, br_token_value *tv);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryAll)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_token_value *buffer, br_size_t buffer_size);
br_error BR_CMETHOD_DECL(br_renderer_d3d,partQueryAllSize)
		(struct br_renderer *self, br_token part, br_int_32 index,
		br_size_t *psize);

br_error BR_CMETHOD_DECL(br_renderer_d3d, partQueryCapability)(
        struct br_renderer *self,
        br_token part,
        br_int_32 index,
        br_token_value *buffer,
        br_size_t buffer_size);

br_error BR_CMETHOD_DECL(br_renderer_d3d, partIndexQuery)(
		struct br_renderer *self,
		br_token part,
		br_int_32 *pnindex);

br_error BR_CMETHOD_DECL(br_renderer_d3d, stateQueryPerformance)(
        struct br_renderer *self,
        br_uint_32 *speed);

br_error BR_CMETHOD_DECL(br_renderer_d3d,stateMask)
		(struct br_renderer *self, br_uint_32 *mask, br_token *parts, int n_parts);
br_error BR_CMETHOD_DECL(br_renderer_d3d,stateDefault)
		(struct br_renderer *self, br_uint_32 mask);

br_error StateCopy(struct state_all *dest, struct state_all *src, br_uint_32 copy_mask, void *res);

br_error BR_CMETHOD_DECL(br_renderer_d3d,modelMulF)
		(struct br_renderer *self, br_matrix34_f *m);
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelMulX)
		(struct br_renderer *self, br_matrix34_x *m);
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelPopPushMulF)
		(struct br_renderer *self, br_matrix34_f *m);
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelPopPushMulX)
		(struct br_renderer *self, br_matrix34_x *m);
br_error BR_CMETHOD_DECL(br_renderer_d3d,modelInvert)
		(struct br_renderer *self);

br_error BR_CMETHOD_DECL(br_renderer_d3d,boundsTestF)
	(struct br_renderer *self, br_token *r, br_bounds3_f *bounds);
br_error BR_CMETHOD_DECL(br_renderer_d3d,boundsTestX)
	(struct br_renderer *self, br_token *r, br_bounds3_x *bounds);

br_error BR_CMETHOD_DECL(br_renderer_d3d, coverageTestF)
	(struct br_renderer *self, br_float *r, br_bounds3_f *bounds);
br_error BR_CMETHOD_DECL(br_renderer_d3d, coverageTestX)
	(struct br_renderer *self, br_fixed_ls *r, br_bounds3_x *bounds);

br_error BR_CMETHOD_DECL(br_renderer_d3d,viewDistanceF)
	(struct br_renderer *self, br_float *r);
br_error BR_CMETHOD_DECL(br_renderer_d3d,viewDistanceX)
	(struct br_renderer *self, br_fixed_ls *r);

br_error BR_CMETHOD_DECL(br_renderer_d3d,statePush)
		(struct br_renderer *self, br_uint_32 mask);
br_error BR_CMETHOD_DECL(br_renderer_d3d,statePop)
		(struct br_renderer *self, br_uint_32 mask);
br_error BR_CMETHOD_DECL(br_renderer_d3d,stateSave)
		(struct br_renderer *self, struct br_renderer_state_stored *save, br_uint_32 mask);
br_error BR_CMETHOD_DECL(br_renderer_d3d,stateRestore)
		(struct br_renderer *self, struct br_renderer_state_stored *save, br_uint_32 mask);

br_error BR_CMETHOD_DECL(br_renderer_d3d,stateDefault)
		(struct br_renderer *self, br_uint_32 mask);

br_error BR_CMETHOD_DECL(br_renderer_d3d,stateMask)
		(struct br_renderer *self, br_uint_32 *mask, br_token *parts, int n_parts);

br_error BR_CMETHOD_DECL(br_renderer_d3d, testRender)(
		struct br_renderer *self,
		br_token type,
		union brp_vertex *v0,
		union brp_vertex *v1,
		union brp_vertex *v2);

/*
 * sstate.c
 */
br_renderer_state_stored *RendererStateStoredD3DAllocate(br_renderer *renderer,
	struct state_all *base_state, br_uint_32 m, br_token_value *tv);

br_error StateCopyToStored(struct br_renderer_state_stored *dest, struct state_all *src, br_uint_32 copy_mask, void *res);

br_error StateCopyFromStored(struct state_all *dest, struct br_renderer_state_stored *src, br_uint_32 copy_mask, void *res);

/*
 * sbuffer.c
 */
struct br_buffer_stored *BufferStoredD3DAllocate(br_renderer *renderer,
	br_token use, struct br_device_pixelmap *pm, br_token_value *tv);

/*
 * state.c
 */
br_error StateInitialise(struct state_all *state);
struct br_tv_template * FindStateTemplate(struct br_renderer *self, struct state_all **state, br_token part, br_int_32 index);
void TemplateActions(struct state_all *state, br_token part, br_int_32 index, br_uint_32 mask);
void TouchModelToView(br_renderer *self);

/*
 * genrend.c
 */
void GEOMETRY_CALL Vertex_ClearFlags(struct br_geometry *self, struct br_renderer *renderer);
void GEOMETRY_CALL ScratchFree(struct br_geometry *self, struct br_renderer *renderer);

void VertexGeometryFns(struct br_renderer *renderer, geometry_fn *prim_outcode);

/*
 * gprim.c
 */
br_geometry_primitives * GeometryPrimitivesAllocate(br_renderer_facility *type, char *id);

/*
 * gv1model.c
 */
struct br_geometry_v1_model * GeometryV1ModelAllocate(br_renderer_facility *type, char *id);

/*
 * gv1buckt.c
 */
struct br_geometry_v1_buckets * GeometryV1BucketsAllocate(br_renderer_facility *type, char *id);

/*
 * lighting.c
 */
struct br_geometry_lighting * GeometryLightingAllocate(br_renderer_facility *type, char *id);

/*
 * v1model.c
 */
br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, render)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
			struct br_renderer_state_stored *default_state,
			br_token type);

br_error BR_CMETHOD_DECL(br_geometry_v1_model_d3d, renderOnScreen)
		(struct br_geometry *self, struct br_renderer *renderer, struct v11model *model,
			struct br_renderer_state_stored *default_state,
			br_token type);

/*
 * convert.c
 */
void BR_ASM_CALL RenderConvert1(struct brp_block *block,
	brp_vertex *v0);
void BR_ASM_CALL RenderConvert2(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1);
void BR_ASM_CALL RenderConvert3(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1,brp_vertex *v2);
void BR_ASM_CALL RenderConvert4(struct brp_block *block,
	brp_vertex *v0, brp_vertex *v1,brp_vertex *v2,brp_vertex *v3);

void ConvertVertex(brp_vertex *dest, brp_vertex *src);

/*
 * clip.c
 */
int FaceClip(br_renderer *self, LPWORD *index_out, LPD3DTLVERTEX *clip_out, LPWORD index_in, LPD3DTLVERTEX clip_in, int n, br_uint_32 mask, br_uint_32 codes);
void ClippedRenderTriangles(struct br_renderer *renderer, brp_block *block, LPWORD index, LPD3DTLVERTEX vertex, int n,
	struct v11face *fp, struct temp_face *tfp);
br_boolean ClipLine(br_renderer *self, union brp_vertex *out, union brp_vertex *v0, union brp_vertex *v1, br_uint_32 mask, br_uint_32 codes);
void ClippedRenderLine(struct br_renderer *renderer, brp_block *block, union brp_vertex *cp_in);

/*
 * setup.c
 */
br_error CacheUpdate(br_renderer *self);
void StaticCacheUpdate_PerScene(br_renderer *self);
void StaticCacheUpdate_PerModel(br_renderer *self);
void ViewToModelUpdate(br_renderer *self);
void ModelToScreenUpdate(br_renderer *self);

void ActiveLightsFind(br_renderer *self);
void ActiveLightsUpdate(br_renderer *self);

br_int_32 GenerateSurfaceFunctions(br_renderer *self, surface_fn **fns, br_uint_32 mask);

br_uint_32 ComponentMaskToSlots(br_uint_32 cm);

void GeometryFunctionReset(br_renderer *renderer);
void GeometryFunctionAdd(br_renderer *renderer, geometry_fn *fn);
void GeometryFunctionOnScreenAdd(br_renderer *renderer, geometry_fn *fn);
void GeometryFunctionBothAdd(br_renderer *renderer, geometry_fn *fn);

void PrimBlockReset(br_renderer *renderer);
void PrimBlockAdd(br_renderer *renderer, brp_render_fn *render, brp_render_fn *render_indexed);
void PrimBlockOnScreenAdd(br_renderer *renderer, brp_render_fn *render, brp_render_fn *render_indexed);
void PrimBlockAddBoth(br_renderer *renderer, brp_render_fn *render, brp_render_fn *render_indexed);

/*
 * onscreen.c
 */
br_token OnScreenCheck(br_renderer *self, br_matrix4 *model_to_screen, br_bounds3 *bounds);

/*
 * light8.c
 */
void SURFACE_CALL SurfaceIndexZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceIndexUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceIndexLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceIndexLitSimple(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void ActiveLightAccumulateIndexSet(struct active_light *alp);

void SURFACE_CALL SurfaceIndexLit1MD(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);

/*
 * light24.c
 */
void SURFACE_CALL SurfaceColourZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourLitSimple(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourAlphaZero(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourAlphaUnlit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourAlphaLit(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceColourAlphaLitSimple(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour gcolour, LPD3DTLVERTEX d3dtlv);
void ActiveLightAccumulateColourSet(struct active_light *alp);

/*
 * mapping.c
 */
void SURFACE_CALL SurfaceMapEnvironmentInfinite(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *normal, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapEnvironmentLocal(br_renderer *self,
	br_vector3 *p, br_vector2 *map, br_vector3 *normal, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryX(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryY(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryZ(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryMap(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryMapScale(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryMapScaleTranslate(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryMapCopy(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);
void SURFACE_CALL SurfaceMapGeometryMapShift(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);

void BR_ASM_CALL OpTriangleMapQuadIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleMapQuad(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);

/*
 * alpha.c
 */
void SURFACE_CALL SurfaceAlpha(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);

/*
 * depth.c
 */
void SURFACE_CALL SurfaceLinearDepth(br_renderer *self, br_vector3 *p, br_vector2 *map, br_vector3 *n, br_colour colour, LPD3DTLVERTEX d3dtlv);

/*
 * faceops.c
 */
void BR_ASM_CALL OpTriangleClipIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleClip(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleClipConstantSurfIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleClipConstantSurf(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleConstantSurfIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleConstantSurf(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleTwoSidedConstantSurfIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleTwoSidedConstantSurf(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);

void BR_ASM_CALL OpTriangleMappingWrapFixIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleMappingWrapFix(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);

void BR_ASM_CALL OpTriangleRelightTwoSidedIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleRelightTwoSided(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);

void BR_ASM_CALL OpTriangleToLinesIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleToLines(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleToPointsIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleToPoints(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleToPointsIndexed_OS(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleToPoints_OS(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpLineClipIndexed(struct brp_block *block, LPWORD i, br_boolean modified);
void BR_ASM_CALL OpLineClip(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v);

void BR_ASM_CALL OpTriangleSubdivideIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleSubdivide(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);

void BR_ASM_CALL OpTriangleSubdivideOnScreenIndexed(struct brp_block *block, LPWORD i, br_boolean modified,
	struct v11face *fp, struct temp_face *tfp);
void BR_ASM_CALL OpTriangleSubdivideOnScreen(struct brp_block *block, LPWORD i, LPD3DTLVERTEX v,
	struct v11face *fp, struct temp_face *tfp);

void SubdivideSetThreshold(br_int_32 subdivide_tolerance);

/*
 * heap.c
 */
void BR_ASM_CALL OpHeapAddPointIndexed(struct brp_block *block, LPWORD i, br_boolean modified);
void BR_ASM_CALL OpHeapAddPoint(struct brp_block *block, LPD3DTLVERTEX v);
void BR_ASM_CALL OpHeapAddLineIndexed(struct brp_block *block, LPWORD i, br_boolean modified);
void BR_ASM_CALL OpHeapAddLine(struct brp_block *block, LPD3DTLVERTEX v);
void BR_ASM_CALL OpHeapAddTriangleIndexed(struct brp_block *block, LPWORD i, br_boolean modified);
void BR_ASM_CALL OpHeapAddTriangle(struct brp_block *block, LPD3DTLVERTEX v);


/*
 * surftype.c
 */
br_error PixelTypeToDDPixelFormat(LPDDPIXELFORMAT *format, br_uint_32 type);
br_error DDPixelFormatToPixelType(br_uint_32 *type, LPDDPIXELFORMAT format);

br_boolean PixelTypeIndexed(br_uint_32 type);
br_uint_32 PixelTypePixelBits(br_uint_32 type);
br_uint_32 PixelTypePixelBytes(br_uint_32 type);
br_uint_32 PixelTypeRBits(br_uint_32 type);
br_uint_32 PixelTypeGBits(br_uint_32 type);
br_uint_32 PixelTypeBBits(br_uint_32 type);
br_uint_32 PixelTypeABits(br_uint_32 type);

/*
 * primdraw.c
 */
br_error SetD3DGlobalState(struct br_renderer *renderer);
br_error SetD3DOutputState(struct br_renderer *renderer);
br_error SetD3DPrimitiveState(struct br_renderer *renderer, struct state_prim *old);
void InitBrpBlock(struct br_renderer *renderer);
struct brp_block *GetBrpBlock(struct br_renderer *renderer, br_token type);

/*
 * Platform dependant downcoded geometry support
 */
#ifdef __386__
#include "v1m_386.h"
#include "gen_386.h"
#endif

#if defined(__586__) || defined(__686__)
#include "gen_586.h"
#endif

/* Rampant optimism ...
 */

#ifdef __MIPS__
#endif

#ifdef __ALPHA__
#endif

#ifdef __PPC__
#endif

#ifdef __680X0__
#endif

#ifdef __cplusplus
};
#endif

#endif
#endif

