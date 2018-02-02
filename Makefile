

CSRC = globals.c pietro.c pietro_enemies.c pietro_game.c pietro_player.c pietro_sprite.c pietro_zx.c
ASRC = btiles.asm font.asm pietro_ay.asm pietro_ay_res_fx.asm pietro_ay_res_midi.asm pietro_game.asm pietro_sound.asm pietro_zx.asm

OBJS = $(CSRC:.c=.o)
INCL = $(ZCCCFG)/../../include/_DEVELOPMENT/sdcc

OUT = bin/pietro_release.tap
OUTC = bin/pietro_release_zx7.tap

//CFLAGS = -SO3 --max-allocs-per-node200000 --opt-code-size
CFLAGS = -SO3 --max-allocs-per-node10000 --opt-code-size

.PHONY: depend clean

default: loader.tap pietro_scr.bin pietro.font $(OUT)
	-ls -l *.bin
	-rm -f *.bin *.tap *.font *.zx7 pietro_bros zcc_proj.lst zcc_opt.def
	grep code_compiler_size pietro_bros.map

zx7: loader.tap pietro_scr.bin pietro.font $(OUTC)
	-ls -l *.bin
	-rm -f *.bin *.tap *.font *.zx7 pietro_bros zcc_proj.lst zcc_opt.def
	grep code_compiler_size pietro_bros.map

loader.tap: src_tap/loader.bas
	src_tap/bas2tap -sPietro -a10 src_tap/loader.bas loader.tap > /dev/null

pietro_scr.bin: src_tap/pietro_scr.bin
	cp src_tap/pietro_scr.bin pietro_scr.bin

pietro.font: src_font/pietro.font
	cp src_font/pietro.font pietro.font

$(OUT): $(OBJS) $(ASRC) mmap.inc
	-rm -f $(OUT)
	zcc +zx -vn -m -zorg=23552 -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) pietro_loader.asm -o pietro_bros -pragma-include:zpragma.inc
	appmake +inject -b pietro_bros_NIRVANAP.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627
	appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b pietro_scr.bin -o pietro_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b pietro_bros_CODE.bin -o pietro.tap --org 23552 --noloader --noheader
	appmake +zx -b pietro_bros_BANK_06.bin -o pietro_ay.tap --org 49152 --noloader --noheader
	cat loader.tap mcload.tap mcloader.tap pietro_scr.tap nirvanap.tap pietro.tap pietro_ay.tap > $(OUT)
	grep code_compiler_size pietro_bros.map

$(OUTC): $(OBJS) $(ASRC) mmap.inc
	-rm -f $(OUTC)
	zcc +zx -vn -zorg=23552 -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) pietro_loader.asm -o pietro_bros -pragma-include:zpragma.inc
	appmake +inject -b pietro_bros_NIRVANAP.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627
	zx7 -f pietro_scr.bin
	zx7 -f nirvanap_final.bin
	zx7 -f pietro_bros_CODE.bin
	zx7 -f pietro_bros_BANK_06.bin
	echo PUBLIC LEN_SCREEN, LEN_NIRVANAP, LEN_PIETRO, LEN_BANK_06 > zx7_pietro_sizes.asm
	echo defc LEN_SCREEN = $(shell wc -c < pietro_scr.bin.zx7) >> zx7_pietro_sizes.asm
	echo defc LEN_NIRVANAP = $(shell wc -c < nirvanap_final.bin.zx7) >> zx7_pietro_sizes.asm
	echo defc LEN_PIETRO = $(shell wc -c < pietro_bros_CODE.bin.zx7) >> zx7_pietro_sizes.asm
	echo defc LEN_BANK_06 = $(shell wc -c < pietro_bros_BANK_06.bin.zx7) >> zx7_pietro_sizes.asm
	zcc +zx -vn -zorg=23552 -startup=31 -clib=sdcc_iy -Ca-DPCOMPRESS $(OBJS) $(ASRC) zx7_pietro_sizes.asm pietro_loader.asm -o pietro_bros -pragma-include:zpragma.inc
	appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b pietro_scr.bin.zx7 -o pietro_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin.zx7 -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b pietro_bros_CODE.bin.zx7 -o pietro.tap --org 23552 --noloader --noheader
	appmake +zx -b pietro_bros_BANK_06.bin.zx7 -o pietro_ay.tap --org 49152 --noloader --noheader
	cat loader.tap mcload.tap mcloader.tap pietro_scr.tap nirvanap.tap pietro.tap pietro_ay.tap > $(OUTC)
	grep code_compiler_size pietro_bros.map

.c.o:
	zcc +zx -vn --list -c -clib=sdcc_iy $(CFLAGS) --fsigned-char $<

clean:
	-rm -f *.o *.lis *.bin *.tap *.font *.map *.zx7 pietro_bros zcc_proj.lst zcc_opt.def

depend: $(CSRC)
	makedepend $(INCL) $^

# DO NOT DELETE THIS LINE -- make depend needs it
