;;
;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: dpmi.inc 2.1 1995/10/13 18:12:53 sam Exp $
;; $Locker: $
;;

; Parameters to DPMI INT 31, 0300
;
dpmi_registers	struct
redi		dword	0
resi		dword	0
rebp		dword	0
		dword	0
rebx		dword	0
redx		dword	0
recx		dword	0
reax		dword	0
rflags		word	0
res		word	0
rds		word	0
rfs		word	0
rgs		word	0
rip		word	0
rcs		word	0
rsp		word	0
rss		word	0
dpmi_registers	ends


