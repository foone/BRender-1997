/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: TEMPLATE.H 1.1 1996/09/26 19:04:04 JOHNG Exp $
 * $Locker: $
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
    struct br_tv_template *primitiveLibraryTemplate;
    struct br_tv_template *primitiveStateTemplate;
    struct br_tv_template *bufferStoredTemplate;

    /*
     * Parts templates
     */
    struct br_tv_template *partPrimitiveTemplate;
    struct br_tv_template *partOutputTemplate;
};

#ifdef __cplusplus
};
#endif
#endif

