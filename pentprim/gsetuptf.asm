; Copyright (c) 1992,1993-1996 Argonaut Technologies Limited. All rights reserved.
;
; $Id: gsetuptf.asm 1.3 1996/12/13 17:51:54 sam Exp $
; $Locker: $
;
; Generic triangle setup code from float component vertices
;
; Uses information in the primitive block to decide what parameters
; should be setup and what rasterisers to use.
;
		.586p
		.model	flat,c
				include drv.inc
				include rastmacs.inc
				include locmacs.inc
				include setupdat.inc

		.code
		extrn	RasteriseBufferFlush:proc

; Do all the per-triangle work for a single float parameter
;
; Optionally, remap value to unsigned by flipping sign bit of
; starting value.
;
; Optionally, prescale value by constant or interpolated alpha
;
;	eax: ptr to top vertex
;
;	edx: ptr to vertex0
;	esi: ptr to vertex1
;	edi: ptr to vertex2
;
;	ebp: ptr to param block
;
;
SETUP_FLOAT_PARAM	macro	param,s_p,d_p_x,conv, unsigned:=<0>,alpha:=<<no>>

		; work out parameter deltas
		;
		; dp1 = param_1 - param_0
		; dp2 = param_2 - param_0
		;
		; 4 cycles
		;
ifidni alpha,<no>
									;	0		1		2		3		4		5		6		7
				fld		[edi].param		;	p2
				fsub	[edx].param		;	dp2

				fld		[esi].param		;	p1		dp2
				fsub	[edx].param		;	dp1		dp2
elseifidni alpha, <const>
				fld		[edi].param		;	p2
				fsub	[edx].param		;	dp2

				fld		[esi].param		;	p1		dp2
				fsub	[edx].param		;	dp1		dp2
				 fxch	st(1)           ;	dp2		dp1
				fmul	[edx]._a		;	dp2*a	dp1
				 fxch	st(1)			;	dp1		dp2*a
; 1 cycle

				fmul	[edx]._a		;	dp1*a	dp2*a
elseifidni alpha, <interp>
 				fld		[edx].param		;	p0
				fmul	[edx]._a		;	p0*a0
				fld		[edi].param		;	p2		p0*a0
				fmul	[edi]._a		;	p2*a2   p0*a0
				fld		[esi].param		;	p1		p2*a2	p0*a0
				fmul	[esi]._a		;	p1*a1	p2*a2	p0*a0
				 fxch	st(1)			;	p2*a2	p1*a1	p0*a0
				fsub	st(2)			;	dp2     p1*a1	p0*a0
				 fxch	st(1)			;	p1*a1	dp2     p0*a0
				fsubp	st(2)			;	dp1		dp2
endif


		; Multiply deltas by precomputed values to get x & y gradients
		; (Also interleaved load of parameter start and fractional x & y offsets of start position)
		;
		; pdx = dp1 * dy2_a - dp2 * dy1_a
		; pdy = dp2 * dx1_a - dp1 * dx2_a
		;
		; 11 cycles
		;
								        ;	0		1		2		3		4		5		6		7
				fld		st(1)			;	dp2		dp1		dp2
				fmul	dy1_a			;	dp2*a	dp1		dp2
				fld		st(1)			;	dp1		dp2*a	dp1		dp2
				fmul	dy2_a			;	dp1*b	dp2*a	dp1		dp2
				fld		t_dx			;	fdx		dp1*b	dp2*a	dp1		dp2
				 fxch	st(4)			;	dp2		dp1*b	dp2*a	dp1		fdx
				fmul	dx1_a			;	dp2*c	dp1*b	dp2*a	dp1		fdx
				fld		t_dy			; 	fdy		dp2*c	dp1*b	dp2*a	dp1		fdx
				 fxch	st(4)			; 	dp1		dp2*c	dp1*b	dp2*a	fdy		fdx
				fmul	dx2_a			; 	dp1*d	dp2*c	dp1*b	dp2*a	fdy		fdx
				 fxch	st(3)			; 	dp2*a	dp2*c	dp1*b	dp1*d	fdy		fdx
				fsubp	st(2),st		; 	dp2*c	d1b-d2a	dp1*d	fdy		fdx
				fld		[eax].param		; 	param_t	dp2*c	d1b-d2a	dp1*d	fdy		fdx

ifidni alpha,<const>
				fmul	[edx]._a
elseifidni alpha, <interp>
				fmul	[eax]._a
endif
				 fxch	st(3)			; 	dp1*d	dp2*c	d1b-d2a	param_t	fdy		fdx
				fsubp	st(1),st		; 	d2c-d1d	d1b-d2a	param_t	fdy		fdx
										; 	pdy		pdx		param_t	fdy		fdx

		; Build the inputs to the rasteriser
		;
		; pdy_0 = pdy + xstep_0 * pdx
		; pdy_1 = pdy + xstep_1 * pdx
		; pstart = param_t + pdx * fdx + pdy * fdy
		;
		; (A couple of the fixed points convertions are interleaved into this block)
		; 12 cycles
		;
										;	0		1		2		3		4		5		6		7
				fld		st(1)			; 	pdx		pdy		pdx		param_t	fdy		fdx
				fmul	xstep_0			; 	pdx*xs0	pdy		pdx		param_t	fdy		fdx
				fld		st(2)			; 	pdx		pdx*xs0	pdy		pdx		param_t	fdy		fdx
				fmul	xstep_1			; 	pdx*xs1	pdx*xs0	pdy		pdx		param_t	fdy		fdx
				 fxch	st(1)			; 	pdx*xs0	pdx*xs1	pdy		pdx		param_t	fdy		fdx
				fadd	st,st(2)		; 	pdy_0	pdx*xs1	pdy		pdx		param_t	fdy		fdx
				 fxch	st(3)			; 	pdx		pdx*xs1	pdy		pdy_0	param_t	fdy		fdx

				fmul	st(6),st		; 	pdx		pdx*xs1	pdy		pdy_0	param_t	fdy		fdx*pdx
				 fxch	st(2)			; 	pdy		pdx*xs1	pdx		pdy_0	param_t	fdy		fdx*pdx
				fadd	st(1),st		; 	pdy		pdy_1	pdx		pdy_0	param_t	fdy		fdx*pdx

				fmulp	st(5),st		; 	pdy_1	pdx		pdy_0	param_t	fdy*pdy	fdx*pdx
				 fxch	st(3)			; 	param_t	pdx		pdy_0	pdy_1	fdy*pdy	fdx*pdx
				faddp	st(5),st		; 	pdx		pdy_0	pdy_1	fdy*pdy	fpx+pt
				 fxch	 st(1)			; 	pdy_0	pdx		pdy_1	fdy*pdy	fpx+pt
				fadd	conv			; 	C+pdy_0	pdx		pdy_1	fdy*pdy	fpx+pt
				 fxch	 st(2)			; 	pdy_1	pdx		C+pdy_0	fdy*pdy	fpx+pt
				fadd	conv			; 	C+pdy_1	pdx		C+pdy_0	fdy*pdy	fpx+pt
				 fxch	st(3)			; 	fdy*pdy	pdx		C+pdy_0	C+pdy_1	fpx+pt
				faddp	st(4),st		;	pdx		C+pdy_0	C+pdy_1	pstart


		; Convert to fixed point, pack and store in output block
		;
		; tsb->d_p_y0 = convert(pdy_0)
		; tsb->d_p_y1 = convert(pdy_1)
		; tsb->d_p_x = convert(pdx)
		; tsb->s_p = convert(pstart)
		;
		; 13 cycles
									;	0		1		2		3		4		5		6		7
				fadd	conv			; 	C+pdx	C+pdy_0	C+pdy_1	pstart
				 fxch	st(3)			; 	pstart	C+pdy_0	C+pdy_1	C+pdx

; 1 Cycle

				fadd	conv			; 	C+pstrt	C+pdy_0	C+pdy_1	C+pdx
				 fxch	st(2)			; 	C+pdy_1	C+pdy_0	C+pstrt	C+pdx

				fstp	real8 ptr s_p
				fstp	real8 ptr d_p_x
				mov		ebx,dword ptr s_p
				mov		ecx,dword ptr d_p_x

				fstp	real8 ptr s_p	;
				fstp	real8 ptr d_p_x	;
				mov		dword ptr s_p+4,ebx
				mov		dword ptr d_p_x+4,ecx

		; Change from -1 to 1 signed to 0 to 1 unsigned
		;
if unsigned
				mov		ebx,dword ptr s_p
				xor		ebx,080000000h
				mov		dword ptr s_p,ebx
endif

		endm

; Set texture mapping info
;
SETUP_TEXTURE_INFO	macro
				mov		ebx,work.texture.base
				mov		ecx,work.texture.palette

				mov		PARAM.tinfo.texture_address,ebx
				mov		PARAM.tinfo.map_address,ecx

				mov		ebx,dword ptr work.texture.width_s
				mov		dword ptr PARAM.tinfo.width_s,ebx
		endm

; Set constant colour from vertex 0
;
SETUP_FLOAT_CONSTANT_COLOUR	macro

												;	0		1		2		3		4		5		6		7
				fld			[edx]._a
				fadd		fp_conv_s8			;	a+C
				fld			[edx]._r
				fadd		fp_conv_s			;	r+C		a+C
				fld			[edx]._g
				fadd		fp_conv_s			;	g+C		r+C		a+C
				fld			[edx]._b
				fadd		fp_conv_s			;	b+C		g+C		r+C		a+C
				 fxch		st(1) 				;	g+C		b+C		r+C		a+C

				fstp		c_g
				fstp		c_b
				fstp		c_r
				fstp		c_a

				mov			ebx,c_b
				mov			ecx,c_g

				mov         byte ptr PARAM.h._c+0,bl

				mov         byte ptr PARAM.h._c+1,cl

				mov			ebx,c_r
				mov			ecx,c_a

				mov         byte ptr PARAM.h._c+2,bl

				mov         byte ptr PARAM.h._c+3,cl

		endm

; Copy constant alpha to parameter block
;
SETUP_FLOAT_CONSTANT_ALPHA	macro

												;	0		1		2		3		4		5		6		7
				fld			[edx]._a
				fadd		fp_conv_s8			;	a+C

				fstp		c_a

				mov			ebx,c_a
				shl			ebx,24
				mov			PARAM.h._c+0,ebx

		endm

; void BR_ASM_CALL GenericSetupTriangleFloat_A(struct local_block *pb, brp_vertex *v0,brp_vertex *v1,brp_vertex *v2)
;
; Setup a triangle given floating point components. Uses the generic_setup_info in
; the supplied parameter block to:
;
; Allocate a parameter block in the rasterise buffer (Flushing if necessary)
; Do commong trinagle setup
; Choose a rasteriser (LR/RL, Small/Large)
; Generate start/deltas for parameters (setup_info has pointer to specific setup code)
;
GenericSetupTriangleFloat_A proc

		; Use ESP to access stack frame
		;
		LOCALS_BEGIN
			PARAM_DWORD lblock
v0_offset equ _param_size
	        PARAM_DWORD v0
	        PARAM_DWORD v1
    	    PARAM_DWORD v2

			LOCAL_DWORD	dx1_a
			LOCAL_DWORD	dx2_a
			LOCAL_DWORD	dy1_a
			LOCAL_DWORD	dy2_a

			LOCAL_DWORD	xstep_1
			LOCAL_DWORD	xstep_0

			LOCAL_DWORD	t_dx
			LOCAL_DWORD_ALIAS m_y
			LOCAL_DWORD	t_dy
			LOCAL_DWORD	t_y

			LOCAL_DWORD	flip
			LOCAL_DWORD	iarea

			LOCAL_DWORD	c_b
			LOCAL_DWORD	c_g
			LOCAL_DWORD	c_r
			LOCAL_DWORD	c_a
   		LOCALS_END

	   ; Load up vertex pointers and check that there is space in rasterise buffer
	   ;
				mov			ecx,rasteriseBufferTop
				mov			eax,lblock

	   			mov			edx,v2
    	        LOCALS_CREATE

				sub			ecx,[eax].local_block.setup.param_size
				mov			eax,v0

				cmp			ecx,offset rasteriseBuffer
				jae			no_flush

		; Flush stacked triangles - have to do this before setup starts, as MMX rasterisers will
		; trash the FPU stack
		;
				call		RasteriseBufferFlush

				mov			eax,lblock

				mov			ecx,rasteriseBufferTop
   				mov			edx,v2

				sub			ecx,[eax].local_block.setup.param_size
				mov			eax,v0

no_flush:		mov			rasteriseBufferTop,ecx
				mov			ecx,v1

		assume eax: ptr brp_vertex, ebx:ptr brp_vertex, ecx: ptr brp_vertex, edx: ptr brp_vertex

		; Calculate area of triangle and generate dx1/2area, dx1/2area, dx1/2area and dx1/2area
		;
        ; Also sort the vertices in Y order whilst divide is happening
        ;
											;	0		1		2		3		4		5		6		7
		        fld			[edx]._x		;	x2
			    fsub		[eax]._x		;	dx2
		        fld			[ecx]._x		;	x1		dx2
			    fsub		[eax]._x		;	dx1		dx2
		        fld			[edx]._y		;	y2		dx1		dx2
			    fsub		[eax]._y		;	dy2		dx1		dx2
		        fld			[ecx]._y		;	y1		dy2		dx1		dx2
			    fsub		[eax]._y		;	dy1		dy2		dx1		dx2

		        fld			st(2)			;	dx1		dy1		dy2		dx1		dx2
			    fmul		st,st(2)		;	dx1*dy2	dy1		dy2		dx1		dx2

				fld			st(4)			;	dx2		dx1*dy2	dy1		dy2		dx1		dx2
				fmul		st,st(2)		;	dx2*dy1	dx1*dy2	dy1		dy2		dx1		dx2

    	        LPUSH       ebx
				mov			eax,[eax]._y

    	        LPUSH       ebp
				mov			ecx,[ecx]._y

				fsubp		st(1),st		;	2area	dy1		dy2		dx1		dx2

				xor			ebx,ebx
				cmp			ecx,eax

				rcl			ebx,1
				mov			edx,[edx]._y

				fdivr		fp_one			;	1/2area	dy1		dy2		dx1		dx2

    	        LPUSH       edi
				cmp			edx,eax

				rcl			ebx,1
				cmp			edx,ecx

				rcl			ebx,1			; ebx now has 3 bit number characterising the order of the vertices.
    	        LPUSH       esi

;U
;V
				mov			eax,sort_table_0[ebx*4]
				mov			edx,sort_table_2[ebx*4]

				mov			esi,flip_table[ebx*4]
				mov			ebx,sort_table_1[ebx*4]

	    ; Load eax,ebx,edx with pointers to the three vertices in vertical order
	    ;
				mov			eax,[v0_offset+_param_offset+esp+eax]
				mov			edx,[v0_offset+_param_offset+esp+edx]

				mov			ebx,[v0_offset+_param_offset+esp+ebx]
				mov			flip,esi

	    ; Work out Y extents of triangle
		;
	    ; Convert float to int using integer instructions, because FPU is in use doing division
	    ;
		; NB: this convertion only works if the number is greater than 0 and less than 1<<24
		;
MASK_MANTISSA   equ 007fffffh
IMPLICIT_ONE    equ 1 shl 23
EXPONENT_OFFSET equ ((127+23) shl 23) or 07fffffh

	            mov         ebp,[eax]._y
    	        mov         ecx,EXPONENT_OFFSET

        	    sub         ecx,ebp				; Offset exponent to get shift value
	            and         ebp,MASK_MANTISSA	; Mask out mantissa

    	        shr         ecx,23				; Move shift value to low bits
	            or         	ebp,IMPLICIT_ONE	; Put the 1 back in top of mantissa

    	        shr         ebp,cl				; EBP = y_t

        	    mov         esi,[ebx]._y
	            mov         ecx,EXPONENT_OFFSET

    	        sub         ecx,esi
        	    and         esi,MASK_MANTISSA

	            shr         ecx,23
    	        or         	esi,IMPLICIT_ONE

        	    shr         esi,cl				; ESI = y_m

	            mov         edi,[edx]._y
    	        mov         ecx,EXPONENT_OFFSET

        	    sub         ecx,edi
            	and         edi,MASK_MANTISSA

	            shr         ecx,23
    	        or         	edi,IMPLICIT_ONE

        	    shr         edi,cl				; EDI = y_b

        ; Catch special cases of empty top or bottom trapezoids
        ;
				cmp			ebp,esi
				je			top_zero

				cmp			esi,edi
				je			bottom_zero

		; Parameter gradient startup and Y deltas for edge gradients
		;
											;	0		1		2		3		4		5		6		7
				fmul		st(1),st		;	1/2area	dy1*a	dy2		dx1		dx2

				fld			[ebx]._y		;	sy2    	1/2area	dy1*a	dy2		dx1		dx2
				fsub		[eax]._y		;   dsy1    1/2area	dy1*a	dy2		dx1		dx2
	             fxch       st(3)           ;   dy2   	1/2area	dy1*a	dsy1	dx1		dx2

				fmul		st,st(1)		;	dy2*a  	1/2area	dy1*a	dsy1	dx1		dx2

				fld			[edx]._y		;   sy3     dy2*a  	1/2area	dy1*a	dsy1	dx1		dx2
				fsub		[ebx]._y		;   dsy2    dy2*a  	1/2area	dy1*a	dsy1	dx1		dx2
    	         fxch       st(5)           ;   dx1	    dy2*a  	1/2area	dy1*a	dsy1	dsy2	dx2

		; top_zero and bottom_zero special cases re-enter here
		;
count_cont:
				fmul		st,st(2)		;	dx1*a   dy2*a  	1/2area	dy1*a	dsy1	dsy2	dx2

				fld			[edx]._y		;   sy3     dx1*a   dy2*a  	1/2area	dy1*a	dsy1	dsy2	dx2
				fsub		[eax]._y		;   dsy3    dx1*a   dy2*a  	1/2area	dy1*a	dsy1	dsy2	dx2
	             fxch       st(7)			;   dx2		dx1*a   dy2*a  	1/2area	dy1*a	dsy1	dsy2	dsy3

				fmul		st,st(3)		;   dx2*a	dx1*a   dy2*a  	1/2area	dy1*a	dsy1	dsy2	dsy3
    	         fxch		st(3)			;   1/2area	dx1*a   dy2*a  	dx2*a	dy1*a	dsy1	dsy2	dsy3

				fstp		iarea
				fstp		dx1_a
				fstp		dy2_a
				fstp		dx2_a
				fstp		dy1_a			;  	dy1		dy2		dy3


		; Find edge gradients of triangle
		;
		; Uses 1 reciprocal for all three edges:
		;
		; R = 1/(dy1.dy2.dy3)
		;
		; gradient_major = dy1.dx2.dy3.R
		; gradient_minor1 = dx1.dy2.dy3.R
		; gradient_minor2 = dy1.dy2.dx3.R
		;
		; This could be dodgy - since it is possible for the gradients on two sides
		; of an edge to wind up being different. In practice, this has not proved
		; to be a problem (yet)
		;
											;	0		1		2		3		4		5		6		7
				fld			st(2)			;	dy3		dy1		dy2		dy3
				fmul		st,st(2)		;	dy2*dy3	dy1		dy2		dy3

				fld			[ebx]._x		;	x2		dy2*dy3	dy1		dy2		dy3
				fsub		[eax]._x		;	dx1		dy2*dy3	dy1		dy2		dy3

				fld			st(1)			;	dy2*dy3 dx1		dy2*dy3	dy1		dy2		dy3
				fmul		st,st(3)		;	dy123	dx1		dy2*dy3	dy1		dy2		dy3

				fld			[edx]._x		;	x3		dy123	dx1		dy2*dy3	dy1		dy2		dy3
				fsub		[ebx]._x		;	dx2		dy123	dx1		dy2*dy3	dy1		dy2		dy3
				 fxch		 st(2)			;	dx1		dy123	dx2		dy2*dy3	dy1		dy2		dy3
				fld			fp_one			;	1.0		dx1		dy123	dx2		dy2*dy3	dy1		dy2		dy3
				fdivrp		st(2),st		;	dx1		R		dx2		dy2*dy3	dy1		dy2		dy3

		; Generate counts
		;
				inc			ebp
				mov			ecx,esi

				sub			ecx,ebp			;  count_t = (y_m-y_t)-1
				mov			t_y,ebp			; save for X intercept calcs

				shl			ecx,16
				inc			esi

				sub			edi,esi			;  count_b = (y_b-y_m)-1
				mov			m_y,esi			; save for X intercept calcs

				and			edi,0ffffh      ; Count can go negative
				mov			ebp,rasteriseBufferTop	; Fetch pointer to parameter block

		PARAMH	equ [ebp].tsb_header

				or			ecx,edi			; combined counts
				mov			edi,t_y

			; Setup stride and screen pointers
			;
			; XXX Some write buffer stalls here - could touch the cache line(s) in advance
			;
				mov			PARAMH.counts,ecx
				dec			edi					; Input are offset by one scanline (to avoid FP convertion problems)

	            mov			esi,lblock
				mov			PARAMH.start_scanline,edi

				mov			edi,work.colour.stride_b
				mov			ecx,work.depth.stride_b

				mov			PARAMH.screen_stride,edi
				mov			PARAMH.depth_stride,ecx

	   			mov			edi,work.colour.base
				mov			ecx,work.depth.base

				mov			PARAMH.screen_address,edi
				mov			edi,iarea

				mov			PARAMH.depth_address,ecx
				and			edi,7fffffffh				; Clear sign bit of area

		; Setup rasterisation function, generate an index using area comparison and LR bit
		;
		; Generate LR/RL flag into ecx (also used to index convertion numbers below)
		;
				cmp			edi,[esi].local_block.setup.iarea_limit
				mov			ecx,flip

				sbb			edi,edi
				xor			ecx,iarea			; Build LR flag in bit 31

				shr			ecx,31				; move down to bit 0
				add			edi,edi

				sub			edi,ecx				; edi now has -3 = RL_S, -2 = LR_S , -1 = RL_L, 0 = LR_L

				mov			edi,[esi+edi*4].local_block.setup.rasterise_lr_s

				mov			PARAMH.function, edi

				mov			edi,[esi].local_block.setup.stride ; Get stride for rasteriser

				neg			edi

			; Finish of gradient calculations, interleaved with working out t_dy, and m_dy, the fractions
			; that the top and middle vertices are from the integer scanline boundaries
			;
			; t_dy = (yt+1) - vt->y
			; m_dy = (ym+1) - vm->y
			;
			; eax - Vertex 0
			; ebx - Vertex 1
			; edx - Vertex 2
			; ecx - L/R flag
			; esi - Pointer to setup block
			; edi - negative stride
			; ebp - Pointer to parameter block header
			;
											;	0		1		2		3		4		5		6		7
				fmulp		st(3),st		;	R		dx2		XYY		dy1		dy2		dy3

				fld			[edx]._x		;	x3		R		dx2		XYY		dy1		dy2		dy3
				 fxch		st(3)			;	XYY		R		dx2		x3		dy1		dy2		dy3

				fmul		st,st(1)		;	XYY*R	R		dx2		x3		dy1		dy2		dy3
				 fxch		st(3)			;	x3		R		dx2		XYY*R	dy1		dy2		dy3

				fsub		[eax]._x		;	dx3		R		dx2		XYY*R	dy1		dy2		dy3
				 fxch		st(1)			;	R		dx3		dx2		XYY*R	dy1		dy2		dy3

				fmulp		st(4),st		;	dx3		dx2		XYY*R	dy1*R	dy2		dy3
				 fxch		st(2)			;	XYY*R	dx2		dx3		dy1*R	dy2		dy3

				fild		m_y				;	m_y		XYY*R	dx2		dx3		dy1*R	dy2		dy3
				 fxch		st(2)			;	dx2		XYY*R	m_y		dx3		dy1*R	dy2		dy3

				fmulp		st(6),st		;	XYY*R	m_y		dx3		dy1*R	dy2		dx2*dy3

				fild		t_y				;	t_y		XYY*R	m_y		dx3		dy1*R	dy2		dx2*dy3
				 fxch		st(3)			;	dx3		XYY*R	m_y		t_y		dy1*R	dy2		dx2*dy3

				fmulp		st(5),st		;	XYY*R	m_y		t_y		dy1*R	dy2*dx3	dx2*dy3
				 fxch		st(1)			;	m_y		XYY*R	t_y		dy1*R	dy2*dx3	dx2*dy3

				fsub		[ebx]._y		;	m_dy	XYY*R	t_y		dy1*R	dy2*dx3	dx2*dy3
				 fxch		st(3)			;	dy1*R	XYY*R	t_y		m_dy	dy2*dx3	dx2*dy3

				fmul		st(4),st		;	dy1*R	XYY*R	t_y		m_dy	YYX*R	dx2*dy3
				 fxch		st(2)			;	t_y		XYY*R	dy1*R	m_dy	YYX*R	dx2*dy3

				fsub		[eax]._y		;	t_dy	XYY*R	dy1*R	m_dy	YYX*R	dx2*dy3
				 fxch		st(2)			;	dy1*R	XYY*R	t_dy	m_dy	YYX*R	dx2*dy3

				fmulp		st(5),st		;	XYY*R	t_dy	m_dy	YYX*R	YXY*R
				 fxch		st(2)			;	m_dy	t_dy	XYY*R	YYX*R	YXY*R
											;	m_dy	t_dy	g1		gm		g2

	    ; Work out initial X intercepts with top and middle scanlines
	    ;
		; x_major  = gm * t_dy + vt->x
		; x_minor1 = g1 * t_dy + vt->x
		; x_minor2 = g2 * m_dy + vm->x
		;
											;	0		1		2		3		4		5		6		7
				fld			st(1)			;	t_dy	m_dy	t_dy	g1		gm		g2
				fxch		st(1)			;	m_dy	t_dy	t_dy	g1		gm		g2


				fmul		st,st(5)		;	m_dy*g2	t_dy	t_dy	g1		gm		g2
				fxch		st(2)			;	t_dy	t_dy	m_dy*g2	g1		gm		g2

				fst			t_dy

				fmul		st,st(3)		;	t_dy*g1	t_dy	m_dy*g2	g1		gm		g2
				 fxch		st(2)			;	m_dy*g2	t_dy	t_dy*g1	g1		gm		g2

				fadd		[ebx]._x		;	x_2		t_dy	t_dy*g1	g1		gm		g2
				 fxch		st(1)			;	t_dy	x_2		t_dy*g1	g1		gm		g2

				fmul		st,st(4)		;	t_dy*gm	x_2		t_dy*g1	g1		gm		g2
				 fxch		st(2)			;	t_dy*g1	x_2		t_dy*gm	g1		gm		g2

				fadd		[eax]._x		;	x_1		x_2		t_dy*gm	g1		gm		g2
				 fxch		st(3)			;	g1		x_2		t_dy*gm	x_1		gm		g2

				fadd		fp_conv_d16		;	g1+C	x_2		t_dy*gm	x_1		gm		g2
	   			 fxch		st(2)			;	t_dy*gm	x_2		g1+C	x_1		gm		g2

			    fadd		[eax]._x		;	x_m		x_2		g1+C	x_1		gm		g2
			     fxch		st(4)			;	gm		x_2		g1+C	x_1		x_m		g2

			    fadd		fp_conv_d16		;	gm+C	x_2		g1+C	x_1		x_m		g2
			     fxch		st(1)			;	x_2		gm+C	g1+C	x_1		x_m		g2

			    fadd	fconv_d16_12[ecx*8]	;	x_2+C	gm+C	g1+C	x_1		x_m		g2
			     fxch		st(5)			;	g2		gm+C	g1+C	x_1		x_m		x_2+C

			    fadd		fp_conv_d16		;	g2+C	gm+C	g1+C	x_1		x_m		x_2+C
			     fxch		st(2)			;	g1+C	gm+C	g2+C	x_1		x_m		x_2+C

			    fstp real8 ptr PARAMH.x1	;	gm+C	g2+C	x_1		x_m		x_2+C
			    fstp real8 ptr PARAMH.xm	;	g2+C	x_1		x_m		x_2+C
			    fstp real8 ptr PARAMH.x2	;	x_1		x_m		x_2+C

			    fadd	fconv_d16_12[ecx*8]	;	x_1+C	x_m		x_2+C
			    fxch		st(1)			;	x_m		x_1+C	x_2+C

			    fadd	fconv_d16_m[ecx*8]	;	x_m+C	x_1+C	x_2+C

		; Load deltas back in registers
		;
			    mov			edx,PARAMH.xm	; read fixed d_xm
			    mov			ecx,PARAMH.x1	; read fixed d_x1

			    fstp real8 ptr PARAMH.xm	;	x_1+C	x_2+C
			    fstp real8 ptr PARAMH.x1	;	x_2+C

			    mov			PARAMH.x1+4,ecx
			    mov			ecx,PARAMH.xm

			    sar			ecx,16
			    mov			PARAMH.xm+4,edx

			    sar			edx,16			; get integer part of x delta down major edge
			    and			ecx,edi

			    and			edx,edi			; Truncate x delta to nearest BOUNDARY pixels
			    mov			t_dx,ecx

			    fild		t_dx			;	t_x		x_2+C

		; Generate floating point versions of x delta and x delta+BOUNDARY
		;
			    mov			xstep_0,edx
			    sub			edx,edi

			    mov			ecx,PARAMH.x2	; read fixed d_x2
			    mov			xstep_1,edx
										    ;	0		1		2		3		4		5		6		7
			    fsub		[eax]._x		;	t_dx	x_2+C
			     fxch		st(1)			;	x_2+C	t_dx
			    fstp real8 ptr PARAMH.x2	;	t_dx

			    fild		xstep_0			;	xstep_0	t_dx
			    fild		xstep_1			;	xstep_1 xstep_0	t_dx
			     fxch		st(2)			;	tdx		xstep_0	xstep_1
			    fstp		t_dx			;	xstep_0	xstep_1

			    mov			PARAMH.x2+4,ecx
			    mov			ebx,[esi].local_block.setup.setup_param ; get pointer to parameter setup code

			    fstp		xstep_0			;	xstep_1
			    fstp		xstep_1			;

		; Jump to parameter specific setup code
		;
		; eax = top vertex
		;
		; edx = v0
		; esi = v1
		; edi = v2
		; ebp = pointer to parameter block
		;
		; ebc & ecx are scratch
		;
	            mov     	edx,v0
    	        mov     	esi,v1

				mov			edi,v2
				jmp			ebx

; Special cases for top or bottom counts == 0
;
top_zero:		cmp			ebp,edi			; Check for completely empty triangle
		   		je			empty_triangle
											;	0		1		2		3		4		5		6		7
				fmul		st(1),st		;	1/2area	dy1*a	dy2		dx1		dx2

				fld			fp_one			;	1.0    	1/2area	dy1*a	dy2		dx1		dx2
        	     fxch       st(3)           ;   dy2   	1/2area	dy1*a	1.0		dx1		dx2

				fmul		st,st(1)		;	dy2*a  	1/2area	dy1*a	1.0		dx1		dx2

				fld			[edx]._y		;   sy3     dy2*a  	1/2area	dy1*a	1.0		dx1		dx2
				fsub		[ebx]._y		;   dsy2    dy2*a  	1/2area	dy1*a	1.0		dx1		dx2
	             fxch       st(5)           ;   dx1	    dy2*a  	1/2area	dy1*a	1.0		dsy2	dx2

				jmp			count_cont

bottom_zero:								;	0		1		2		3		4		5		6		7
		   		fmul		st(1),st		;	1/2area	dy1*a	dy2		dx1		dx2

				fld			[ebx]._y		;	sy2    	1/2area	dy1*a	dy2		dx1		dx2
			    fsub		[eax]._y		;   dsy1    1/2area	dy1*a	dy2		dx1		dx2
                 fxch       st(3)           ;   dy2   	1/2area	dy1*a	dsy1	dx1		dx2

			    fmul		st,st(1)		;	dy2*a  	1/2area	dy1*a	dsy1	dx1		dx2

			    fld			fp_one			;   1.0     dy2*a  	1/2area	dy1*a	dsy1	dx1		dx2
                 fxch       st(5)           ;   dx1	    dy2*a  	1/2area	dy1*a	dsy1	1.0		dx2

			    jmp			count_cont

		; Triangle has no pixels:
		;
		; Back out the buffer allocation, clear the FPU stack and return
		;
		assume eax:nothing, ebx:nothing, ecx:nothing, edx:nothing

empty_triangle:	mov			eax,rasteriseBufferTop
				mov			ecx,lblock

			    fstp		st(0)

			    add			eax,[ecx].local_block.setup.param_size
			    pop			esi

			    fstp		st(0)

           	    mov			rasteriseBufferTop,eax
			    pop			edi

			    fstp		st(0)
			    fstp		st(0)
			    fstp		st(0)

			    pop			ebp
			    pop			ebx

			    LOCALS_DESTROY
			    ret

		assume eax:ptr brp_vertex,edx:ptr brp_vertex, esi:ptr brp_vertex, edi:ptr brp_vertex

		; Setup for Z RGB UV S
		;
		; Arranged as a tower with several entry points to save code
		;
		; XXX This could be re-done so that parameter specific entry points simply call
		; the general setup code first.
		;

		public GenericSetupFloatZRGBUVS_A
		public GenericSetupFloatZRGBUV_A
		public GenericSetupFloatZRGB_A
		public GenericSetupFloatZ_A

		PARAM	equ [ebp].param_zrgbuv

GenericSetupFloatZRGBUVS_A label near
				SETUP_FLOAT_CONSTANT_ALPHA

GenericSetupFloatZRGBUV_A label near
				SETUP_FLOAT_PARAM	_u,PARAM.s_u,PARAM.d_u_x,fp_conv_d12
				SETUP_FLOAT_PARAM	_v,PARAM.s_v,PARAM.d_v_x,fp_conv_d12
				SETUP_TEXTURE_INFO

GenericSetupFloatZRGB_A label near
				SETUP_FLOAT_PARAM	_r,PARAM.s_r,PARAM.d_r_x,fp_conv_d24
				SETUP_FLOAT_PARAM	_g,PARAM.s_g,PARAM.d_g_x,fp_conv_d24
				SETUP_FLOAT_PARAM	_b,PARAM.s_b,PARAM.d_b_x,fp_conv_d24

GenericSetupFloatZ_A label near
				SETUP_FLOAT_PARAM	_z,PARAM.s_z,PARAM.d_z_x,fp_conv_d16,1

		; Exit
        ;
				pop			esi
				pop			edi
				pop			ebp
				pop			ebx
				LOCALS_DESTROY
				ret

		; Setup for Z RGB S
		;
		public GenericSetupFloatZRGBS_A

		PARAM	equ [ebp].param_zrgb

GenericSetupFloatZRGBS_A label near
				SETUP_FLOAT_CONSTANT_ALPHA

				SETUP_FLOAT_PARAM	_r,PARAM.s_r,PARAM.d_r_x,fp_conv_d24
				SETUP_FLOAT_PARAM	_g,PARAM.s_g,PARAM.d_g_x,fp_conv_d24
				SETUP_FLOAT_PARAM	_b,PARAM.s_b,PARAM.d_b_x,fp_conv_d24

				SETUP_FLOAT_PARAM	_z,PARAM.s_z,PARAM.d_z_x,fp_conv_d16,1

		; Exit
        ;
				pop			esi
				pop			edi
				pop			ebp
				pop			ebx
				LOCALS_DESTROY
				ret

		; Setup for Z UV C
		;
		public GenericSetupFloatZUVC_A
		public GenericSetupFloatZUV_A

	   	PARAM	equ [ebp].param_zuv

GenericSetupFloatZUVC_A label near
				SETUP_FLOAT_CONSTANT_COLOUR

GenericSetupFloatZUV_A label near
				SETUP_FLOAT_PARAM	_u,PARAM.s_u,PARAM.d_u_x,fp_conv_d12
				SETUP_FLOAT_PARAM	_v,PARAM.s_v,PARAM.d_v_x,fp_conv_d12
				SETUP_TEXTURE_INFO

				SETUP_FLOAT_PARAM	_z,PARAM.s_z,PARAM.d_z_x,fp_conv_d16,1

		; Exit
        ;
				pop			esi
				pop			edi
				pop			ebp
				pop			ebx
				LOCALS_DESTROY
				ret

		; Setup for Z C
		;
		public GenericSetupFloatZC_A

				PARAM	equ [ebp].param_z

GenericSetupFloatZC_A label near
				SETUP_FLOAT_CONSTANT_COLOUR

				SETUP_FLOAT_PARAM	_z,PARAM.s_z,PARAM.d_z_x,fp_conv_d16,1

		; Exit
        ;
				pop			esi
				pop			edi
				pop			ebp
				pop			ebx
				LOCALS_DESTROY
				ret

GenericSetupTriangleFloat_A endp

		end
