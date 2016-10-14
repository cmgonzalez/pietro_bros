SECTION LOADER

;; Machine Code Loader

PUBLIC pietro_loader
PUBLIC l_ret

EXTERN asm_dzx7_standard
EXTERN ASMHEAD_CODE, ASMTAIL_CODE_END
EXTERN LEN_SCREEN, LEN_NIRVANAP, LEN_PIETRO

pietro_loader:

   ; 1. SCREEN$
   
   ld ix,32768                 ; load compressed screen$ to convenient spot
   ld de,LEN_SCREEN
   
   ld a,$ff
   scf
   
   call 0x0556
   jr nc, error
   
   ld hl,32768
   ld de,16384
  
   call asm_dzx7_standard      ; decompress to 16384
   
   ; 2. Nirvana+
   
   ld ix,32768                 ; load compressed nirvana+ to convenient spot
   ld de,LEN_NIRVANAP
   
   ld a,$ff
   scf
   
   call 0x0556
   jr nc, error
   
   ld hl,32768
   ld de,56323
   
   call asm_dzx7_standard      ; decompress to 56323
   
   ; 3. Pietro Bros
   
   ld sp,56323                                   ; move stack underneath nirvana
   ld iy,128                                     ; move IY into ROM in case an im1 interrupt occurs
   
   ld ix,ASMTAIL_CODE_END + 20 - LEN_PIETRO      ; load binary with really large & fixed 20 byte delta (see zx7 docs)
   ld de,LEN_PIETRO                              ; length of binary

   ld a,$ff
   scf

   call 0x0556
   
   di
   jp nc, 0                    ; nothing to do on error except reset
   
   ld hl,ASMTAIL_CODE_END + 20 - LEN_PIETRO
   ld de,ASMHEAD_CODE
   
   call asm_dzx7_standard      ; perform overlapped decompression
   
   jp ASMHEAD_CODE             ; execute pietro bros

error:

   rst 0x08
   defb $1a                    ; R Tape Loading Error

;; l_ret is a library function used as an easy exit with built-in pop
;; asm_dzx7_standard calls into it so we place it here so that the
;; library function will not be placed elsewhere in ram during linking
   
   pop hl

l_ret:

   ret
