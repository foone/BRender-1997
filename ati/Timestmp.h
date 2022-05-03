/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: TIMESTMP.H 2.1 1996/03/07 20:41:56 sam Exp $
 * $Locker: $
 *
 * Private device driver structure
 */
#ifndef _TIMESTMP_H_
#define _TIMESTMP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Driver-wide timestamp
 */
typedef br_uint_32 br_timestamp;

extern br_timestamp ATIDriverTimestamp;

#define Timestamp()     (ATIDriverTimestamp += 2)

/*
 * Initial timestamp 
 */
#define TIMESTAMP_START		1

/*
 * A time that will never happen
 */
#define INVALID_TIME		0

#ifdef __cplusplus
};
#endif
#endif

