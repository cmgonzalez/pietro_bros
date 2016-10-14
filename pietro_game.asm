SECTION code_user

; void game_fill_row(unsigned char f_row, unsigned char f_asc)
; callee linkage

PUBLIC _game_fill_row

EXTERN _pietro_font, _screen_ink, _screen_paper
EXTERN asm_zx_saddr2aaddr, asm_zx_cyx2saddr

_game_fill_row:

IF __SDCC

   pop hl
   ex (sp),hl                  ; l = row, h = ascii
   
   ld e,h
   ld d,0                      ; de = ascii
   
   ld h,l
   ld l,d                      ; h = row, l = 0

ENDIF

IF __SCCZ80

   pop hl
   pop de                      ; de = ascii
   ex (sp),hl                  ; l = row
   
   ld h,l
   ld l,0                      ; h = row, l = 0
   
ENDIF

game_fill_row:

   ; de = ascii code
   ;  h = row
   ;  l = 0
   
   call asm_zx_cyx2saddr       ; z88dk function: character y,x to screen address
   ex de,hl
   
   add hl,hl
   add hl,hl
   add hl,hl
   ld bc, _pietro_font - 256
   add hl,bc

   ld b,8

gf_ploop:

   ;  b = remaining pixel rows
   ; de = char *screen
   ; hl = char *glyph

   push bc
   push de
   push hl
   
   ld a,(hl)
   
   ld l,e
   ld h,d
   inc e
   
   ld (hl),a
   
   ld bc,31
   ldir
   
   pop hl
   pop de
   pop bc
   
   inc hl
   inc d
   
   djnz gf_ploop

   dec d
   ex de,hl
   
   call asm_zx_saddr2aaddr     ; z88dk function: screen address to attribute address
   
   ld a,(_screen_ink)
   ld b,a
   ld a,(_screen_paper)
   or b
   
   ld (hl),a
   
   ld e,l
   ld d,h
   inc e
   
   ld bc,31
   ldir
   
   ret
