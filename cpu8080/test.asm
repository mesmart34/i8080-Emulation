MVI h, 0xFF
mov c, h
mov a, c
; moves 0x10 to a register
print a
dcr a
print a
jnz 0x0003
hlt