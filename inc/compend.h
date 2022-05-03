/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: compend.h 1.2 1996/10/03 11:09:43 sam Exp $
 * $Locker: $
 *
 * Included at end of BRender includes - undoes any compiler specific pragmas
 */
#ifndef _COMPEND_H_
#define _COMPEND_H_

/*
 * WATCOM C/C++ 32
 */
#if defined(__WATCOMC__)

#ifndef __H2INC__
#pragma pack()
#endif

/*
 * Zortech C++
 */
#elif defined(__ZTC__)

/*
 * GNU C
 */
#elif defined (__GNUC__)


/*
 * Apple MPW C
 */
#elif defined (__MPW__)

/*
 * Intel Proton
 */
#elif defined (__PROTONC__)

/*
 * Microsoft Visual C++
 */
#elif defined (_MSC_VER)

#ifndef __H2INC__
#pragma pack()
#endif

/*
 * Metaware High-C Version 1
 */
#elif defined (__HIGHC_V1__)

/*
 * Metaware High-C Version 3
 */
#elif defined (__HIGHC__)

/*
 * Borland BC 4
 */
#elif defined (__BORLANDC__)

/*
 * IBM CSet++
 */
#elif defined (__IBMC__)

#endif

#endif



