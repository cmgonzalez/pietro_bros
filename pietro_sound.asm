; various sound effects

SECTION code_user

EXTERN _game_sound, asm_bit_beep_raw


_sound_nirvana_halt:

   ld a,(_game_sound)
   
   and $01
   ret

   
PUBLIC _sound_slide

_sound_slide:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,400
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,0
   call asm_bit_beep_raw

   halt
   ret
   
   
PUBLIC _sound_jump

_sound_jump:

   call _sound_nirvana_halt
   ret z

   ld de,1
   ld hl,400
   call asm_bit_beep_raw

   ld de,1
   ld hl,400
   call asm_bit_beep_raw

   ld de,1
   ld hl,400
   call asm_bit_beep_raw
	
   ld de,1
   ld hl,0
   call asm_bit_beep_raw

   halt
   ret

   
PUBLIC _sound_coin

_sound_coin:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,1024
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,256
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,1024
   call asm_bit_beep_raw

   halt
   ret
   
   
PUBLIC _sound_enter_enemy

_sound_enter_enemy:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,400
   call asm_bit_beep_raw

   halt
   ret

   
PUBLIC _sound_kill

_sound_kill:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,1024
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,2048
   call asm_bit_beep_raw
   
   halt
   ret
   
   
PUBLIC _sound_hit_enemy

_sound_hit_enemy:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,0
   call asm_bit_beep_raw
   
   ld de,6
   ld hl,256
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,0
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,4096
   call asm_bit_beep_raw
   
   halt
   ret
   
   
PUBLIC _sound_hit_brick

_sound_hit_brick:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,0
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,512
   call asm_bit_beep_raw
   
   halt
   ret
   
   
PUBLIC _sound_hit_pow

_sound_hit_pow:

   call _sound_nirvana_halt
   ret z
   
   ld de,1
   ld hl,512
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,1024
   call asm_bit_beep_raw
   
   ld de,1
   ld hl,512
   call asm_bit_beep_raw

   halt
   ret
