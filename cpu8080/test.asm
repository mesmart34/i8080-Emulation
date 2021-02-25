	mvi a, 0xFF
	call func
	jmp func_end
func:	dcr a
	print a
	ret
func_end:	print a
