

CSRC = globals.c pietro.c pietro_enemies.c pietro_game.c pietro_player.c pietro_sprite.c pietro_zx.c
ASRC = btiles.asm font.asm pietro_ay.asm pietro_ay_res_fx.asm pietro_ay_res_midi.asm pietro_game.asm pietro_loader.asm pietro_sound.asm pietro_zx.asm

OBJS = $(CSRC:.c=.o)
INCL = $(ZCCCFG)/../../include/_DEVELOPMENT/sdcc

OUTF = bin/pietro_release.tap
OUTG = bin\pietro_release.tap

CFLAGS = -SO3 --max-allocs-per-node200000 --opt-code-size

.PHONY: depend clean

default: loader.tap pietro_scr.bin pietro.font nirvanap.bin $(OUTF)
	-del *.bin *.tap *.font pietro_bros zcc_proj.lst zcc_opt.def > nul 2>&1

loader.tap: src_tap/loader.bas
	src_tap\bas2tap -sPietro -a10 src_tap\loader.bas loader.tap 1>nul

pietro_scr.bin: src_tap/pietro_scr.bin
	copy /b src_tap\pietro_scr.bin pietro_scr.bin 1>nul

pietro.font: src_font/pietro.font
	copy /b src_font\pietro.font pietro.font 1>nul

nirvanap.bin: src_nirvana/nirvana+.asm
	src_nirvana\pasmo src_nirvana\nirvana+.asm nirvanap.bin

$(OUTF): $(OBJS) mmap.inc
	-del $(OUTG) > nul 2>&1
	zcc +zx -vn -zorg=23552 -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) -o pietro_bros -pragma-include:zpragma.inc
	appmake +inject -b nirvanap.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627
	appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b pietro_scr.bin -o pietro_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b pietro_bros_CODE.bin -o pietro.tap --org 23552 --noloader --noheader
	appmake +zx -b pietro_bros_BANK_06.bin -o pietro_ay.tap --org 49152 --noloader --noheader
	copy /b /Y loader.tap + mcload.tap + mcloader.tap + pietro_scr.tap + nirvanap.tap + pietro.tap + pietro_ay.tap $(OUTG)  1>nul

.c.o:
	zcc +zx -vn -c -clib=sdcc_iy $(CFLAGS) --fsigned-char $<

clean:
	-del *.o *.bin *.tap *.font pietro_bros zcc_proj.lst zcc_opt.def > nul 2>&1

depend: $(CSRC)
	makedepend $(INCL) $^

# DO NOT DELETE THIS LINE -- make depend needs it
