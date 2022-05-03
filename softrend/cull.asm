.686p
.model	flat,c

include	drv.inc
include pro_epi.inc
;include klamath.inc


.code


;static void GEOMETRY_CALL V1Face_CullOneSidedPerspective(struct br_geometry *self, struct br_renderer *renderer)
;{
;	int f,n;
;	struct v11face *fp = rend.faces;
;	struct temp_face *tfp = rend.temp_faces;
;	union {
;		float f;
;		int i;
;	}result;
;
;//	rend.nvisible_faces = 0;
;
; 	for(f=0,n=0; f < rend.nfaces; f++, fp++, tfp++) {
;
;		/*
;		 * if Plane_Eqn . Eye <= 0, face is away from eye
;		 */
;		result.f=BrVector3Dot((br_vector3 *)&fp->eqn,&scache.eye_m)-fp->eqn.v[3];
;		if(result.i&0x80000000){
;			tfp->flag = 0;
;			continue;
;		}
;
;		tfp->flag = TFF_VISIBLE;
;
;		rend.vertex_counts[fp->vertices[0]]++;
;		rend.vertex_counts[fp->vertices[1]]++;
;		rend.vertex_counts[fp->vertices[2]]++;
;
;//		rend.nvisible_faces++;
;	}
;}

X equ 0
Y equ 4
Z equ 8
CONST equ 12

V1Face_CullOneSidedPerspective proc self:ptr geometry, renderer:ptr br_renderer
	mov esi,rend.faces
	assume esi:ptr v11face
	xor eax,eax

	mov edi,rend.temp_faces
	assume edi:ptr temp_face
	xor ebx,ebx

	mov edx,rend.nfaces
	xor ecx,ecx

	mov ebp,rend.vertex_counts
	; stall v pipe

next:
	dec edx
	jl done

									;	0			1			2			3			4			5			6			7
	fld dword ptr [esi+X].eqn		;	eqnx
	fmul dword ptr scache.eye_m+X	;	eqnx.eyex
	fld dword ptr [esi+Y].eqn		;	eqny		eqnx.eyex
	fmul dword ptr scache.eye_m+Y	;	eqny.eyey	eqnx.eyex
	fld dword ptr [esi+Z].eqn		;	eqnz		eqny.eyey	eqnx.eyex
	fmul dword ptr scache.eye_m+Z	;	eqnz.eyez	eqny.eyey	eqnx.eyex
	 fxch st(2)						;	eqnx.eyex	eqny.eyey	eqnz.eyez
	faddp st(1),st					;	y.y+x.x		eqnz.eyez
	fxch st(1)						;	eqnz.eyez	y.y+x.x
	fsub dword ptr [esi+CONST].eqn	;	z.z-c		y.y+x.x
	mov ax,[esi].vertices ; two cycles
	mov count,edx
	faddp st(1),st					;	dot
	mov bx,[esi+2].vertices ; two cycles
	mov dl,byte ptr[ebp+eax]
	fstp floatResult				; 

	mov dh,byte ptr floatResult+3
	mov byte ptr [edi].flag,0

	test dh,080h
	jnz invisible
		
	mov cx,[esi+4].vertices ; two cycles
	mov byte ptr [edi].flag,TFF_VISIBLE

	inc dl
	mov dh,byte ptr [ebp+ebx]

	mov byte ptr[ebp+eax],dl
	mov dl,byte ptr [ebp+ecx]

	inc dh
	inc dl

	mov byte ptr[ebp+ebx],dh
	mov byte ptr[ebp+ecx],dl

invisible:
	mov edx,count
	add edi,sizeof temp_face

	add esi,sizeof v11face
	jmp next
done:
	ret
V1Face_CullOneSidedPerspective endp
.data

count dword ?
floatResult dword ?

end


