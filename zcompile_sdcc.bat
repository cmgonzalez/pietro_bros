@echo off

del pietro_release_sdcc.tap > nul 2>&1

@rem SET ENVIRONMENT VARIABLES FOR Z88DK
SET ZCCCFG=C:\z88dk\lib\config
PATH=C:\z88dk\bin;%PATH%

@rem SET VARIOUS ORG ADDRESSES

set "LADDR=23296"   %= ORG of Loader =%
set "PADDR=23552"   %= ORG of Pietro binary =%
set "NADDR=56323"   %= ORG of Nirvana+ binary (fixed) =%

@rem MAKE BASIC LOADER

cd src_tap
bas2tap -sPietro -a10 loader.bas loader_bas.tap
copy /b loader_bas.tap ..\loader.tap

@rem COPY SCREEN$

copy /b pietro_scr.bin ..\pietro_scr.bin
cd ..

@rem COPY FONT

cd src_font
copy /b pietro.font ..\pietro.font
cd ..

@rem ASSEMBLE NIRVANA

cd src_nirvana
pasmo nirvana+.asm nirvanap.bin
copy /b nirvanap.bin ..\nirvanap.bin
cd ..

@rem COMPILE PROGRAM
@rem zorg overrides org set in zpragma.inc

zcc +zx -vn -zorg=%PADDR% -startup=31 -SO3 -clib=sdcc_iy --max-allocs-per-node200000 --opt-code-size --fsigned-char @zproject.lst -o pietro_bros -pragma-include:zpragma.inc
@rem zcc +zx -vn -zorg=%PADDR% -startup=31 -O3 -clib=new @zproject.lst -o pietro_bros -pragma-include:zpragma.inc

@rem INJECT SOME CODE AND RAM VARIABLES INTO NIRVANA HOLE
@rem hole offset = 56718+328*TOTAL_ROWS-56323

appmake +inject -b nirvanap.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627

@rem CREATE TAPS OUT OF EACH BINARY

appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org %LADDR% --noloader --noheader
appmake +zx -b pietro_scr.bin -o pietro_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org %NADDR% --noloader --noheader
appmake +zx -b pietro_bros_CODE.bin -o pietro.tap --org %PADDR% --noloader --noheader
appmake +zx -b pietro_bros_BANK_06.bin -o pietro_ay.tap --org 49152 --noloader --noheader

@rem MAKE FINAL TAP

copy /b /Y loader.tap + mcload.tap + mcloader.tap + pietro_scr.tap + nirvanap.tap + pietro.tap + pietro_ay.tap tap\pietro_release_sdcc.tap

del loader.tap mcload.tap mcloader.tap nirvanap.tap nirvanap.bin nirvanap_final.bin > nul 2>&1
del pietro.font pietro.tap pietro_bros pietro_bros_CODE.bin > nul 2>&1
del pietro_bros_MCLOAD.bin pietro_bros_LOADER.bin pietro_bros_NIRVANA_HOLE.bin > nul 2>&1
del pietro_scr.bin pietro_scr.tap pietro_bros_BANK_06.bin pietro_ay.tap > nul 2>&1

@rem LAUNCH
