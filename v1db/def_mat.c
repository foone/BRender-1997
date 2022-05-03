/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: def_mat.c 2.5 1996/12/10 13:02:19 STEVEW Exp $
 * $Locker: $
 *
 * default material
 */

#include "v1db.h"
#include "host.h"
#include "datafile.h"
#include <brassert.h>


BR_RCS_ID("$Id: def_mat.c 2.5 1996/12/10 13:02:19 STEVEW Exp $")


/*
 * Global database state
 */
extern br_v1db_state BR_ASM_DATA v1db;


/*
 * Default material - this should really be in the renderer structure
 * so that private fields can be per renderer
 */

br_material *SetupDefaultMaterial()
{
   static br_material *default_mat ;

   default_mat = BrResAllocate(v1db.res,sizeof(br_material),BR_MEMORY_MATERIAL);

   if ( !default_mat )
      return NULL ;

   default_mat->identifier = "default_mat" ;
   default_mat->colour = BR_COLOUR_RGB(255,255,255) ;
   default_mat->opacity = 255 ;
   default_mat->ka = BR_UFRACTION(0.10);
   default_mat->kd = BR_UFRACTION(0.70);
   default_mat->ks = BR_UFRACTION(0.0);
   default_mat->power = BR_SCALAR(20);
   default_mat->flags = BR_MATF_LIGHT;

   // Set map transform default

   BrMatrix23Identity( &default_mat->map_transform );

   default_mat->index_base = 10;
   default_mat->index_range = 31 ;

   return default_mat ;
};
