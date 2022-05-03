/*
 * Table of exported names
 */
static char * namePointers_BRCORE1[] = {
	"_BrAbort",
	"_BrAddHead",
	"_BrAddTail",
	"_BrDevAdd",
	"_BrDevAddStatic",
	"_BrDevBegin",
	"_BrDevBeginVar",
	"_BrDevCheckAdd",
	"_BrDevContainedCount",
	"_BrDevContainedFind",
	"_BrDevContainedFindMany",
	"_BrDevCount",
	"_BrDevFind",
	"_BrDevFindMany",
	"_BrDevLastBeginQuery",
	"_BrDevLastBeginSet",
	"_BrDevRemove",
	"_BrFailure",
	"_BrFatal",
	"_BrFileAdvance",
	"_BrFileAttributes",
	"_BrFileClose",
	"_BrFileEof",
	"_BrFileGetChar",
	"_BrFileGetLine",
	"_BrFileOpenRead",
	"_BrFileOpenWrite",
	"_BrFilePrintf",
	"_BrFilePutChar",
	"_BrFilePutLine",
	"_BrFileRead",
	"_BrFileWrite",
	"_BrGetEnv",
	"_BrImageAdd",
	"_BrImageDereference",
	"_BrImageLookupName",
	"_BrImageLookupOrdinal",
	"_BrImageReference",
	"_BrImageRemove",
	"_BrInsert",
	"_BrIsAlpha",
	"_BrIsDigit",
	"_BrIsPrint",
	"_BrIsSpace",
	"_BrLastErrorGet",
	"_BrLastErrorSet",
	"_BrLexerAllocate",
	"_BrLexerCommentSet",
	"_BrLexerDumpSet",
	"_BrLexerErrorSet",
	"_BrLexerFree",
	"_BrLexerPop",
	"_BrLexerPosition",
	"_BrLexerPushFile",
	"_BrLexerPushString",
	"_BrLexerTokenError",
	"_BrLogPrintf",
	"_BrMemAllocate",
	"_BrMemCalloc",
	"_BrMemCmp",
	"_BrMemCpy",
	"_BrMemFree",
	"_BrMemInquire",
	"_BrMemSet",
	"_BrMemStrDup",
	"_BrNamePatternMatch",
	"_BrNewList",
	"_BrObjectContainerFree",
	"_BrObjectListAllocate",
	"_BrParseFixed",
	"_BrParseFloat",
	"_BrParseInteger",
	"_BrParseMatrixFixed",
	"_BrParseMatrixFloat",
	"_BrParseVectorFixed",
	"_BrParseVectorFloat",
	"_BrPrimitiveLibraryFind",
	"_BrQsort",
	"_BrRegistryAdd",
	"_BrRegistryAddMany",
	"_BrRegistryAddStatic",
	"_BrRegistryClear",
	"_BrRegistryCount",
	"_BrRegistryEnum",
	"_BrRegistryFind",
	"_BrRegistryFindMany",
	"_BrRegistryNew",
	"_BrRegistryNewStatic",
	"_BrRegistryRemove",
	"_BrRegistryRemoveMany",
	"_BrRegistryRemoveStatic",
	"_BrRemHead",
	"_BrRemTail",
	"_BrRemove",
	"_BrRendererFacilityFind",
	"_BrResAdd",
	"_BrResAllocate",
	"_BrResCheck",
	"_BrResChildEnum",
	"_BrResClass",
	"_BrResDump",
	"_BrResFree",
	"_BrResFreeNoCallback",
	"_BrResIsChild",
	"_BrResRemove",
	"_BrResSize",
	"_BrResSizeTotal",
	"_BrResStrDup",
	"_BrSScanf",
	"_BrScratchAllocate",
	"_BrScratchFlush",
	"_BrScratchFree",
	"_BrScratchInquire",
	"_BrScratchString",
	"_BrScratchStringSize",
	"_BrSimpleAddHead",
	"_BrSimpleInsert",
	"_BrSimpleNewList",
	"_BrSimpleRemHead",
	"_BrSimpleRemove",
	"_BrSprintf",
	"_BrSprintfN",
	"_BrStrCat",
	"_BrStrCmp",
	"_BrStrCpy",
	"_BrStrICmp",
	"_BrStrLen",
	"_BrStrNCmp",
	"_BrStrNCpy",
	"_BrStrNICmp",
	"_BrStrRChr",
	"_BrStrToD",
	"_BrStrToF",
	"_BrStrToL",
	"_BrStrToUL",
	"_BrStringToTokenValue",
	"_BrSwap16",
	"_BrSwap32",
	"_BrSwapBlock",
	"_BrSwapFloat",
	"_BrTVTemplateAllocate",
	"_BrTVTemplateFree",
	"_BrTokenCount",
	"_BrTokenCreate",
	"_BrTokenFind",
	"_BrTokenFindMany",
	"_BrTokenIdentifier",
	"_BrTokenType",
	"_BrTokenValueCompare",
	"_BrTokenValueComparePartial",
	"_BrTokenValueDump",
	"_BrTokenValueQuery",
	"_BrTokenValueQueryAll",
	"_BrTokenValueQueryAllSize",
	"_BrTokenValueQueryMany",
	"_BrTokenValueQueryManySize",
	"_BrTokenValueSet",
	"_BrTokenValueSetMany",
	"_BrVSScanf",
	"_BrVSprintf",
	"_BrVSprintfN",
	"_BrWarning",
	"__BrAssert",
	"__BrExceptionBegin",
	"__BrExceptionEnd",
	"__BrExceptionResource",
	"__BrExceptionThrow",
	"__BrExceptionValueFetch",
	"__BrUAssert",
	"__EPI",
	"__M_br_object_container_addFront",
	"__M_br_object_container_count",
	"__M_br_object_container_find",
	"__M_br_object_container_findMany",
	"__M_br_object_container_remove",
	"__M_br_object_container_removeFront",
	"__M_br_object_container_tokensMatch",
	"__M_br_object_container_tokensMatchBegin",
	"__M_br_object_container_tokensMatchEnd",
	"__M_br_object_query",
	"__M_br_object_queryAll",
	"__M_br_object_queryAllSize",
	"__M_br_object_queryBuffer",
	"__M_br_object_queryMany",
	"__M_br_object_queryManySize",
	"__PRO",
};

/*
 * Table of ordinals
 */
static br_uint_16 nameOrdinals_BRCORE1[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	136,
	137,
	138,
	139,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	184,
	185,
};

/*
 * Table of ordinals
 */
static void * functionPointers_BRCORE1[] = {
	BrAbort                                                         , /* 0 */
	BrAddHead                                                       , /* 1 */
	BrAddTail                                                       , /* 2 */
	BrDevAdd                                                        , /* 3 */
	BrDevAddStatic                                                  , /* 4 */
	BrDevBegin                                                      , /* 5 */
	BrDevBeginVar                                                   , /* 6 */
	BrDevCheckAdd                                                   , /* 7 */
	BrDevContainedCount                                             , /* 8 */
	BrDevContainedFind                                              , /* 9 */
	BrDevContainedFindMany                                          , /* 10 */
	BrDevCount                                                      , /* 11 */
	BrDevFind                                                       , /* 12 */
	BrDevFindMany                                                   , /* 13 */
	BrDevLastBeginQuery                                             , /* 14 */
	BrDevLastBeginSet                                               , /* 15 */
	BrDevRemove                                                     , /* 16 */
	BrFailure                                                       , /* 17 */
	BrFatal                                                         , /* 18 */
	BrFileAdvance                                                   , /* 19 */
	BrFileAttributes                                                , /* 20 */
	BrFileClose                                                     , /* 21 */
	BrFileEof                                                       , /* 22 */
	BrFileGetChar                                                   , /* 23 */
	BrFileGetLine                                                   , /* 24 */
	BrFileOpenRead                                                  , /* 25 */
	BrFileOpenWrite                                                 , /* 26 */
	BrFilePrintf                                                    , /* 27 */
	BrFilePutChar                                                   , /* 28 */
	BrFilePutLine                                                   , /* 29 */
	BrFileRead                                                      , /* 30 */
	BrFileWrite                                                     , /* 31 */
	BrGetEnv                                                        , /* 32 */
	BrImageAdd                                                      , /* 33 */
	BrImageDereference                                              , /* 34 */
	BrImageLookupName                                               , /* 35 */
	BrImageLookupOrdinal                                            , /* 36 */
	BrImageReference                                                , /* 37 */
	BrImageRemove                                                   , /* 38 */
	BrInsert                                                        , /* 39 */
	BrIsAlpha                                                       , /* 40 */
	BrIsDigit                                                       , /* 41 */
	BrIsPrint                                                       , /* 42 */
	BrIsSpace                                                       , /* 43 */
	BrLastErrorGet                                                  , /* 44 */
	BrLastErrorSet                                                  , /* 45 */
	BrLexerAllocate                                                 , /* 46 */
	BrLexerCommentSet                                               , /* 47 */
	BrLexerDumpSet                                                  , /* 48 */
	BrLexerErrorSet                                                 , /* 49 */
	BrLexerFree                                                     , /* 50 */
	BrLexerPop                                                      , /* 51 */
	BrLexerPosition                                                 , /* 52 */
	BrLexerPushFile                                                 , /* 53 */
	BrLexerPushString                                               , /* 54 */
	BrLexerTokenError                                               , /* 55 */
	BrLogPrintf                                                     , /* 56 */
	BrMemAllocate                                                   , /* 57 */
	BrMemCalloc                                                     , /* 58 */
	BrMemCmp                                                        , /* 59 */
	BrMemCpy                                                        , /* 60 */
	BrMemFree                                                       , /* 61 */
	BrMemInquire                                                    , /* 62 */
	BrMemSet                                                        , /* 63 */
	BrMemStrDup                                                     , /* 64 */
	BrNamePatternMatch                                              , /* 65 */
	BrNewList                                                       , /* 66 */
	BrObjectContainerFree                                           , /* 67 */
	BrObjectListAllocate                                            , /* 68 */
	BrParseFixed                                                    , /* 69 */
	BrParseFloat                                                    , /* 70 */
	BrParseInteger                                                  , /* 71 */
	BrParseMatrixFixed                                              , /* 72 */
	BrParseMatrixFloat                                              , /* 73 */
	BrParseVectorFixed                                              , /* 74 */
	BrParseVectorFloat                                              , /* 75 */
	BrPrimitiveLibraryFind                                          , /* 76 */
	BrQsort                                                         , /* 77 */
	BrRegistryAdd                                                   , /* 78 */
	BrRegistryAddMany                                               , /* 79 */
	BrRegistryAddStatic                                             , /* 80 */
	BrRegistryClear                                                 , /* 81 */
	BrRegistryCount                                                 , /* 82 */
	BrRegistryEnum                                                  , /* 83 */
	BrRegistryFind                                                  , /* 84 */
	BrRegistryFindMany                                              , /* 85 */
	BrRegistryNew                                                   , /* 86 */
	BrRegistryNewStatic                                             , /* 87 */
	BrRegistryRemove                                                , /* 88 */
	BrRegistryRemoveMany                                            , /* 89 */
	BrRegistryRemoveStatic                                          , /* 90 */
	BrRemHead                                                       , /* 91 */
	BrRemTail                                                       , /* 92 */
	BrRemove                                                        , /* 93 */
	BrRendererFacilityFind                                          , /* 94 */
	BrResAdd                                                        , /* 95 */
	BrResAllocate                                                   , /* 96 */
	BrResCheck                                                      , /* 97 */
	BrResChildEnum                                                  , /* 98 */
	BrResClass                                                      , /* 99 */
	BrResDump                                                       , /* 100 */
	BrResFree                                                       , /* 101 */
	BrResFreeNoCallback                                             , /* 102 */
	BrResIsChild                                                    , /* 103 */
	BrResRemove                                                     , /* 104 */
	BrResSize                                                       , /* 105 */
	BrResSizeTotal                                                  , /* 106 */
	BrResStrDup                                                     , /* 107 */
	BrSScanf                                                        , /* 108 */
	BrScratchAllocate                                               , /* 109 */
	BrScratchFlush                                                  , /* 110 */
	BrScratchFree                                                   , /* 111 */
	BrScratchInquire                                                , /* 112 */
	BrScratchString                                                 , /* 113 */
	BrScratchStringSize                                             , /* 114 */
	BrSimpleAddHead                                                 , /* 115 */
	BrSimpleInsert                                                  , /* 116 */
	BrSimpleNewList                                                 , /* 117 */
	BrSimpleRemHead                                                 , /* 118 */
	BrSimpleRemove                                                  , /* 119 */
	BrSprintf                                                       , /* 120 */
	BrSprintfN                                                      , /* 121 */
	BrStrCat                                                        , /* 122 */
	BrStrCmp                                                        , /* 123 */
	BrStrCpy                                                        , /* 124 */
	BrStrICmp                                                       , /* 125 */
	BrStrLen                                                        , /* 126 */
	BrStrNCmp                                                       , /* 127 */
	BrStrNCpy                                                       , /* 128 */
	BrStrNICmp                                                      , /* 129 */
	BrStrRChr                                                       , /* 130 */
	BrStrToD                                                        , /* 131 */
	BrStrToF                                                        , /* 132 */
	BrStrToL                                                        , /* 133 */
	BrStrToUL                                                       , /* 134 */
	BrStringToTokenValue                                            , /* 135 */
	BrSwap16                                                        , /* 136 */
	BrSwap32                                                        , /* 137 */
	BrSwapBlock                                                     , /* 138 */
	BrSwapFloat                                                     , /* 139 */
	BrTVTemplateAllocate                                            , /* 140 */
	BrTVTemplateFree                                                , /* 141 */
	BrTokenCount                                                    , /* 142 */
	BrTokenCreate                                                   , /* 143 */
	BrTokenFind                                                     , /* 144 */
	BrTokenFindMany                                                 , /* 145 */
	BrTokenIdentifier                                               , /* 146 */
	BrTokenType                                                     , /* 147 */
	BrTokenValueCompare                                             , /* 148 */
	BrTokenValueComparePartial                                      , /* 149 */
	BrTokenValueDump                                                , /* 150 */
	BrTokenValueQuery                                               , /* 151 */
	BrTokenValueQueryAll                                            , /* 152 */
	BrTokenValueQueryAllSize                                        , /* 153 */
	BrTokenValueQueryMany                                           , /* 154 */
	BrTokenValueQueryManySize                                       , /* 155 */
	BrTokenValueSet                                                 , /* 156 */
	BrTokenValueSetMany                                             , /* 157 */
	BrVSScanf                                                       , /* 158 */
	BrVSprintf                                                      , /* 159 */
	BrVSprintfN                                                     , /* 160 */
	BrWarning                                                       , /* 161 */
	_BrAssert                                                       , /* 162 */
	_BrExceptionBegin                                               , /* 163 */
	_BrExceptionEnd                                                 , /* 164 */
	_BrExceptionResource                                            , /* 165 */
	_BrExceptionThrow                                               , /* 166 */
	_BrExceptionValueFetch                                          , /* 167 */
	_BrUAssert                                                      , /* 168 */
	_EPI                                                            , /* 169 */
	_M_br_object_container_addFront                                 , /* 170 */
	_M_br_object_container_count                                    , /* 171 */
	_M_br_object_container_find                                     , /* 172 */
	_M_br_object_container_findMany                                 , /* 173 */
	_M_br_object_container_remove                                   , /* 174 */
	_M_br_object_container_removeFront                              , /* 175 */
	_M_br_object_container_tokensMatch                              , /* 176 */
	_M_br_object_container_tokensMatchBegin                         , /* 177 */
	_M_br_object_container_tokensMatchEnd                           , /* 178 */
	_M_br_object_query                                              , /* 179 */
	_M_br_object_queryAll                                           , /* 180 */
	_M_br_object_queryAllSize                                       , /* 181 */
	_M_br_object_queryBuffer                                        , /* 182 */
	_M_br_object_queryMany                                          , /* 183 */
	_M_br_object_queryManySize                                      , /* 184 */
	_PRO                                                            , /* 185 */
};

static br_image Image_BRCORE1 = {
	{0},
	"BRCORE1",						/* Identifier				*/
	BR_IMG_RESIDENT,				/* Image type				*/
	0,								/* Reference count			*/

	1,									/* Ordinal base				*/
	BR_ASIZE(functionPointers_BRCORE1),	/* Number of functions		*/
	functionPointers_BRCORE1,				/* Table of functions		*/

	BR_ASIZE(namePointers_BRCORE1),	/* Number of names			*/
	namePointers_BRCORE1,				/* Table of names 			*/
	nameOrdinals_BRCORE1,				/* Table of name ordinals	*/
	
	0,								/* Number of imports		*/
	NULL,							/* Table of imports			*/

	0,								/* Number of sections		*/
	NULL,							/* Table of sections		*/

	NULL,							/* Type pointer				*/
};

