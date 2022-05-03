#define BR_ASSOCIATIVE_ARRAY_BLOCK_SIZE 10

typedef struct br_associative_array_tag
{
   br_token_value *tv ;
   br_uint_16 num_elements ;
   br_uint_16 max_elements ;
}
br_associative_array ;


br_associative_array *BrAssociativeArrayAllocate();
br_error BrAssociativeArraySetEntry( br_associative_array *pArray, br_token t, br_value v );
br_error BrAssociativeArrayRemoveEntry( br_associative_array *pArray, br_token t );
br_error BrAssociativeArrayQuery( br_associative_array *pArray, br_token t, br_value *pValue );