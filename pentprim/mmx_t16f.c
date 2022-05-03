{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Perspective Correct, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(123),BR_SCALAR(125),BR_SCALAR(123),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUVS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_LR",
#else
		RasteriseMMX565_Z_UV_RGB_SD_RL,
		RasteriseMMX565_Z_UV_RGB_SD_LR,
		RasteriseMMX565_Z_UV_RGB_SD_RL,
		RasteriseMMX565_Z_UV_RGB_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Perspective Correct, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_BLEND,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUVS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_LR",
#else
		RasteriseMMX565_Z_UV_RGB_S_RL,
		RasteriseMMX565_Z_UV_RGB_S_LR,
		RasteriseMMX565_Z_UV_RGB_S_RL,
		RasteriseMMX565_Z_UV_RGB_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Perspective Correct, Dithered", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(123),BR_SCALAR(125),BR_SCALAR(123),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_LR",
#else
		RasteriseMMX565_Z_UV_RGB_D_RL,
		RasteriseMMX565_Z_UV_RGB_D_LR,
		RasteriseMMX565_Z_UV_RGB_D_RL,
		RasteriseMMX565_Z_UV_RGB_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_LR",
#else
		RasteriseMMX565_Z_UV_RGB_RL,
		RasteriseMMX565_Z_UV_RGB_LR,
		RasteriseMMX565_Z_UV_RGB_RL,
		RasteriseMMX565_Z_UV_RGB_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Perspective Correct, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B|CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_SD_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_SD_LR",
#else
		RasteriseMMX565_Z_UV_C_SD_RL,
		RasteriseMMX565_Z_UV_C_SD_LR,
		RasteriseMMX565_Z_UV_C_SD_RL,
		RasteriseMMX565_Z_UV_C_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Perspective Correct, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B|CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_BLEND|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_S_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_S_LR",
#else
		RasteriseMMX565_Z_UV_C_S_RL,
		RasteriseMMX565_Z_UV_C_S_LR,
		RasteriseMMX565_Z_UV_C_S_RL,
		RasteriseMMX565_Z_UV_C_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Perspective Correct, Dithered", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_D_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_D_LR",
#else
		RasteriseMMX565_Z_UV_C_D_RL,
		RasteriseMMX565_Z_UV_C_D_LR,
		RasteriseMMX565_Z_UV_C_D_RL,
		RasteriseMMX565_Z_UV_C_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_LR",
#else
		RasteriseMMX565_Z_UV_C_RL,
		RasteriseMMX565_Z_UV_C_LR,
		RasteriseMMX565_Z_UV_C_RL,
		RasteriseMMX565_Z_UV_C_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Perspective Correct, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_SD_LR",
                (void *)"_RasteriseMMX565_Z_UV_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_SD_LR",
#else
		RasteriseMMX565_Z_UV_SD_RL,
		RasteriseMMX565_Z_UV_SD_LR,
		RasteriseMMX565_Z_UV_SD_RL,
		RasteriseMMX565_Z_UV_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Perspective Correct, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_BLEND,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_S_LR",
                (void *)"_RasteriseMMX565_Z_UV_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_S_LR",
#else
		RasteriseMMX565_Z_UV_S_RL,
		RasteriseMMX565_Z_UV_S_LR,
		RasteriseMMX565_Z_UV_S_RL,
		RasteriseMMX565_Z_UV_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Perspective Correct, Dithered", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_D_LR",
                (void *)"_RasteriseMMX565_Z_UV_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_D_LR",
#else
		RasteriseMMX565_Z_UV_D_RL,
		RasteriseMMX565_Z_UV_D_LR,
		RasteriseMMX565_Z_UV_D_RL,
		RasteriseMMX565_Z_UV_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y|RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RL",
                (void *)"_RasteriseMMX565_Z_UV_LR",
                (void *)"_RasteriseMMX565_Z_UV_RL",
                (void *)"_RasteriseMMX565_Z_UV_LR",
#else
		RasteriseMMX565_Z_UV_RL,
		RasteriseMMX565_Z_UV_LR,
		RasteriseMMX565_Z_UV_RL,
		RasteriseMMX565_Z_UV_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(123),BR_SCALAR(125),BR_SCALAR(123),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUVS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_SD_LR",
#else
		RasteriseMMX565_Z_UV_RGB_SD_RL,
		RasteriseMMX565_Z_UV_RGB_SD_LR,
		RasteriseMMX565_Z_UV_RGB_SD_RL,
		RasteriseMMX565_Z_UV_RGB_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_BLEND,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUVS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_S_LR",
#else
		RasteriseMMX565_Z_UV_RGB_S_RL,
		RasteriseMMX565_Z_UV_RGB_S_LR,
		RasteriseMMX565_Z_UV_RGB_S_RL,
		RasteriseMMX565_Z_UV_RGB_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured, Dithered", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(123),BR_SCALAR(125),BR_SCALAR(123),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_D_LR",
#else
		RasteriseMMX565_Z_UV_RGB_D_RL,
		RasteriseMMX565_Z_UV_RGB_D_LR,
		RasteriseMMX565_Z_UV_RGB_D_RL,
		RasteriseMMX565_Z_UV_RGB_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Textured", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_R|CM_G|CM_B|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(126),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgbuv),
		4,
		GenericSetupFloatZRGBUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RGB_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_LR",
                (void *)"_RasteriseMMX565_Z_UV_RGB_RL",
                (void *)"_RasteriseMMX565_Z_UV_RGB_LR",
#else
		RasteriseMMX565_Z_UV_RGB_RL,
		RasteriseMMX565_Z_UV_RGB_LR,
		RasteriseMMX565_Z_UV_RGB_RL,
		RasteriseMMX565_Z_UV_RGB_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B|CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_SD_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_SD_LR",
#else
		RasteriseMMX565_Z_UV_C_SD_RL,
		RasteriseMMX565_Z_UV_C_SD_LR,
		RasteriseMMX565_Z_UV_C_SD_RL,
		RasteriseMMX565_Z_UV_C_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B|CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_BLEND|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_S_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_S_LR",
#else
		RasteriseMMX565_Z_UV_C_S_RL,
		RasteriseMMX565_Z_UV_C_S_LR,
		RasteriseMMX565_Z_UV_C_S_RL,
		RasteriseMMX565_Z_UV_C_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured, Dithered", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_D_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_D_LR",
#else
		RasteriseMMX565_Z_UV_C_D_RL,
		RasteriseMMX565_Z_UV_C_D_LR,
		RasteriseMMX565_Z_UV_C_D_RL,
		RasteriseMMX565_Z_UV_C_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Textured", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_C_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_LR",
                (void *)"_RasteriseMMX565_Z_UV_C_RL",
                (void *)"_RasteriseMMX565_Z_UV_C_LR",
#else
		RasteriseMMX565_Z_UV_C_RL,
		RasteriseMMX565_Z_UV_C_LR,
		RasteriseMMX565_Z_UV_C_RL,
		RasteriseMMX565_Z_UV_C_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_SD_LR",
                (void *)"_RasteriseMMX565_Z_UV_SD_RL",
                (void *)"_RasteriseMMX565_Z_UV_SD_LR",
#else
		RasteriseMMX565_Z_UV_SD_RL,
		RasteriseMMX565_Z_UV_SD_LR,
		RasteriseMMX565_Z_UV_SD_RL,
		RasteriseMMX565_Z_UV_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_BLEND,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUVC_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_S_LR",
                (void *)"_RasteriseMMX565_Z_UV_S_RL",
                (void *)"_RasteriseMMX565_Z_UV_S_LR",
#else
		RasteriseMMX565_Z_UV_S_RL,
		RasteriseMMX565_Z_UV_S_LR,
		RasteriseMMX565_Z_UV_S_RL,
		RasteriseMMX565_Z_UV_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured, Dithered", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_D_LR",
                (void *)"_RasteriseMMX565_Z_UV_D_RL",
                (void *)"_RasteriseMMX565_Z_UV_D_LR",
#else
		RasteriseMMX565_Z_UV_D_RL,
		RasteriseMMX565_Z_UV_D_LR,
		RasteriseMMX565_Z_UV_D_RL,
		RasteriseMMX565_Z_UV_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Textured", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE,
	PRIMF_PALETTE|PRIMF_POWER2|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zuv),
		4,
		GenericSetupFloatZUV_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_UV_RL",
                (void *)"_RasteriseMMX565_Z_UV_LR",
                (void *)"_RasteriseMMX565_Z_UV_RL",
                (void *)"_RasteriseMMX565_Z_UV_LR",
#else
		RasteriseMMX565_Z_UV_RL,
		RasteriseMMX565_Z_UV_LR,
		RasteriseMMX565_Z_UV_RL,
		RasteriseMMX565_Z_UV_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_R|CM_G|CM_B,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_DITHER_COLOUR|PRIMF_BLEND,
	PRIMF_SMOOTH|PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGBS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_RGB_SD_LR",
                (void *)"_RasteriseMMX565_Z_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_RGB_SD_LR",
#else
		RasteriseMMX565_Z_RGB_SD_RL,
		RasteriseMMX565_Z_RGB_SD_LR,
		RasteriseMMX565_Z_RGB_SD_RL,
		RasteriseMMX565_Z_RGB_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_A,
		CM_SX|CM_SY|CM_R|CM_G|CM_B,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_A)|(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_BLEND,
	PRIMF_SMOOTH|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGBS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_RGB_S_LR",
                (void *)"_RasteriseMMX565_Z_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_RGB_S_LR",
#else
		RasteriseMMX565_Z_RGB_S_RL,
		RasteriseMMX565_Z_RGB_S_LR,
		RasteriseMMX565_Z_RGB_S_RL,
		RasteriseMMX565_Z_RGB_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour, Dithered", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_R|CM_G|CM_B,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_DITHER_COLOUR,
	PRIMF_SMOOTH|PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGB_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_RGB_D_LR",
                (void *)"_RasteriseMMX565_Z_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_RGB_D_LR",
#else
		RasteriseMMX565_Z_RGB_D_RL,
		RasteriseMMX565_Z_RGB_D_LR,
		RasteriseMMX565_Z_RGB_D_RL,
		RasteriseMMX565_Z_RGB_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Interpolated Colour", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_R|CM_G|CM_B,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_R)|(1<<C_G)|(1<<C_B),
		0,
		0,

		/* Constant slots
	 	 */
		0,
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH,
	PRIMF_SMOOTH,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGB_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_RL",
                (void *)"_RasteriseMMX565_Z_RGB_LR",
                (void *)"_RasteriseMMX565_Z_RGB_RL",
                (void *)"_RasteriseMMX565_Z_RGB_LR",
#else
		RasteriseMMX565_Z_RGB_RL,
		RasteriseMMX565_Z_RGB_LR,
		RasteriseMMX565_Z_RGB_RL,
		RasteriseMMX565_Z_RGB_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Dithered, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B|CM_A,
		CM_SX|CM_SY,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A)|(1<<C_SX)|(1<<C_SY),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_DITHER_COLOUR|PRIMF_BLEND|PRIMF_SMOOTH,
	PRIMF_DITHER_COLOUR|PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGBS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_RGB_SD_LR",
                (void *)"_RasteriseMMX565_Z_RGB_SD_RL",
                (void *)"_RasteriseMMX565_Z_RGB_SD_LR",
#else
		RasteriseMMX565_Z_RGB_SD_RL,
		RasteriseMMX565_Z_RGB_SD_LR,
		RasteriseMMX565_Z_RGB_SD_RL,
		RasteriseMMX565_Z_RGB_SD_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Screendoor", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE|BR_PRIMF_BLENDED,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B|CM_A,
		CM_SX|CM_SY,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A)|(1<<C_SX)|(1<<C_SY),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_A),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(1)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_BLEND|PRIMF_SMOOTH,
	PRIMF_BLEND,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGBS_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_RGB_S_LR",
                (void *)"_RasteriseMMX565_Z_RGB_S_RL",
                (void *)"_RasteriseMMX565_Z_RGB_S_LR",
#else
		RasteriseMMX565_Z_RGB_S_RL,
		RasteriseMMX565_Z_RGB_S_LR,
		RasteriseMMX565_Z_RGB_S_RL,
		RasteriseMMX565_Z_RGB_S_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour, Dithered", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(247),BR_SCALAR(251),BR_SCALAR(247),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_DITHER_COLOUR|PRIMF_SMOOTH,
	PRIMF_DITHER_COLOUR,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGB_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_RGB_D_LR",
                (void *)"_RasteriseMMX565_Z_RGB_D_RL",
                (void *)"_RasteriseMMX565_Z_RGB_D_LR",
#else
		RasteriseMMX565_Z_RGB_D_RL,
		RasteriseMMX565_Z_RGB_D_LR,
		RasteriseMMX565_Z_RGB_D_RL,
		RasteriseMMX565_Z_RGB_D_LR,
#endif
	}
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)GenericAutoloadThunk, NULL,
#else
		(brp_render_fn *)GenericSetupTriangleFloat_A, NULL,
#endif

		"MMX, RGB 565, Constant Colour", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_R)|(1<<C_G)|(1<<C_B),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(1),BR_SCALAR(0)},
	{BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(254),BR_SCALAR(0)},

	/* range flags
	 */
	RF_OFFSET_Y,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH,
	0,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,
	PMT_NONE,

	/* Colour & Depth  row size
	 */
	0,0,

	/* Texture size
	 */
	0,0,

	/* Input colour type
	 */
	0,

	/* Autoload info
	 */
#if AUTOLOAD
	"mmxf",(void *)GenericSetupTriangleFloat_A,
#else
	NULL,NULL,
#endif
	/* Generic setup info.
	 */
	{
		0.0f,
		sizeof(struct param_zrgb),
		4,
		GenericSetupFloatZRGB_A,

#if AUTOLOAD
                (void *)"_RasteriseMMX565_Z_RGB_RL",
                (void *)"_RasteriseMMX565_Z_RGB_LR",
                (void *)"_RasteriseMMX565_Z_RGB_RL",
                (void *)"_RasteriseMMX565_Z_RGB_LR",
#else
		RasteriseMMX565_Z_RGB_RL,
		RasteriseMMX565_Z_RGB_LR,
		RasteriseMMX565_Z_RGB_RL,
		RasteriseMMX565_Z_RGB_LR,
#endif
	}
},
