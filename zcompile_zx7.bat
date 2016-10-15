@echo off

del pietro_release_zx7.tap > nul 2>&1

@rem SET ENVIRONMENT VARIABLES FOR Z88DK
SET ZCCCFG=C:\z88dk\lib\config
PATH=C:\z88dk\bin;%PATH%

@rem SET VARIOUS ORG ADDRESSES

set "LADDR=65377"   %= ORG of Loader =%
set "PADDR=23296"   %= ORG of Pietro binary =%
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

@rem MAKE A CONSOLIDATED OBJECT FILE OUT OF THE SOURCE FILES
@rem the object file will make it possible to compile twice and produce exactly the same binaries
@rem there are three invocations of zcc below and all must use the same compiler

@rem zcc +zx -vn -c -O3 -clib=new @zx7_zproject.lst -o pietro_bros
zcc +zx -vn -c -SO3 -clib=sdcc_iy --max-allocs-per-node200000 --opt-code-size --fsigned-char @zx7_zproject.lst -o pietro_bros

@rem COLLECT FILE SIZES
@rem compressed file sizes are unknown in the first compile so supply a dummy file

copy /Y zx7_pietro_sizes.bak zx7_pietro_sizes.asm

@rem COMPILE PROGRAM FIRST TIME USING OBJECT FILE
@rem zorg overrides org set in zpragma.inc

@rem zcc +zx -vn -zorg=%PADDR% -startup=31 -O3 -clib=new zx7_pietro_sizes.asm pietro_bros.o -o pietro_bros -pragma-include:zpragma.inc
zcc +zx -vn -zorg=%PADDR% -startup=31 -SO3 -clib=sdcc_iy --max-allocs-per-node200000 --opt-code-size --fsigned-char zx7_pietro_sizes.asm pietro_bros.o -o pietro_bros -pragma-include:zpragma.inc

@rem INJECT SOME CODE AND RAM VARIABLES INTO NIRVANA HOLE
@rem hole offset = 56718+328*TOTAL_ROWS-56323

appmake +inject -b nirvanap.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627

@rem COMPRESS BINARY FILES

zx7 -f pietro_scr.bin
zx7 -f nirvanap_final.bin
zx7 -f pietro_bros_CODE.bin

@rem COLLECT FILE SIZES

FOR %%A IN ("pietro_scr.bin.zx7") DO set LEN_SCREEN=%%~zA
FOR %%A IN ("nirvanap_final.bin.zx7") DO set LEN_NIRVANAP=%%~zA
FOR %%A IN ("pietro_bros_CODE.bin.zx7") DO set LEN_PIETRO=%%~zA

echo PUBLIC LEN_SCREEN, LEN_NIRVANAP, LEN_PIETRO > zx7_pietro_sizes.asm
echo defc LEN_SCREEN = %LEN_SCREEN% >> zx7_pietro_sizes.asm
echo defc LEN_NIRVANAP = %LEN_NIRVANAP% >> zx7_pietro_sizes.asm
echo defc LEN_PIETRO = %LEN_PIETRO% >> zx7_pietro_sizes.asm
echo EXTERN pietro_loader >> zx7_pietro_sizes.asm
echo defc NEED_LOADER = pietro_loader >> zx7_pietro_sizes.asm

@rem COMPILE PROGRAM SECOND TIME WITH FILE SIZES CORRECTLY DEFINED
@rem zorg overrides org set in zpragma.inc
@rem only the loader section changes in this compile and that's the only section that will be used from this compile

@rem zcc +zx -vn -zorg=%PADDR% -startup=31 -O3 -clib=new zx7_pietro_sizes.asm pietro_bros.o -o pietro_bros -pragma-include:zpragma.inc
zcc +zx -vn -zorg=%PADDR% -startup=31 -SO3 -clib=sdcc_iy --max-allocs-per-node200000 --opt-code-size --fsigned-char zx7_pietro_sizes.asm pietro_bros.o -o pietro_bros -pragma-include:zpragma.inc

@rem CREATE TAPS OUT OF EACH BINARY

appmake +zx -b pietro_bros_LOADER.bin -o mcload.tap --blockname mcload --org %LADDR% --noloader
appmake +zx -b pietro_scr.bin.zx7 -o pietro_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin.zx7 -o nirvanap.tap --org %NADDR% --noloader --noheader
appmake +zx -b pietro_bros_CODE.bin.zx7 -o pietro.tap --org %PADDR% --noloader --noheader

@rem MAKE FINAL TAP

copy /b /Y loader.tap + mcload.tap + pietro_scr.tap + nirvanap.tap + pietro.tap pietro_release_zx7.tap

del loader.tap mcload.tap nirvanap.tap nirvanap.bin nirvanap_final.bin nirvanap_final.bin.zx7 > nul 2>&1
del pietro_bros_CODE.bin pietro_bros_CODE.bin.zx7 pietro.font pietro.tap pietro_bros > nul 2>&1
del pietro_bros_LOADER.bin pietro_bros_NIRVANA_HOLE.bin > nul 2>&1
del pietro_scr.bin pietro_scr.bin.zx7 pietro_scr.tap > nul 2>&1

@rem LAUNCH
