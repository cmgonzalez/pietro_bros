SECTION LOADER

;; Machine Code Loader

EXTERN ASMHEAD_CODE, ASMTAIL_CODE_END

loader:

   ; 1. SCREEN$
   
   ld ix,16384
   ld de,6912
   
   ld a,$ff
   scf
   
   call 0x0556
   jr nc, error
   
   ; 2. Nirvana+
   
   ld ix,56323
   ld de,9055
   
   ld a,$ff
   scf
   
   call 0x0556
   jr nc, error
   
   ; 3. Pietro Bros
   
   ld sp,56323                                   ; move stack underneath nirvana
   ld iy,128                                     ; move IY into ROM in case an im1 interrupt occurs
   
   ld ix,ASMHEAD_CODE                            ; start of binary
   ld de,ASMTAIL_CODE_END - ASMHEAD_CODE         ; length of binary

   ld a,$ff
   scf

   call 0x0556
   
   di
   jp nc, 0                    ; nothing to do on error except reset

   jp ASMHEAD_CODE             ; execute pietro bros

error:

   rst 0x08
   defb $1a                    ; R Tape Loading Error
