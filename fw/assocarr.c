/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: assocarr.c 1.2 1996/12/13 14:35:42 STEVEW Exp $
 * $Locker: $
 *
 * Associative array handling code
 */

#include "fw.h"
#include <brassert.h>



br_associative_array *BrAssociativeArrayAllocate()
{
   br_associative_array *pArray ;

   pArray = (br_associative_array *) BrResAllocate( fw.res, sizeof(br_associative_array), BR_MEMORY_SCRATCH );

   if ( !pArray )
      return NULL ;

   pArray->tv = (br_token_value *) BrResAllocate( pArray, BR_ASSOCIATIVE_ARRAY_BLOCK_SIZE * sizeof(br_token_value ), BR_MEMORY_SCRATCH );
 
   if ( !pArray->tv )
     return NULL ;  

   pArray->max_elements = BR_ASSOCIATIVE_ARRAY_BLOCK_SIZE ;

   pArray->num_elements = 0 ;

   return pArray ;
}




static br_error Set_Associative_Array_Value( br_associative_array *pArray, int index, br_value v )
{
    switch ( BrTokenType(pArray->tv[index].t) )
    {
         case BRT_STRING:
           if ( pArray->tv[index].v.str )
              BrResFree( pArray->tv[index].v.str );
                  
          
           if ( v.str )
              pArray->tv[index].v.str = BrResStrDup( pArray, v.str ) ;
           else
              pArray->tv[index].v.str = NULL ;
      
           break ;

         default:
           pArray->tv[index].v = v ;
           break ;
    }

    return BRE_OK ;
}



br_error BrAssociativeArraySetEntry( br_associative_array *pArray, br_token t, br_value v )
{
   br_uint_16 i = 0 ;
   br_token_value *temp ;

   ASSERT(pArray);
   ASSERT(pArray->tv);

   // If the key is already in the array, set the value ...

   for ( i = 0 ; i < pArray->num_elements ; i++ )
   {
       if ( pArray->tv[i].t == t )
       {
            Set_Associative_Array_Value( pArray, i, v );
            return BRE_OK ;
       }
   } 

   // The key is not present in the associative array, so add it if
   // there is space ...

   if ( pArray->num_elements < pArray->max_elements )
   {
      pArray->tv[pArray->num_elements].t   = t ;
      pArray->tv[pArray->num_elements].v.u32 = 0 ;

      Set_Associative_Array_Value( pArray, pArray->num_elements++, v );

      return BRE_OK ;
   }

   // There's no more room in the associative array, so allocate a bigger one 
   // and copy over all the existing contents ..

   temp = (br_token_value *) BrResAllocate( pArray, sizeof(br_token_value) * (pArray->max_elements + BR_ASSOCIATIVE_ARRAY_BLOCK_SIZE), BR_MEMORY_APPLICATION );

   BrMemCpy( temp, pArray->tv, pArray->max_elements * sizeof(br_token_value) );
   BrMemFree(pArray->tv);
   pArray->tv = temp ;
   temp = NULL ;

   if ( !pArray->tv )
      return BRE_FAIL ;

   pArray->max_elements += BR_ASSOCIATIVE_ARRAY_BLOCK_SIZE ;
  
   pArray->tv[pArray->num_elements].t   = t ;
   pArray->tv[pArray->num_elements].v.u32 = 0 ;

   Set_Associative_Array_Value( pArray, pArray->num_elements++, v );


   return BRE_OK ;
}





br_error BrAssociativeArrayRemoveEntry( br_associative_array *pArray, br_token t )
{
   br_uint_16 i ;
   br_boolean bFound = BR_FALSE ;

   ASSERT(pArray);
   ASSERT(pArray->tv);

   for ( i = 0 ; i < pArray->num_elements ; i++ )
   {
       if ( pArray->tv[i].t == t )
       {
	        bFound = BR_TRUE ;
           break ;
       }
   } 

   if ( !bFound )
      return BRE_FAIL ;


   // Remove token's data if necesary ...

   switch ( BrTokenType(t) )
   {
       case BRT_STRING:
          if ( pArray->tv[i].v.str )
             BrResFree( pArray->tv[i].v.str );
          break ;

       default :
         break ;
   }
                  
   // Delete by moving all following entries up one place - not fast,
   // but it'll do for now. 

   for ( ; i < pArray->num_elements - 1 ; i ++ )
   {
	   BrMemCpy( &pArray->tv[i], &pArray[i+1], sizeof(br_token_value) );
   }

   pArray->num_elements -- ;
 
   return BRE_OK ;
}








br_error BrAssociativeArrayQuery( br_associative_array *pArray, br_token t, br_value *pValue )
{
   br_uint_16 i ;

   ASSERT(pArray);
   ASSERT(pArray->tv);

   // If the key is already in the array, set the value ...

   for ( i = 0 ; i < pArray->num_elements ; i++ )
   {
       if ( pArray->tv[i].t == t )
       {
           *pValue = pArray->tv[i].v ;
           return BRE_OK ;
       }
   } 


   return BRE_FAIL ;
}



