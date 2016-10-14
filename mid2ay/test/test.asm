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
	db	000h, 010h, 000h, 000h, 000h, 0fch, 000h, 001h, 000h
	db	000h, 008h, 00fh, 019h, 000h, 07eh, 000h, 002h, 0fch
	db	000h, 003h, 000h, 00dh, 000h, 0a8h, 000h, 002h, 07eh
	db	000h, 004h, 0fch, 000h, 005h, 000h, 019h, 000h, 0fch
	db	000h, 002h, 0a8h, 000h, 004h, 07eh, 019h, 000h, 08dh
	db	000h, 002h, 0fch, 000h, 004h, 0a8h, 019h, 000h, 0fch
	db	000h, 002h, 08dh, 000h, 004h, 0fch, 00dh, 000h, 0bdh
	db	000h, 002h, 0fch, 000h, 004h, 08dh, 019h, 002h, 0bdh
	db	000h, 004h, 0fch, 019h, 000h, 08dh, 000h, 002h, 0a8h
	db	000h, 009h, 00fh, 000h, 004h, 0bdh, 019h, 000h, 0fch
	db	000h, 002h, 08dh, 000h, 009h, 000h, 000h, 004h, 0a8h
	db	00dh, 002h, 0fch, 000h, 004h, 08dh, 00dh, 000h, 07eh
	db	000h, 002h, 0a8h, 000h, 009h, 00fh, 000h, 004h, 0fch
	db	00dh, 008h, 000h, 000h, 009h, 000h, 00dh, 008h, 00fh
	db	000h, 009h, 00fh, 000h, 004h, 07eh, 019h, 000h, 0fch
	db	000h, 002h, 07eh, 000h, 009h, 000h, 000h, 004h, 0a8h
	db	00dh, 000h, 08dh, 000h, 002h, 0a8h, 000h, 009h, 00fh
	db	000h, 004h, 0fch, 00dh, 008h, 000h, 000h, 009h, 000h
	db	00dh, 008h, 00fh, 000h, 002h, 0bdh, 000h, 009h, 00fh
	db	000h, 004h, 08dh, 00dh, 008h, 000h, 000h, 009h, 000h
	db	00dh, 000h, 096h, 000h, 008h, 00fh, 000h, 009h, 00fh
	db	00dh, 008h, 000h, 000h, 009h, 000h, 00dh, 000h, 0fch
	db	000h, 008h, 00fh, 000h, 002h, 096h, 000h, 004h, 0bdh
	db	00dh, 000h, 0a8h, 000h, 001h, 001h, 000h, 002h, 0fch
	db	000h, 004h, 096h, 003h, 002h, 0a8h, 000h, 003h, 001h
	db	000h, 004h, 0fch, 003h, 004h, 0a8h, 000h, 005h, 001h
	db	013h, 000h, 0f8h, 00dh, 000h, 0fch, 000h, 001h, 000h
	db	000h, 002h, 050h, 000h, 009h, 00fh, 000h, 004h, 0f8h
	db	00dh, 008h, 000h, 000h, 009h, 000h, 00dh, 008h, 00fh
	db	000h, 009h, 00fh, 000h, 004h, 0fch, 000h, 005h, 000h
	db	00dh, 008h, 000h, 000h, 009h, 000h, 026h, 000h, 0a8h
	db	000h, 008h, 00fh, 000h, 002h, 0d4h, 000h, 003h, 000h
	db	000h, 009h, 00fh, 000h, 004h, 01bh, 000h, 005h, 001h
	db	000h, 00ah, 00fh, 019h, 004h, 0fch, 000h, 005h, 000h
	db	00dh, 004h, 01bh, 000h, 005h, 001h, 00dh, 004h, 0fch
	db	000h, 005h, 000h, 00dh, 004h, 01bh, 000h, 005h, 001h
	db	026h, 000h, 096h, 000h, 002h, 0bdh, 000h, 004h, 0a8h
	db	00dh, 000h, 0f8h, 000h, 001h, 001h, 000h, 002h, 096h
	db	000h, 009h, 000h, 000h, 004h, 0bdh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 00dh, 000h, 096h, 000h, 001h, 000h
	db	000h, 002h, 0bdh, 000h, 009h, 00fh, 000h, 004h, 0fch
	db	000h, 00ah, 00fh, 00dh, 004h, 096h, 000h, 00ah, 000h
	db	00dh, 004h, 0fch, 000h, 00ah, 00fh, 00dh, 004h, 096h
	db	000h, 00ah, 000h, 019h, 000h, 0a8h, 000h, 002h, 0d4h
	db	002h, 004h, 0a8h, 00bh, 000h, 0fch, 000h, 002h, 0a8h
	db	000h, 009h, 000h, 000h, 004h, 0d4h, 003h, 002h, 0fch
	db	000h, 004h, 0a8h, 003h, 004h, 0fch, 006h, 002h, 0f8h
	db	000h, 003h, 001h, 000h, 009h, 00fh, 00dh, 002h, 0fch
	db	000h, 003h, 000h, 000h, 004h, 050h, 000h, 005h, 001h
	db	000h, 00ah, 00fh, 00dh, 009h, 000h, 000h, 004h, 0fch
	db	000h, 005h, 000h, 000h, 00ah, 000h, 00dh, 009h, 00fh
	db	000h, 004h, 050h, 000h, 005h, 001h, 000h, 00ah, 00fh
	db	00dh, 009h, 000h, 000h, 004h, 0fch, 000h, 005h, 000h
	db	000h, 00ah, 000h, 00dh, 002h, 0f8h, 000h, 003h, 001h
	db	000h, 009h, 00fh, 00dh, 002h, 0a8h, 019h, 008h, 000h
	db	000h, 009h, 000h, 000h, 0ffh, 000h

end	inicio	