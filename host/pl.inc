;;
;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: pl.inc 2.1 1995/10/13 18:13:03 sam Exp $
;; $Locker: $
;;

; Structure used to pass register values to real mode interrupt
; via Pharlap

pl_registers	struc
interrupt	word	?
rds		word	?
res		word	?
rfs		word	?
rgs		word	?
reax		dword	?
redx		dword	?
pl_registers	ends

