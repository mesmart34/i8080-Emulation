	mvi a, 0xFF
func:	dcr a
	jnz func
	print a
	hlt

;
;
;
;
;	jmp 0x000C
;
;FUNC:	mvi a, 0xFF
;	print a
;	dcr a
;	print a
;	jnz 0x0005
;	ret
;
;	call FUNC