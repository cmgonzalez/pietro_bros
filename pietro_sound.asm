
; various sound effects

SECTION code_user

EXTERN _game_sound_48, asm_bit_beep_raw

;; NOTE:
;;
;; Originally these called into rom routine 0x03b5 to generate
;; tones but now z88dk replacement "asm_bit_beep_raw" is used
;; to avoid having the rom fiddle with the system variables
;; area.
;;
;; However the rom routine is actually bugged, generating one
;; more cycle of tone than asked so the conversion to the
;; fixed z88dk replacement must have one added to the cycle
;; count.

_sound_nirvana_halt:

   ld a,(_game_sound_48)
   
   or a
   ret z
   
   halt
   ret

   
PUBLIC _sound_slide

_sound_slide:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,400
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,0
   call asm_bit_beep_raw

   ei
   ret
   
   
PUBLIC _sound_jump

_sound_jump:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,400
   call asm_bit_beep_raw

   ld de,1+1
   ld hl,400
   call asm_bit_beep_raw

   ld de,1+1
   ld hl,400
   call asm_bit_beep_raw

   ld de,1+1
   ld hl,0
   call asm_bit_beep_raw

   ei
   ret

   
PUBLIC _sound_coin

_sound_coin:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,1024
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,256
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,1024
   call asm_bit_beep_raw

   ei
   ret
   
   
PUBLIC _sound_enter_enemy

_sound_enter_enemy:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,400
   call asm_bit_beep_raw

   ei
   ret

   
PUBLIC _sound_kill

_sound_kill:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,1024
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,2048
   call asm_bit_beep_raw
   
   ei
   ret
   
   
PUBLIC _sound_hit_enemy

_sound_hit_enemy:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,0
   call asm_bit_beep_raw
   
   ld de,6+1
   ld hl,256
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,0
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,4096
   call asm_bit_beep_raw
   
   ei
   ret
   
   
PUBLIC _sound_hit_brick

_sound_hit_brick:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,0
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,512
   call asm_bit_beep_raw
   
   ei
   ret
   
   
PUBLIC _sound_hit_pow

_sound_hit_pow:

   call _sound_nirvana_halt
   ret z
   
   di
   
   ld de,1+1
   ld hl,512
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,1024
   call asm_bit_beep_raw
   
   ld de,1+1
   ld hl,512
   call asm_bit_beep_raw

   ei
   ret
