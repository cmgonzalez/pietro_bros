
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; AY SILENCE ;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Variables available from the main bank
; allow sound to be controlled without
; disabling interrupts.

SECTION bss_user

PUBLIC _ay_reset_low

_ay_reset_low : defs 1

; Controlling functions available from
; the main bank.

SECTION code_user

PUBLIC _ay_reset

_ay_reset:

   ; uses : af, de, hl

   ld hl,0
   jp _ay_midi_play

;

PUBLIC _ay_is_playing

EXTERN _spec128, error_znc

_ay_is_playing:

   ; exit : l = 0 if none
   ;          = 1 if effect playing
   ;          = 2 if midi playing
   ;
   ; uses : af, de, l
   
   ld a,(_spec128)
   or a
   jp z, error_znc             ; 48k is never playing ay
   
   ld l,2
   
   ld de,(_ay_midi_playing)
   
   ld a,d
   or e
   ret nz
   
   dec l
   
   ld de,(_ay_fx_playing)
   
   ld a,d
   or e
   ret nz
   
   dec l
   ret

; Bank 06 functions

SECTION BANK_06

PUBLIC BANK06_ay_reset

EXTERN error_znc

BANK06_ay_reset:

   ; Resets the AY chip
   ;
   ; exit : hl = 0
   ;        carry reset
   ;
   ; uses : af, bc, de, hl

   xor a
   
   ld hl,BANK06_ay_reg
   ld de,BANK06_ay_reg + 1
   ld bc,13
   
   ld (hl),a
   ldir
   
   ld a,%10111000
   ld (BANK06_ay_reg + 7),a
   
   ld a,31
   ld (BANK06_ay_reg + 6),a
   
   call BANK06_ay_rout
   jp error_znc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; MIDI PLAYER ;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Variables available from the main bank
; allow sound to be controlled without
; disabling interrupts.

SECTION bss_user

PUBLIC _ay_midi_playing
PUBLIC _ay_midi_hold

_ay_midi_playing : defs 2
_ay_midi_hold    : defs 1

; Controlling functions available from
; the main bank.

SECTION code_user

PUBLIC _ay_midi_play

_ay_midi_play:

   ; enter : hl = song address
   ;
   ; uses  : af, de, hl
   
   ex de,hl

   ; stop all sound
   
   ld hl,0
   ld (_ay_midi_playing),hl
   ld (_ay_fx_playing),hl

   ld a,l
   ld (_ay_reset_low),a
   
   ; start midi
   
   ld a,(de)
   ld (_ay_midi_hold),a
   
   ld (_ay_midi_playing),de
   ret

; Bank 06 functions

SECTION BANK_06

PUBLIC BANK06_ay_midi_play_isr

BANK06_ay_midi_play_isr:

   ; enter : hl = void *position
   ;          d = hold
   ;
   ; exit  : hl = void *position
   ;          a = hold
   ;
   ; uses  : af, bc, de, hl
   
   ld a,h
   or l
   ret z
   
   ld a,d
   or a
   jr nz, midi_position_hold

midi_loop:

   inc hl
   ld a,(hl)
   
   cp 0xff
   jp z, BANK06_ay_reset
   
   cp 0xfe
   jr z, midi_position_next
   
   ld bc,$fffd
   out (c),a
   
   inc hl
   ld a,(hl)
   
   ld b,$bf
   out (c),a
   
   inc hl
   ld a,(hl)
   
   or a
   jr z, midi_loop

midi_position_hold:

   dec a
   ret

midi_position_next:

   inc hl
   inc hl
   
   ld a,(hl)
   ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; AY WAV EFFECTS PLAYER ;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Variables available from the main bank
; allow sound to be controlled without
; disabling interrupts.

SECTION bss_user

PUBLIC _ay_fx_playing

_ay_fx_playing : defs 2

; Controlling functions available from
; the main bank.

SECTION code_user

PUBLIC _ay_fx_play

_ay_fx_play:

   ; enter : hl = void *effect
   ;
   ; uses  : af, de, hl

   ex de,hl

   ; stop all sound
   
   ld hl,0
   ld (_ay_midi_playing),hl
   ld (_ay_fx_playing),hl

   ld a,l
   ld (_ay_reset_low),a
   
   ; start ay wav effect
   
   ld (_ay_fx_playing),de
   ret

; Bank 06 variables

SECTION BANK_06

BANK06_ay_reg : defs 14

; Bank 06 functions

SECTION BANK_06

PUBLIC BANK06_ay_fx_play_isr

BANK06_ay_fx_play_isr:

   ; enter : hl = void *position
   ;
   ; exit  : hl = void *position
   ;
   ; uses  : af, bc, de, hl
   
   ld a,h
   or l
   ret z

   ld a,(hl)

   cp $ff
   jr nz, fx_playing

   inc hl
   
   cp (hl)
   jp z, BANK06_ay_reset
   
   dec hl
   
fx_playing:

   ld de,BANK06_ay_reg
   ld bc,$03ff

fx_loop:

   ldi 
   
   ld a,(hl)
   inc hl
   
   push af
   and $0f
   
   ld (de),a
   inc de
   
   djnz fx_loop
   
   ld de,BANK06_ay_reg + 10
   ld b,3
   
logvol_loop:

   pop af
   
   rlca
   rlca
   rlca
   rlca
   
   and $0f
   ld (de),a
   
   dec de
   djnz logvol_loop

   push hl
   
   call BANK06_ay_rout
   
   pop hl
   ret

;
   
BANK06_ay_rout:

   xor a
   ld c,$fd
   ld hl,BANK06_ay_reg
   
rout_loop:

   ld b,$ff
   out (c),a
   
   ld e,(hl)
   inc hl
   
   ld b,$bf
   out (c),e
   
   inc a
   
   cp 11
   jr nz, rout_loop
   
   ret  
