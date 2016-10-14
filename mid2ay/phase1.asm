; Generado automáticamente por 'midi2ay'

	org	8000h

inicio	di
	ld	sp,0
	ld	hl,0feffh
	ld	(hl),rsi and 0ffh
	inc	hl
	ld	(hl),rsi shr 8
	ld	a,0feh
	ld	i,a
	im	2
	ei
	jr	$

rsi	ld	hl,(pos)
bucle	ld	a,(hl)
	or	a
	jr	nz,finrsi
	inc	hl
	ld	a,(hl)
	cp	0ffh
	jr	z,fin
	cp	0feh
	jr	z,salta
	ld	bc,0fffdh
	out	(c),a
	inc	hl
	ld	a,(hl)
	ld	b,0bfh
	out	(c),a
	inc	hl
	jr	bucle
finrsi	ld	(pos),hl
	dec	a
	ld	(hl),a
	ei
	ret
salta	inc	hl
	inc	hl
	ld	(pos),hl
	ei
	ret
fin	xor	a
	out	(0feh),a
	jr	$

pos	dw	notas

notas
	db	000h, 007h, 038h, 000h, 008h, 000h, 000h, 009h, 000h
	db	000h, 010h, 000h, 005h, 000h, 0d4h, 000h, 001h, 000h
	db	000h, 008h, 00ch, 018h, 000h, 01bh, 000h, 001h, 001h
	db	000h, 008h, 00ch, 000h, 002h, 0d4h, 000h, 003h, 000h
	db	008h, 000h, 0d4h, 000h, 001h, 000h, 000h, 008h, 00ch
	db	000h, 002h, 01bh, 000h, 003h, 001h, 000h, 004h, 0d4h
	db	000h, 005h, 000h, 018h, 008h, 000h, 001h, 000h, 01bh
	db	000h, 001h, 001h, 000h, 008h, 00ch, 000h, 002h, 0d4h
	db	000h, 003h, 000h, 000h, 004h, 01bh, 000h, 005h, 001h
	db	008h, 000h, 0d4h, 000h, 001h, 000h, 000h, 008h, 00ch
	db	000h, 002h, 01bh, 000h, 003h, 001h, 000h, 004h, 0d4h
	db	000h, 005h, 000h, 008h, 000h, 01bh, 000h, 001h, 001h
	db	000h, 008h, 00ch, 000h, 002h, 0d4h, 000h, 003h, 000h
	db	000h, 004h, 01bh, 000h, 005h, 001h, 008h, 000h, 0d4h
	db	000h, 001h, 000h, 000h, 008h, 00ch, 000h, 002h, 01bh
	db	000h, 003h, 001h, 000h, 004h, 0d4h, 000h, 005h, 000h
	db	008h, 000h, 0a8h, 000h, 008h, 00ch, 000h, 002h, 0d4h
	db	000h, 003h, 000h, 000h, 004h, 01bh, 000h, 005h, 001h
	db	008h, 008h, 000h, 001h, 000h, 08dh, 000h, 008h, 00ch
	db	000h, 002h, 0a8h, 000h, 004h, 0d4h, 000h, 005h, 000h
	db	021h, 000h, 09fh, 000h, 008h, 00ch, 000h, 002h, 08dh
	db	000h, 004h, 0a8h, 018h, 008h, 000h, 001h, 000h, 0bdh
	db	000h, 008h, 00ch, 000h, 002h, 09fh, 000h, 004h, 08dh
	db	008h, 000h, 09fh, 000h, 008h, 00ch, 000h, 002h, 0bdh
	db	000h, 004h, 09fh, 01ah, 000h, 0bdh, 000h, 008h, 00ch
	db	000h, 002h, 09fh, 000h, 004h, 0bdh, 008h, 000h, 09fh
	db	000h, 008h, 00ch, 000h, 002h, 0bdh, 000h, 004h, 09fh
	db	008h, 000h, 0bdh, 000h, 008h, 00ch, 000h, 002h, 09fh
	db	000h, 004h, 0bdh, 009h, 000h, 0e0h, 000h, 008h, 00ch
	db	000h, 002h, 0bdh, 000h, 004h, 09fh, 008h, 000h, 0bdh
	db	000h, 008h, 00ch, 000h, 002h, 0e0h, 000h, 004h, 0bdh
	db	008h, 000h, 01bh, 000h, 001h, 001h, 000h, 008h, 00ch
	db	000h, 002h, 0bdh, 000h, 004h, 0e0h, 022h, 008h, 000h
	db	000h, 0ffh, 000h

end	inicio	