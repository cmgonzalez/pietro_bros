
SECTION bss_user
PUBLIC _midi_position

_midi_hold    :  defb 0
_midi_position:  defw 0


; reset AY and play new song

SECTION code_user
PUBLIC _midi_play

_midi_play:

   ; hl = song address

   ; disable midi isr
   
   ld de,0
   ld (_midi_position),de
   
   ; silence AY
   
   ld a,8
   ld c,0xfd

loop_silence:

   ld b,0xff
   out (c),a
   
   ld b,0xbf
   out (c),e
   
   inc a
   
   cp 11
   jr nz, loop_silence
   
   ; play song
   
   ld (_midi_position),hl
   
   ld a,(hl)
   ld (_midi_hold),a

   ret

   
; midi player interrupt service routine
; adapted from midi2ay

SECTION code_user
PUBLIC _midi_play_isr

_midi_play_isr:

   ld hl,(_midi_position)
   
   ld a,h
   or l
   ret z

   ld a,(_midi_hold)
   or a
   jr nz, hold

loop:
   
   inc hl
   ld a,(hl)
   
   cp 0xff
   jr z, midi_finished
   
   cp 0xfe
   jr z, midi_position_next
   
   ld bc,0xfffd
   out (c),a
   
   inc hl
   ld a,(hl)
   
   ld b,0xbf
   out (c),a
   
   inc hl
   ld a,(hl)
   
   or a
   jr z, loop
   
midi_position_hold:

   ld (_midi_position),hl

hold:

   dec a
   ld (_midi_hold),a
   ret
   
midi_position_next:

   inc hl
   inc hl
   ld (_midi_position),hl
   
   ld a,(hl)
   ld (_midi_hold),a
   ret
   
midi_finished:

   ld hl,0
   ld (_midi_position),hl
   ret

   
; midi music converted by midi2ay

SECTION rodata_user
PUBLIC _mb_midi_title

_mb_midi_title:

	defb	000h, 007h, 038h, 000h, 008h, 000h, 000h, 009h, 000h
	defb	000h, 010h, 000h, 02ch, 000h, 06ah, 000h, 001h, 000h
	defb	000h, 008h, 00fh, 000h, 002h, 07eh, 000h, 003h, 000h
	defb	000h, 009h, 00fh, 000h, 004h, 07bh, 000h, 005h, 002h
	defb	000h, 00ah, 00fh, 011h, 002h, 03dh, 000h, 003h, 001h
	defb	009h, 002h, 09fh, 000h, 003h, 000h, 000h, 000h, 07eh
	defb	011h, 002h, 0d4h, 009h, 002h, 0bdh, 008h, 002h, 07bh
	defb	000h, 003h, 002h, 000h, 004h, 07eh, 000h, 005h, 000h
	defb	000h, 00ah, 000h, 000h, 002h, 0b2h, 000h, 003h, 000h
	defb	000h, 004h, 07bh, 000h, 005h, 002h, 000h, 00ah, 00fh
	defb	008h, 002h, 0a8h, 000h, 000h, 08dh, 000h, 004h, 0a8h
	defb	000h, 005h, 001h, 008h, 002h, 09fh, 000h, 000h, 07eh
	defb	008h, 002h, 0d4h, 009h, 000h, 077h, 000h, 002h, 08dh
	defb	009h, 002h, 0d4h, 000h, 004h, 077h, 000h, 005h, 000h
	defb	000h, 00ah, 000h, 000h, 004h, 0a8h, 000h, 005h, 001h
	defb	000h, 00ah, 00fh, 008h, 000h, 07eh, 000h, 002h, 09fh
	defb	009h, 000h, 08dh, 000h, 002h, 0d4h, 009h, 002h, 0a8h
	defb	009h, 000h, 06ah, 000h, 002h, 07eh, 000h, 004h, 07bh
	defb	000h, 005h, 002h, 011h, 002h, 03dh, 000h, 003h, 001h
	defb	009h, 000h, 07eh, 000h, 002h, 09fh, 000h, 003h, 000h
	defb	011h, 002h, 0d4h, 009h, 002h, 0bdh, 009h, 002h, 09fh
	defb	009h, 002h, 0a8h, 000h, 000h, 08dh, 000h, 004h, 0a8h
	defb	000h, 005h, 001h, 008h, 000h, 07eh, 000h, 002h, 09fh
	defb	009h, 002h, 0d4h, 009h, 000h, 077h, 000h, 002h, 08dh
	defb	009h, 002h, 0d4h, 009h, 000h, 07eh, 000h, 002h, 09fh
	defb	009h, 002h, 0a8h, 000h, 000h, 08dh, 008h, 002h, 0d4h
	defb	009h, 000h, 07eh, 000h, 002h, 09fh, 000h, 004h, 03dh
	defb	008h, 000h, 077h, 000h, 002h, 08dh, 009h, 002h, 0d4h
	defb	009h, 000h, 06ah, 000h, 002h, 07eh, 009h, 002h, 09fh
	defb	009h, 000h, 077h, 000h, 002h, 08dh, 009h, 000h, 07eh
	defb	000h, 002h, 0d4h, 009h, 002h, 09fh, 008h, 002h, 03dh
	defb	000h, 003h, 001h, 000h, 004h, 07eh, 000h, 005h, 000h
	defb	000h, 00ah, 000h, 000h, 000h, 077h, 000h, 002h, 08dh
	defb	000h, 003h, 000h, 000h, 004h, 03dh, 000h, 005h, 001h
;;	defb	000h, 00ah, 00fh, 000h, 004h, 0defbh, 008h, 000h, 07eh
    defb    000h, 00ah, 00fh, 000h, 004h, 0fbh, 008h, 000h, 07eh
;;	defb	000h, 002h, 0defbh, 000h, 003h, 001h, 000h, 004h, 077h
    defb    000h, 002h, 0fbh, 000h, 003h, 001h, 000h, 004h, 077h
	defb	000h, 005h, 000h, 000h, 00ah, 000h, 000h, 000h, 06ah
;;	defb	000h, 002h, 07eh, 000h, 003h, 000h, 000h, 004h, 0defbh
    defb    000h, 002h, 07eh, 000h, 003h, 000h, 000h, 004h, 0fbh
	defb	000h, 005h, 001h, 000h, 00ah, 00fh, 008h, 002h, 09fh
	defb	009h, 000h, 05eh, 000h, 002h, 077h, 009h, 002h, 0bdh
	defb	009h, 002h, 07eh, 000h, 000h, 06ah, 008h, 000h, 05eh
	defb	000h, 002h, 09fh, 009h, 002h, 077h, 000h, 000h, 054h
	defb	008h, 002h, 05eh, 000h, 000h, 04fh, 008h, 002h, 077h
	defb	009h, 002h, 06ah, 000h, 004h, 07bh, 000h, 005h, 002h
	defb	009h, 002h, 05eh, 009h, 000h, 047h, 000h, 002h, 054h
	defb	000h, 004h, 0a8h, 000h, 005h, 001h, 008h, 002h, 06ah
	defb	009h, 002h, 05eh, 000h, 004h, 035h, 000h, 005h, 002h
	defb	008h, 002h, 054h, 009h, 000h, 070h, 000h, 002h, 035h
	defb	000h, 003h, 002h, 000h, 004h, 047h, 000h, 005h, 000h
	defb	000h, 00ah, 000h, 000h, 002h, 03dh, 000h, 003h, 001h
	defb	000h, 004h, 070h, 003h, 000h, 06ah, 005h, 000h, 00bh
	defb	000h, 001h, 001h, 000h, 004h, 06ah, 009h, 002h, 0a8h
	defb	000h, 004h, 00bh, 000h, 005h, 001h, 009h, 000h, 0fch
	defb	000h, 001h, 000h, 009h, 000h, 03dh, 000h, 001h, 001h
	defb	000h, 004h, 0fch, 000h, 005h, 000h, 000h, 002h, 03dh
	defb	000h, 009h, 000h, 000h, 004h, 0a8h, 000h, 005h, 001h
	defb	055h, 008h, 000h, 000h, 0ffh, 000h
