{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2TA24, NULL,
#endif

		"RGB 888, Z Buffered, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_SZ|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_U)|(1<<C_V),
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
	RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,
	PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
	BR_PMT_RGB_888,
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
	"softtzf",(void *)"_TriangleRenderPIZ2TA24",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2TIA_RGB_888, NULL,
#endif

		"RGB 888, Z Buffered, Constant Intensity, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE|BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		CM_I,
		CM_SX|CM_SY|CM_SZ|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_I)|(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_I),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtzf",(void *)"_TriangleRenderPIZ2TIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2TIA_RGB_888, NULL,
#endif

		"RGB 888, Z Buffered, Interpolated Intensity, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_SZ|CM_I|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_I)|(1<<C_U)|(1<<C_V),
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
	RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtzf",(void *)"_TriangleRenderPIZ2TIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2TA24, NULL,
#endif

		"RGB 888, Z Buffered, Textured", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_SZ|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_U)|(1<<C_V),
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
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_STRIDE_POSITIVE,
	PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
	BR_PMT_RGB_888,
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
	"softtzf",(void *)"_TriangleRenderPIZ2TA24",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2TIA_RGB_888, NULL,
#endif

		"RGB 888, Z Buffered, Interpolated Intensity, Textured", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_SZ|CM_I|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_I)|(1<<C_U)|(1<<C_V),
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
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtzf",(void *)"_TriangleRenderPIZ2TIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2TIA_RGB_888, NULL,
#endif

		"RGB 888, Z Buffered, Constant Intensity, Textured", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE,

		/* components - constant and per vertex
		 */
		CM_I,
		CM_SX|CM_SY|CM_SZ|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_I)|(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_I),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtzf",(void *)"_TriangleRenderPIZ2TIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2I_RGB_888, NULL,
#endif

		"RGB 888, Z Buffered, Interpolated Colour", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_SZ|CM_R|CM_G|CM_B,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_SZ)|(1<<C_R)|(1<<C_G)|(1<<C_B),
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
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH,
	PRIMF_SMOOTH,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
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
	"softtzf",(void *)"_TriangleRenderPIZ2I_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPIZ2_RGB_888, NULL,
#endif

		"RGB 888, Z Buffered, Constant Colour", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		CM_R|CM_G|CM_B,
		CM_SX|CM_SY|CM_SZ,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_R)|(1<<C_G)|(1<<C_B)|(1<<C_SX)|(1<<C_SY)|(1<<C_SZ),
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
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH,
	0,

	/* Texture, depth and shade type
	 */
	BR_PMT_DEPTH_16,
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
	"softtzf",(void *)"_TriangleRenderPIZ2_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPITA24, NULL,
#endif

		"RGB 888, Textured, Perspective Correct", NULL,
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
	RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,
	PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_RGB_888,
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
	"softtf",(void *)"_TriangleRenderPITA24",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPITIA_RGB_888, NULL,
#endif

		"RGB 888, Constant Intensity, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE|BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		CM_I,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_I)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_I),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtf",(void *)"_TriangleRenderPITIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPITIA_RGB_888, NULL,
#endif

		"RGB 888, Interpolated Intensity, Textured, Perspective Correct", NULL,
		BRT_TRIANGLE, BR_PRIMF_SUBDIVIDE,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_I|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_I)|(1<<C_U)|(1<<C_V),
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
	RF_NEED_SUBDIVIDE,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_PERSPECTIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtf",(void *)"_TriangleRenderPITIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPITA24, NULL,
#endif

		"RGB 888, Textured", NULL,
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
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_STRIDE_POSITIVE,
	PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_RGB_888,
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
	"softtf",(void *)"_TriangleRenderPITA24",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPITIA_RGB_888, NULL,
#endif

		"RGB 888, Interpolated Intensity, Textured", NULL,
		BRT_TRIANGLE, 0,

		/* components - constant and per vertex
		 */
		0,
		CM_SX|CM_SY|CM_I|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_SX)|(1<<C_SY)|(1<<C_I)|(1<<C_U)|(1<<C_V),
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
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE,
	PRIMF_SMOOTH|PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtf",(void *)"_TriangleRenderPITIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPITIA_RGB_888, NULL,
#endif

		"RGB 888, Constant Intensity, Textured", NULL,
		BRT_TRIANGLE, BR_PRIMF_CONST_DUPLICATE,

		/* components - constant and per vertex
		 */
		CM_I,
		CM_SX|CM_SY|CM_U|CM_V,

		/* Component slots as - float, fixed or integer
		 */
		(1<<C_I)|(1<<C_SX)|(1<<C_SY)|(1<<C_U)|(1<<C_V),
		0,
		0,

		/* Constant slots
	 	 */
		(1<<C_I),
	},

	/* Offset and scale for R,G,B,A
	 */
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},
	{BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0),BR_SCALAR(0)},

	/* range flags
	 */
	0,

	/* Work buffer
	 */
	&work,

	/* Masks
	 */
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE|PRIMF_SMOOTH,
	PRIMF_MODULATE|PRIMF_STRIDE_POSITIVE,

	/* Texture, depth and shade type
	 */
	PMT_NONE,
	BR_PMT_INDEX_8,
	BR_PMT_RGB_888,
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
	"softtf",(void *)"_TriangleRenderPITIA_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPII_RGB_888, NULL,
#endif

		"RGB 888, Interpolated Colour", NULL,
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
	0,

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
	"softtf",(void *)"_TriangleRenderPII_RGB_888",
#else
	NULL,NULL,
#endif
},
{
	{
		/* Render function
		 */
#if AUTOLOAD
		(brp_render_fn *)RenderAutoloadThunk, NULL,
#else
		(brp_render_fn *)TriangleRenderPI_RGB_888, NULL,
#endif

		"RGB 888, Constant Colour", NULL,
		BRT_TRIANGLE, 0,

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
	0,

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
	"softtf",(void *)"_TriangleRenderPI_RGB_888",
#else
	NULL,NULL,
#endif
},
