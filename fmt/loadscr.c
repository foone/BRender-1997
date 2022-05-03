/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: loadscr.c 2.15 1997/07/14 10:44:00 JOHNG Exp $
 * $Locker: $
 *
 * Load a Material Script file
 */
#include "brddi.h"

BR_RCS_ID("$Id: loadscr.c 2.15 1997/07/14 10:44:00 JOHNG Exp $")

/*
 * Default material fields
 */
br_material _DefaultScriptMaterial = {
	0,
	NULL,
	BR_COLOUR_RGB(255,255,255),	/* colour			*/
	255,						/* opacity			*/

	BR_UFRACTION(0.10),			/* Indexed ka		*/
	BR_UFRACTION(0.70),			/*         kd		*/
	BR_UFRACTION(0.0),			/*         ks		*/
	
	BR_COLOUR_RGB(26,26,26),

	BR_SCALAR(20),				/* power			*/
	BR_MATF_LIGHT,				/* flags			*/
	{{
		BR_VECTOR2(1,0),		/* map transform	*/
		BR_VECTOR2(0,1),
		BR_VECTOR2(0,0),
	}},
	0,63,						/* index base/range	*/
};

enum {
	/*
	 * Keywords
	 */
	T_MATERIAL = T_KEYWORD,

	/*
	 * Members of material
	 */
	T_IDENTIFIER,
	T_NAME,
	T_FLAGS,
	T_COLOUR,
	T_OPACITY,
	T_AMBIENT,
	T_KA,
	T_DIFFUSE,
	T_KD,
	T_SPECULAR,
	T_KS,
	T_POWER,
	T_MAP_TRANSFORM,
	T_INDEX_BASE,
	T_INDEX_RANGE,
	T_COLOUR_MAP,
	T_SCREEN_DOOR,
	T_INDEX_SHADE,
	T_INDEX_BLEND,
    T_FOG_MIN,
    T_FOG_MAX,
    T_FOG_COLOUR,
    T_INDEX_FOG,
    T_SUBDIVIDE_TOLERANCE,

	/*
	 * Material flags
	 */
	T_LIGHT,
	T_PRELIT,
	T_SMOOTH,
	T_FLAT,
	T_GOURAUD,
	T_ENVIRONMENT,
	T_ENVIRONMENT_I,
	T_ENVIRONMENT_LOCAL,
	T_ENVIRONMENT_L,
	T_PERSPECTIVE,
	T_DECAL,
	T_ALWAYS_VISIBLE,
	T_TWO_SIDED,
	T_FORCE_FRONT,
	T_FORCE_BACK,
	T_DITHER,
	T_EXTRA_SURFACE,
	T_EXTRA_PRIMITIVE,
    T_MAP_ANTIALIASING,
    T_MAP_INTERPOLATION,
    T_MIP_INTERPOLATION,
    T_FOG_LOCAL,
    T_SUBDIVIDE,
	T_QUAD_MAPPING,
	T_NO_RENDER,
	T_NO_SCALABILITY,
};

static br_lexer_keyword keywords[] = {
    { "material",                   T_MATERIAL,             },
    { "identifier",                 T_IDENTIFIER,           },
    { "name",                       T_NAME,                 },
    { "flags",                      T_FLAGS,                },
    { "colour",                     T_COLOUR,               },
    { "opacity",                    T_OPACITY,              },
    { "ambient",                    T_AMBIENT,              },
    { "ka",                         T_KA,                   },
    { "diffuse",                    T_DIFFUSE,              },
    { "kd",                         T_KD,                   },
    { "specular",                   T_SPECULAR,             },
    { "ks",                         T_KS,                   },
    { "power",                      T_POWER,                },
    { "map_transform",              T_MAP_TRANSFORM,        },
    { "index_base",                 T_INDEX_BASE,           },
    { "index_range",                T_INDEX_RANGE,          },
    { "colour_map",                 T_COLOUR_MAP,           },
    { "screen_door",                T_SCREEN_DOOR,          },
    { "index_shade",                T_INDEX_SHADE,          },
    { "index_blend",                T_INDEX_BLEND,          },
    { "light",                      T_LIGHT,                },
    { "prelit",                     T_PRELIT,               },
    { "smooth",                     T_SMOOTH,               },
    { "flat",						T_FLAT,               },
    { "gouraud",                    T_GOURAUD,              },
    { "environment",                T_ENVIRONMENT,          },
    { "environment_i",              T_ENVIRONMENT_I,        },
    { "environment_local",          T_ENVIRONMENT_LOCAL,    },
    { "environment_l",              T_ENVIRONMENT_L,        },
    { "perspective",                T_PERSPECTIVE,          },
    { "decal",                      T_DECAL,                },
    { "always_visible",             T_ALWAYS_VISIBLE,       },
    { "two_sided",                  T_TWO_SIDED,            },
    { "force_z_0",                  T_FORCE_FRONT,          },
    { "force_front",                T_FORCE_FRONT,          },
    { "force_back",                 T_FORCE_BACK,           },
    { "dither",                     T_DITHER,               },
    { "extra_surface",              T_EXTRA_SURFACE,        },
    { "extra_primitive",            T_EXTRA_PRIMITIVE,      },
    { "map_antialiasing",           T_MAP_ANTIALIASING,     },
    { "map_interpolation",          T_MAP_INTERPOLATION,    },
    { "mip_interpolation",          T_MIP_INTERPOLATION,    },
    { "fog_local",                  T_FOG_LOCAL,            },
    { "fog_min",                    T_FOG_MIN,              },
    { "fog_max",                    T_FOG_MAX,              },
    { "fog_colour",                 T_FOG_COLOUR,           },
    { "index_fog",                  T_INDEX_FOG,            },
    { "subdivide_tolerance",        T_SUBDIVIDE_TOLERANCE,  },
    { "subdivide",                  T_SUBDIVIDE,            },
    { "quad_mapping",               T_QUAD_MAPPING,         },
    { "no_render",                  T_NO_RENDER,            },
    { "no_scalability",             T_NO_SCALABILITY,       },
};

static void BrLexerPrintPos(struct br_lexer *l)
{
	char pos[256];

	BrLexerPosition(l,pos, BR_ASIZE(pos));
        BR_WARNING1("In file %s", pos);
}

STATIC br_uint_32 ParseMatFlags(br_lexer *l)
{
	br_uint_32 f = 0;

	BrLexerExpect(l,T_LSQUARE);

	if(BrLexerCurrent(l) == T_RSQUARE) {
		BrLexerAdvance(l);
		return 0;
	}

	/*
	 * Read flag keywords until ]
	 */
	for(;;) {
		switch(BrLexerCurrent(l)) {
        case T_LIGHT:                   f |= BR_MATF_LIGHT;                     break;
        case T_PRELIT:                  f |= BR_MATF_PRELIT;                    break;
		case T_SMOOTH:
        case T_GOURAUD:                 f |= BR_MATF_SMOOTH;                    break;
        case T_FLAT:	                f |= BR_MATF_FLAT;                    break;
		case T_ENVIRONMENT_I:
        case T_ENVIRONMENT:             f |= BR_MATF_ENVIRONMENT_I;             break;
		case T_ENVIRONMENT_L:
        case T_ENVIRONMENT_LOCAL:       f |= BR_MATF_ENVIRONMENT_L;             break;
        case T_PERSPECTIVE:             f |= BR_MATF_PERSPECTIVE;               break;
        case T_DECAL:                   f |= BR_MATF_DECAL;                     break;
        case T_ALWAYS_VISIBLE:          f |= BR_MATF_ALWAYS_VISIBLE;            break;
        case T_TWO_SIDED:               f |= BR_MATF_TWO_SIDED;                 break;
        case T_FORCE_FRONT:             f |= BR_MATF_FORCE_FRONT;               break;
        case T_FORCE_BACK:              f |= BR_MATF_FORCE_BACK;                break;
        case T_DITHER:                  f |= BR_MATF_DITHER;                    break;
        case T_MAP_ANTIALIASING:        f |= BR_MATF_MAP_ANTIALIASING;          break;
        case T_MAP_INTERPOLATION:       f |= BR_MATF_MAP_INTERPOLATION;         break;
        case T_MIP_INTERPOLATION:       f |= BR_MATF_MIP_INTERPOLATION;         break;
        case T_FOG_LOCAL:               f |= BR_MATF_FOG_LOCAL;                 break;
        case T_SUBDIVIDE:               f |= BR_MATF_SUBDIVIDE;                 break;
        case T_QUAD_MAPPING:            f |= BR_MATF_QUAD_MAPPING;              break;
        case T_NO_RENDER:               f |= BR_MATF_NO_RENDER;                 break;
        case T_NO_SCALABILITY:          f |= BR_MATF_NO_SCALABILITY;            break;
		default:
			BrLexerError(l, "Unknown material flag");
		}
		BrLexerAdvance(l);

		if(BrLexerCurrent(l) == T_RSQUARE)
			break;

		BrLexerExpect(l,T_COMMA);
	}

	BrLexerExpect(l,T_RSQUARE);

	return f;
}

STATIC br_material *ParseMaterial(br_lexer *l)
{
	br_material *mat;
	br_vector3 v3;
#if 0 
	br_token_value token_buffer[512];
	int i;
#endif

	mat = BrMaterialAllocate(NULL);
	*mat = _DefaultScriptMaterial;

	/*
	 * material = [
	 */
	BrLexerExpect(l,T_MATERIAL);
	BrLexerExpect(l,T_EQUAL);
	BrLexerExpect(l,T_LSQUARE);

	while(BrLexerCurrent(l) != T_RSQUARE) {
		switch(BrLexerCurrent(l)) {
		case T_IDENTIFIER:
		case T_NAME:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);
			mat->identifier = BrResStrDup(mat,BrLexerString(l));
			BrLexerAdvance(l);
			break;

		case T_FLAGS:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->flags = ParseMatFlags(l);
			break;

		case T_COLOUR:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);

			if(BrParseVector(l, &v3.v[0],3) != 3)
				BrLexerError(l,"Colour has too few entries");

			mat->colour = BR_COLOUR_RGB(
				BrScalarToInt(v3.v[0]),
				BrScalarToInt(v3.v[1]),
				BrScalarToInt(v3.v[2]));
			break;

		case T_OPACITY:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
                        mat->opacity = (br_uint_8)BrParseInteger(l);
			break;

		case T_KA:
		case T_AMBIENT:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->ka = (br_ufraction)BrParseScalar(l);
			break;

		case T_KD:
		case T_DIFFUSE:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->kd = (br_ufraction)BrParseScalar(l);
			break;

		case T_KS:
		case T_SPECULAR:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->ks = (br_ufraction)BrParseScalar(l);
			break;

		case T_POWER:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->power = BrParseScalar(l);
			break;

		case T_MAP_TRANSFORM:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			BrParseMatrix(l, &mat->map_transform.m[0][0],2,3);
			break;

		case T_INDEX_BASE:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->index_base = (br_uint_8)BrParseInteger(l);
			break;

		case T_INDEX_RANGE:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
			mat->index_range = (br_uint_8)BrParseInteger(l);
			break;

        case T_SUBDIVIDE_TOLERANCE:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
            mat->subdivide_tolerance = (br_int_32)BrParseInteger(l);
			break;

		case T_COLOUR_MAP:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);
			mat->colour_map = BrMapFind(BrLexerString(l));
			BrLexerAdvance(l);
			break;

		case T_SCREEN_DOOR:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);
			mat->screendoor = BrTableFind(BrLexerString(l));
			BrLexerAdvance(l);
			break;

		case T_INDEX_SHADE:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);
			mat->index_shade = BrTableFind(BrLexerString(l));
			BrLexerAdvance(l);
			break;

		case T_INDEX_BLEND:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);
			mat->index_blend = BrTableFind(BrLexerString(l));
			BrLexerAdvance(l);
			break;
#if 0
		case T_EXTRA_SURFACE:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);

			BrStringToTokenValue(token_buffer, sizeof(token_buffer),BrLexerString(l));

			mat->extra_surf = index_blend = BrTableFind(BrLexerString(l));
			BrLexerAdvance(l);
			break;
#endif
        case T_FOG_MIN:
            BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
            mat->fog_min = BrParseScalar(l);
			break;

        case T_FOG_MAX:
			BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);
            mat->fog_max = BrParseScalar(l);
			break;

        case T_FOG_COLOUR:
            BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL);

			if(BrParseVector(l, &v3.v[0],3) != 3)
                BrLexerError(l,"Fog colour has too few entries");

            mat->fog_colour = BR_COLOUR_RGB(
				BrScalarToInt(v3.v[0]),
				BrScalarToInt(v3.v[1]),
				BrScalarToInt(v3.v[2]));
			break;

        case T_INDEX_FOG:
            BrLexerAdvance(l); BrLexerExpect(l,T_EQUAL); BrLexerCheck(l,T_STRING);
            mat->index_fog = BrTableFind(BrLexerString(l));
			BrLexerAdvance(l);
			break;

		default:
                        BrLexerPrintPos(l);
                        BR_WARNING1("Incorrect material member '%s' ignored",BrLexerString(l));
                        while (BrLexerCurrent(l) != T_SEMICOLON) {
                          BrLexerAdvance(l);
                        }
		}
		BrLexerExpect(l,T_SEMICOLON);
	}

	/*
	 * ];
	 */
	BrLexerAdvance(l);
	BrLexerExpect(l,T_SEMICOLON);

	return mat;
};

static void BR_CALLBACK lexerError(struct br_lexer *l, char *string)
{
	char pos[256];

	BrLexerPosition(l,pos, BR_ASIZE(pos));
	BR_FAILURE2("%s%s", pos, string);
}

#if 0
#include <stdio.h>
void BR_CALLBACK dumpLine(char *str, void *arg)
{
	puts(str);
}
#endif

br_uint_32 BR_PUBLIC_ENTRY BrFmtScriptMaterialLoadMany(char *filename,br_material **materials,br_uint_16 num)
{
	int count;
	br_lexer *l;
	br_error r;

	l = BrLexerAllocate(keywords, BR_ASIZE(keywords));
	if(l == NULL)
		return 0;
		
	BrLexerCommentSet(l,'#');
	BrLexerErrorSet(l,lexerError);
#if 0
	BrLexerDumpSet(l, dumpLine, NULL);
#endif

	r = BrLexerPushFile(l,filename);
	if(r != BRE_OK) {
	 	BrLexerFree(l);
		return 0;
	}

	/*
	 * Start up the pipleline
	 */
	BrLexerAdvance(l);

	for (count=0; BrLexerCurrent(l) != T_EOF && count<num; count++)
		materials[count] = ParseMaterial(l);

	BrLexerFree(l);
	return count;
}

br_material * BR_PUBLIC_ENTRY BrFmtScriptMaterialLoad(char *filename)
{
	br_material *ptr;

	return (BrFmtScriptMaterialLoadMany(filename,&ptr,1) != 1)?NULL:ptr;
}
