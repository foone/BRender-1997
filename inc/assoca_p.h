/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: assoca_p.h 1.1 1996/10/29 16:01:01 STEVEW Exp $
 * $Locker: $
 *
 * Public function prototypes for BRender associative arrays
 */
#ifndef _ASSOC_P_H_
#define _ASSOC_P_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _NO_PROTOTYPES


br_associative_array *BrAssociativeArrayAllocate();
br_error BrAssociativeArraySetEntry( br_associative_array *pArray, br_token t, br_value v );
br_error BrAssociativeArrayRemoveEntry( br_associative_array *pArray, br_token t );
br_error BrAssociativeArrayQuery( br_associative_array *pArray, br_token t, br_value *pValue );



#endif /* _NO_PROTOTYPES */

#ifdef __cplusplus
};
#endif
#endif
