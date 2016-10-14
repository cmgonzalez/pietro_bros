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
	db	000h, 010h, 000h, 000h, 000h, 06ah, 000h, 001h, 000h
	db	000h, 008h, 00ch, 000h, 002h, 07eh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 000h, 004h, 07bh, 000h, 005h, 002h
	db	000h, 00ah, 00ch, 012h, 002h, 03dh, 000h, 003h, 001h
	db	000h, 004h, 06ah, 000h, 005h, 000h, 000h, 00ah, 000h
	db	006h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 07eh
	db	000h, 008h, 00ch, 000h, 002h, 09fh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 000h, 004h, 07bh, 000h, 005h, 002h
	db	000h, 00ah, 00ch, 006h, 004h, 07eh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 004h, 07bh, 000h, 005h, 002h
	db	000h, 00ah, 00ch, 006h, 004h, 07eh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 002h, 0d4h, 006h, 002h, 07eh
	db	000h, 009h, 000h, 000h, 004h, 0d4h, 003h, 002h, 0bdh
	db	000h, 009h, 00ch, 000h, 004h, 07eh, 006h, 002h, 07eh
	db	000h, 009h, 000h, 000h, 004h, 0bdh, 003h, 000h, 0b2h
	db	000h, 004h, 07eh, 003h, 002h, 0b2h, 003h, 008h, 000h
	db	003h, 000h, 08dh, 000h, 008h, 00ch, 000h, 002h, 0a8h
	db	000h, 009h, 00ch, 000h, 004h, 0a8h, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 000h, 0a8h, 000h, 001h, 001h
	db	000h, 002h, 08dh, 000h, 009h, 000h, 000h, 004h, 0a8h
	db	000h, 005h, 000h, 000h, 00ah, 000h, 003h, 000h, 07eh
	db	000h, 001h, 000h, 000h, 002h, 09fh, 000h, 009h, 00ch
	db	000h, 004h, 0a8h, 000h, 005h, 001h, 000h, 00ah, 00ch
	db	006h, 000h, 0a8h, 000h, 001h, 001h, 000h, 002h, 07eh
	db	000h, 009h, 000h, 000h, 004h, 09fh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 0d4h, 000h, 001h, 000h
	db	000h, 002h, 0a8h, 000h, 003h, 001h, 000h, 004h, 07eh
	db	003h, 002h, 0d4h, 000h, 003h, 000h, 000h, 004h, 0a8h
	db	000h, 005h, 001h, 003h, 008h, 000h, 003h, 000h, 077h
	db	000h, 008h, 00ch, 000h, 002h, 08dh, 000h, 009h, 00ch
	db	000h, 00ah, 00ch, 006h, 008h, 000h, 000h, 009h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 0d4h, 000h, 008h, 00ch
	db	000h, 002h, 0a8h, 000h, 003h, 001h, 000h, 009h, 00ch
	db	000h, 004h, 077h, 000h, 005h, 000h, 003h, 004h, 0d4h
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 07eh
	db	000h, 008h, 00ch, 000h, 002h, 09fh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 003h, 004h, 07eh, 003h, 008h, 000h
	db	000h, 009h, 000h, 003h, 000h, 08dh, 000h, 008h, 00ch
	db	000h, 002h, 0d4h, 000h, 009h, 00ch, 003h, 004h, 08dh
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 0a8h
	db	000h, 008h, 00ch, 000h, 002h, 08dh, 000h, 004h, 0d4h
	db	003h, 002h, 0a8h, 000h, 004h, 08dh, 003h, 008h, 000h
	db	003h, 000h, 06ah, 000h, 008h, 00ch, 000h, 002h, 07eh
	db	000h, 009h, 00ch, 000h, 004h, 07bh, 000h, 005h, 002h
	db	000h, 00ah, 00ch, 012h, 002h, 03dh, 000h, 003h, 001h
	db	000h, 004h, 06ah, 000h, 005h, 000h, 000h, 00ah, 000h
	db	006h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 07eh
	db	000h, 008h, 00ch, 000h, 002h, 09fh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 000h, 004h, 07bh, 000h, 005h, 002h
	db	000h, 00ah, 00ch, 006h, 004h, 07eh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 004h, 07bh, 000h, 005h, 002h
	db	000h, 00ah, 00ch, 006h, 004h, 07eh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 002h, 0d4h, 006h, 002h, 07eh
	db	000h, 009h, 000h, 000h, 004h, 0d4h, 003h, 002h, 0bdh
	db	000h, 009h, 00ch, 000h, 004h, 07eh, 006h, 002h, 07eh
	db	000h, 009h, 000h, 000h, 004h, 0bdh, 003h, 000h, 09fh
	db	000h, 004h, 07eh, 003h, 002h, 09fh, 003h, 008h, 000h
	db	003h, 000h, 08dh, 000h, 008h, 00ch, 000h, 002h, 0a8h
	db	000h, 009h, 00ch, 000h, 004h, 0a8h, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 000h, 0a8h, 000h, 001h, 001h
	db	000h, 002h, 08dh, 000h, 009h, 000h, 000h, 004h, 0a8h
	db	000h, 005h, 000h, 000h, 00ah, 000h, 003h, 000h, 07eh
	db	000h, 001h, 000h, 000h, 002h, 09fh, 000h, 009h, 00ch
	db	000h, 004h, 0a8h, 000h, 005h, 001h, 000h, 00ah, 00ch
	db	006h, 000h, 0a8h, 000h, 001h, 001h, 000h, 002h, 07eh
	db	000h, 009h, 000h, 000h, 004h, 09fh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 0d4h, 000h, 001h, 000h
	db	000h, 002h, 0a8h, 000h, 003h, 001h, 000h, 004h, 07eh
	db	003h, 002h, 0d4h, 000h, 003h, 000h, 000h, 004h, 0a8h
	db	000h, 005h, 001h, 003h, 008h, 000h, 003h, 000h, 077h
	db	000h, 008h, 00ch, 000h, 002h, 08dh, 000h, 009h, 00ch
	db	000h, 00ah, 00ch, 006h, 008h, 000h, 000h, 009h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 0d4h, 000h, 008h, 00ch
	db	000h, 002h, 0a8h, 000h, 003h, 001h, 000h, 009h, 00ch
	db	000h, 004h, 077h, 000h, 005h, 000h, 003h, 004h, 0d4h
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 07eh
	db	000h, 008h, 00ch, 000h, 002h, 09fh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 003h, 004h, 07eh, 003h, 008h, 000h
	db	000h, 009h, 000h, 003h, 000h, 08dh, 000h, 008h, 00ch
	db	000h, 002h, 0a8h, 000h, 009h, 00ch, 003h, 004h, 08dh
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 0d4h
	db	000h, 008h, 00ch, 000h, 002h, 08dh, 000h, 004h, 0a8h
	db	003h, 002h, 0d4h, 000h, 004h, 08dh, 003h, 008h, 000h
	db	003h, 000h, 07eh, 000h, 008h, 00ch, 000h, 002h, 09fh
	db	000h, 009h, 00ch, 000h, 004h, 03dh, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 000h, 03dh, 000h, 001h, 001h
	db	000h, 002h, 07eh, 000h, 009h, 000h, 000h, 004h, 09fh
	db	000h, 005h, 000h, 000h, 00ah, 000h, 003h, 000h, 077h
	db	000h, 001h, 000h, 000h, 002h, 08dh, 000h, 009h, 00ch
	db	000h, 004h, 03dh, 000h, 005h, 001h, 000h, 00ah, 00ch
	db	006h, 000h, 03dh, 000h, 001h, 001h, 000h, 002h, 077h
	db	000h, 009h, 000h, 000h, 004h, 08dh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 0d4h, 000h, 001h, 000h
	db	000h, 002h, 03dh, 000h, 003h, 001h, 000h, 004h, 077h
	db	003h, 002h, 0d4h, 000h, 003h, 000h, 000h, 004h, 03dh
	db	000h, 005h, 001h, 003h, 008h, 000h, 003h, 000h, 06ah
	db	000h, 008h, 00ch, 000h, 002h, 07eh, 000h, 009h, 00ch
	db	000h, 00ah, 00ch, 006h, 008h, 000h, 000h, 009h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 09fh, 000h, 008h, 00ch
	db	000h, 002h, 03dh, 000h, 003h, 001h, 000h, 009h, 00ch
	db	000h, 004h, 06ah, 000h, 005h, 000h, 003h, 004h, 09fh
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 077h
	db	000h, 008h, 00ch, 000h, 002h, 08dh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 003h, 004h, 077h, 003h, 008h, 000h
	db	000h, 009h, 000h, 003h, 000h, 07eh, 000h, 008h, 00ch
	db	000h, 002h, 0d4h, 000h, 009h, 00ch, 003h, 004h, 07eh
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 09fh
	db	000h, 008h, 00ch, 000h, 002h, 07eh, 000h, 004h, 0d4h
	db	003h, 002h, 09fh, 000h, 004h, 07eh, 003h, 008h, 000h
	db	003h, 000h, 077h, 000h, 008h, 00ch, 000h, 002h, 08dh
	db	000h, 009h, 00ch, 000h, 004h, 0dbh, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 000h, 0dbh, 000h, 001h, 001h
	db	000h, 002h, 077h, 000h, 009h, 000h, 000h, 004h, 08dh
	db	000h, 005h, 000h, 000h, 00ah, 000h, 003h, 000h, 06ah
	db	000h, 001h, 000h, 000h, 002h, 07eh, 000h, 009h, 00ch
	db	000h, 004h, 0dbh, 000h, 005h, 001h, 000h, 00ah, 00ch
	db	006h, 000h, 0dbh, 000h, 001h, 001h, 000h, 002h, 06ah
	db	000h, 009h, 000h, 000h, 004h, 07eh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 09fh, 000h, 001h, 000h
	db	000h, 002h, 0dbh, 000h, 003h, 001h, 000h, 004h, 06ah
	db	003h, 002h, 09fh, 000h, 003h, 000h, 000h, 004h, 0dbh
	db	000h, 005h, 001h, 003h, 008h, 000h, 003h, 000h, 05eh
	db	000h, 008h, 00ch, 000h, 002h, 077h, 000h, 009h, 00ch
	db	000h, 00ah, 00ch, 006h, 008h, 000h, 000h, 009h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 0bdh, 000h, 008h, 00ch
	db	000h, 002h, 0dbh, 000h, 003h, 001h, 000h, 009h, 00ch
	db	000h, 004h, 05eh, 000h, 005h, 000h, 003h, 004h, 0bdh
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 06ah
	db	000h, 008h, 00ch, 000h, 002h, 07eh, 000h, 003h, 000h
	db	000h, 009h, 00ch, 003h, 004h, 06ah, 003h, 008h, 000h
	db	000h, 009h, 000h, 003h, 000h, 05eh, 000h, 008h, 00ch
	db	000h, 002h, 09fh, 000h, 009h, 00ch, 003h, 004h, 05eh
	db	003h, 008h, 000h, 000h, 009h, 000h, 003h, 000h, 054h
	db	000h, 008h, 00ch, 000h, 002h, 077h, 000h, 009h, 00ch
	db	003h, 004h, 054h, 003h, 008h, 000h, 000h, 009h, 000h
	db	003h, 000h, 04fh, 000h, 008h, 00ch, 000h, 002h, 04fh
	db	000h, 009h, 00ch, 000h, 004h, 0dbh, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 002h, 0dbh, 000h, 003h, 001h
	db	000h, 004h, 04fh, 000h, 005h, 000h, 000h, 00ah, 000h
	db	003h, 002h, 077h, 000h, 003h, 000h, 000h, 004h, 0dbh
	db	000h, 005h, 001h, 000h, 00ah, 00ch, 006h, 002h, 0dbh
	db	000h, 003h, 001h, 000h, 004h, 04fh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 002h, 06ah, 000h, 003h, 000h
	db	000h, 004h, 07bh, 000h, 005h, 002h, 000h, 00ah, 00ch
	db	006h, 000h, 07bh, 000h, 001h, 002h, 000h, 002h, 04fh
	db	000h, 009h, 000h, 000h, 004h, 06ah, 000h, 005h, 000h
	db	000h, 00ah, 000h, 003h, 000h, 05eh, 000h, 001h, 000h
	db	000h, 002h, 07bh, 000h, 003h, 002h, 000h, 009h, 00ch
	db	000h, 004h, 07bh, 000h, 005h, 002h, 003h, 004h, 05eh
	db	000h, 005h, 000h, 003h, 000h, 07bh, 000h, 001h, 002h
	db	000h, 002h, 05eh, 000h, 003h, 000h, 000h, 009h, 000h
	db	000h, 004h, 07bh, 000h, 005h, 002h, 003h, 000h, 047h
	db	000h, 001h, 000h, 000h, 002h, 054h, 000h, 009h, 00ch
	db	000h, 004h, 0a8h, 000h, 005h, 001h, 000h, 00ah, 00ch
	db	006h, 002h, 0a8h, 000h, 003h, 001h, 000h, 004h, 047h
	db	000h, 005h, 000h, 000h, 00ah, 000h, 003h, 002h, 06ah
	db	000h, 003h, 000h, 000h, 004h, 0a8h, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 002h, 0a8h, 000h, 003h, 001h
	db	000h, 004h, 047h, 000h, 005h, 000h, 000h, 00ah, 000h
	db	003h, 002h, 05eh, 000h, 003h, 000h, 000h, 004h, 035h
	db	000h, 005h, 002h, 000h, 00ah, 00ch, 006h, 000h, 035h
	db	000h, 001h, 002h, 000h, 002h, 047h, 000h, 009h, 000h
	db	000h, 004h, 05eh, 000h, 005h, 000h, 000h, 00ah, 000h
	db	003h, 000h, 054h, 000h, 001h, 000h, 000h, 002h, 035h
	db	000h, 003h, 002h, 000h, 009h, 00ch, 000h, 004h, 035h
	db	000h, 005h, 002h, 003h, 004h, 054h, 000h, 005h, 000h
	db	003h, 000h, 035h, 000h, 001h, 002h, 000h, 002h, 054h
	db	000h, 003h, 000h, 000h, 009h, 000h, 000h, 004h, 035h
	db	000h, 005h, 002h, 003h, 000h, 04fh, 000h, 001h, 000h
	db	000h, 002h, 0bdh, 000h, 009h, 00ch, 000h, 004h, 03dh
	db	000h, 005h, 001h, 000h, 00ah, 00ch, 001h, 002h, 03dh
	db	000h, 003h, 001h, 000h, 004h, 04fh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 000h, 002h, 0a8h, 000h, 003h, 000h
	db	000h, 004h, 03dh, 000h, 005h, 001h, 000h, 00ah, 00ch
	db	001h, 002h, 03dh, 000h, 003h, 001h, 000h, 004h, 04fh
	db	000h, 005h, 000h, 000h, 00ah, 000h, 000h, 002h, 09fh
	db	000h, 003h, 000h, 000h, 004h, 03dh, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 006h, 002h, 03dh, 000h, 003h, 001h
	db	000h, 004h, 04fh, 000h, 005h, 000h, 000h, 00ah, 000h
	db	001h, 002h, 00bh, 000h, 004h, 03dh, 000h, 005h, 001h
	db	000h, 00ah, 00ch, 009h, 000h, 00bh, 000h, 001h, 001h
	db	000h, 002h, 0a8h, 000h, 004h, 04fh, 000h, 005h, 000h
	db	000h, 00ah, 000h, 009h, 000h, 0fch, 000h, 001h, 000h
	db	000h, 004h, 00bh, 000h, 005h, 001h, 003h, 004h, 0fch
	db	000h, 005h, 000h, 003h, 000h, 0a8h, 000h, 001h, 001h
	db	000h, 002h, 0fch, 000h, 003h, 000h, 000h, 009h, 000h
	db	000h, 004h, 0a8h, 000h, 005h, 001h, 003h, 000h, 03dh
	db	000h, 002h, 07bh, 000h, 003h, 002h, 000h, 009h, 00ch
	db	003h, 004h, 03dh, 00fh, 008h, 000h, 000h, 009h, 000h
	db	000h, 0ffh, 000h

end	inicio	