;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: v1m_386.asm 1.1 1997/07/11 16:16:15 jon Exp JON $
;; $Locker: JON $
;;
;; Downcoded model support
;;
	.586p
	.model flat,c

	include	drv.inc
	include 586_macs.inc

if ADD_RCS_ID
	.data
	db '$Id: v1m_386.asm 1.1 1997/07/11 16:16:15 jon Exp JON $',0
	align	16
endif
	.data
loop_count	dd	0

	.code

	if BASED_FIXED	; Only provide these when driver is fixed point

	include vertex.inc

; Process one on-screen group of culled faces
;
V1Face_CullOneSidedPerspective_A proc uses ebx esi edi,
	self : ptr word,  
	renderer : ptr word

		push	ebp

		mov	eax,rend.nfaces
		mov	loop_count,eax

		mov	esi,rend.faces
		mov	ebp,rend.temp_faces
		
	; Loop for each face in group
	;
	; edx:eax	Multiply result
	; ecx:ebx	Multiply accumulator
	; esi 		Current br_face pointer
	; ebp 		Current temp_face pointer
	; edi		zb.vertex_counts
	;

		mov	edi,rend.vertex_counts
	align	16

face_loop:

	; Dot the face normal with the eye position
	;		
		mov		eax,[esi].v11face.eqn.v[0]
		imul	scache.eye_m.v[0]
		mov		ebx,eax
		mov		ecx,edx

		mov		eax,[esi].v11face.eqn.v[4]
		imul	scache.eye_m.v[4]
		add		ebx,eax
		adc		ecx,edx

		mov		eax,[esi].v11face.eqn.v[8]
		imul	scache.eye_m.v[8]
		add		eax,ebx
		adc		edx,ecx

		shr		eax,16
		shl		edx,16
		or		eax,edx

	; If n.eye > face->d then face is visible
	;
		cmp		eax,[esi].v11face.eqn.v[12]
		jle		not_visible

	; Mark vertices as visible
	;


	; Get vertex 0,1,2
	;
		mov		eax,dword ptr [esi].v11face.vertices[0]
		mov		ecx,dword ptr [esi].v11face.vertices[4]
		mov		ebx,dword ptr [esi].v11face.vertices[2]
		and		eax,0000ffffh
		and		ebx,0000ffffh
		and		ecx,0000ffffh

	; Mark face as visible
	;
		mov	[ebp].temp_face.flag, TFF_VISIBLE ; temp_face flag

	; Mark vertices as referenced
	;
		inc		byte ptr [edi+eax]
		inc		byte ptr [edi+ebx]
		inc		byte ptr [edi+ecx]

	; Next vertex
	;
		dec		loop_count
		lea		esi,[esi+sizeof v11face]
		lea		ebp,[ebp+sizeof temp_face]
		jne		face_loop

		pop		ebp
		ret

	; Next face
	;
not_visible:
		mov		byte ptr [ebp].temp_face.flag,0		; temp_face flag

		dec		loop_count
		lea		esi,[esi+sizeof v11face]
		lea		ebp,[ebp+sizeof temp_face]
		jne		face_loop

		pop		ebp
		ret
V1Face_CullOneSidedPerspective_A endp

; Process one on-screen group of culled faces
;
V1Face_OS_CullOneSidedPerspective_A proc uses ebx esi edi,
	self : 	ptr word,
	renderer : ptr word

		push	ebp

		mov		eax,rend.nfaces
		mov		loop_count,eax

		mov		esi,rend.faces
		mov		ebp,rend.temp_faces

	; Loop for each face in group
	;
	; edx:eax	Multiply result
	; ecx:ebx	Multiply accumulator
	; esi 		Current br_face pointer
	; ebp 		Current temp_face pointer
	; edi		zb.vertex_counts
	;

		mov		edi,rend.vertex_counts

	align	16
face_loop:

	; Dot the face normal with the eye position
	;		
		mov		eax,[esi].v11face.eqn.v[0]
		imul	scache.eye_m.v[0]
		mov		ebx,eax
		mov		ecx,edx

		mov		eax,[esi].v11face.eqn.v[4]
		imul	scache.eye_m.v[4]
		add		ebx,eax
		adc		ecx,edx

		mov		eax,[esi].v11face.eqn.v[8]
		imul	scache.eye_m.v[8]
		add		eax,ebx
		adc		edx,ecx

		shr		eax,16
		shl		edx,16
		or 		eax,edx

	; If n.eye > face->d then face is visible
	;
		cmp		eax,[esi].v11face.eqn.v[12]
		jle		not_visible

	; Mark vertices as visible
	;

	; Get vertex 0,1,2
	;
		mov		eax,dword ptr [esi].v11face.vertices[0]
		mov		ecx,dword ptr [esi].v11face.vertices[4]
		mov		ebx,dword ptr [esi].v11face.vertices[2]
		and		eax,0000ffffh
		and		ebx,0000ffffh
		and		ecx,0000ffffh

	; Mark face as visible
	;
		mov		[ebp].temp_face.flag, TFF_VISIBLE ; temp_face flag

	; Mark vertices as referenced
	;
		mov		byte ptr [edi+eax],1
		mov		byte ptr [edi+ebx],1
		mov		byte ptr [edi+ecx],1

	; Next vertex
	;
		dec		loop_count
		lea		esi,[esi+sizeof v11face]
		lea		ebp,[ebp+sizeof temp_face]
		jne		face_loop

		pop		ebp
		ret

	; Next face
	;
not_visible:
		mov		byte ptr [ebp].temp_face.flag,0		; temp_face flag

		dec		loop_count
		lea		esi,[esi+sizeof v11face]
		lea		ebp,[ebp+sizeof temp_face]
		jne		face_loop

		pop		ebp
		ret
V1Face_OS_CullOneSidedPerspective_A endp

	endif	; BASED_FIXED
end

