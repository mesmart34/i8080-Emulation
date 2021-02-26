	call fill_a
	jmp fill_ae
fill_a:	mvi a, 0xFF
	ret
fill_ae: call func
	jmp func_end
func:	dcr a
	jnz func
	ret
func_end: print a
	hlt