/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: template.h 1.4 1997/01/27 19:23:51 STEVEW Exp $
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

    struct br_tv_template *deviceTemplate;
    struct br_tv_template *devicePixelmapTemplate;
    struct br_tv_template *deviceClutTemplate;
    struct br_tv_template *outputFacilityTemplate;
    struct br_tv_template *hwndTemplate ;
    struct br_tv_template *matchTemplate ;
    struct br_tv_template *matchOffTemplate ;
};

#ifdef __cplusplus
};
#endif
#endif

