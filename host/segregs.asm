;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: segregs.asm 2.1 1995/10/13 18:13:07 sam Exp $
;; $Locker: $
;;
	.386p
	.model flat

	.code

; br_uint_16 ASM_CALL HostDSQuery(void);
;
		public _HostDSQuery
_HostDSQuery proc
		mov	ax,ds
		ret
_HostDSQuery endp

; br_uint_16 ASM_CALL HostESQuery(void);
;
		public _HostESQuery
_HostESQuery proc
		mov	ax,ds
		ret
_HostESQuery endp

; br_uint_16 ASM_CALL HostCSQuery(void);
;
		public _HostCSQuery
_HostCSQuery proc
		mov	ax,cs
		ret
_HostCSQuery endp

; br_uint_16 ASM_CALL HostSSQuery(void);
;
		public _HostSSQuery
_HostSSQuery proc
		mov	ax,ss
		ret
_HostSSQuery endp

	end

