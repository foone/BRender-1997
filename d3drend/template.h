/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: template.h 1.3 1997/08/11 14:26:13 jon Exp JON $
 * $Locker: JON $
 *
 * per-device store of allocated token-value templates
 */
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct device_templates {
    /*
     * Object query templates
     */
    struct br_tv_template *deviceTemplate;
    struct br_tv_template *outputFacilityTemplate;
    struct br_tv_template *devicePixelmapTemplate;
    struct br_tv_template *rendererFacilityTemplate;
    struct br_tv_template *rendererTemplate;
    struct br_tv_template *rendererStateStoredTemplate;
    struct br_tv_template *bufferStoredTemplate;
    struct br_tv_template *geometryV1ModelTemplate;
    struct br_tv_template *geometryV1BucketsTemplate;
    struct br_tv_template *geometryPrimitivesTemplate;
    struct br_tv_template *geometryLightingTemplate;

    /*
     * Argument processing templates
     */
    struct br_tv_template *pixelmapNewTemplate;
    struct br_tv_template *pixelmapMatchTemplate;
    struct br_tv_template *rendererNewTemplate;

    /*
     * Parts templates
     */
    struct br_tv_template *partCullTemplate;
    struct br_tv_template *partSurfaceTemplate;
    struct br_tv_template *partMatrixTemplate;
    struct br_tv_template *partEnableTemplate;
    struct br_tv_template *partHiddenSurfaceTemplate;
    struct br_tv_template *partBoundsTemplate;
    struct br_tv_template *partLightTemplate;
    struct br_tv_template *partClipTemplate;
    struct br_tv_template *partPrimitiveTemplate;
    struct br_tv_template *partOutputTemplate;
};

#ifdef __cplusplus
};
#endif
#endif

