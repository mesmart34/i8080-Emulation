	mvi b, 0x00	;	x
	mvi c, 0x01	;	y
	mvi d, 0x00	;	z
	xra a
loop_1:	add b
	add c
	mov d, a
	mov b, c
	mov c, d
	mov a, d
	cpi 0xFF
	print a	
	jnc loop_1
	hlt