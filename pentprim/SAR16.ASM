;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: SAR16.ASM 1.1 1996/09/26 19:04:00 JOHNG Exp $
;; $Locker: $
;;
;; Out of line call for gauranteed SAR
;;
	.386p
	.model flat
ifndef __WATCOMC__
	.code
__sar16	proc
	mov	eax,4[esp]
	sar	eax,16
	ret
__sar16	endp
endif

	end

