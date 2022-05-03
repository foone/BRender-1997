/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: ports.c 2.1 1996/01/16 12:12:22 sam Exp $
 * $Locker: $
 *
 * IO port management - always succeeds at the moment
 */
#include "host.h"
#include "host_ip.h"

br_error BR_RESIDENT_ENTRY HostPortEnable(br_uint_16 port, br_boolean exclusive)
{
	return BRE_OK;
}

br_error BR_RESIDENT_ENTRY HostPortDisable(br_uint_16 port)
{
	return BRE_OK;
}

br_error BR_RESIDENT_ENTRY HostPortRangeEnable(br_uint_16 low, br_uint_16 high, br_boolean exclusive)
{
	return BRE_OK;
}

br_error BR_RESIDENT_ENTRY HostPortRangeDisable(br_uint_16 low, br_uint_16 high)
{
	return BRE_OK;
}

