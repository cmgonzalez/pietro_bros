@echo off
@setlocal

set STARTTIME=%time%
echo Start     %STARTTIME%

set "CFLAGS=-SO3 --max-allocs-per-node100000 --opt-code-size"

@rem MAKE BASIC LOADER
echo Creating Basic Loader.
src_tap\bas2tap -sPietro -a10 src_tap\loader.bas loader.tap 1>nul

@rem COPY SCREEN$
echo Copying Loading Screen.
copy /b src_tap\pietro_scr.bin pietro_scr.bin 1>nul

@rem COPY FONT
echo Copying Fonts.
copy /b src_font\pietro.font pietro.font 1>nul

@rem BUILD CONSOLIDATED OBJECT FILE
echo Building Consolidated Object File
zcc +zx -v -c -clib=sdcc_iy %CFLAGS% --fsigned-char -o pietro_bros @zproject.lst

@rem MAKE NORMAL LOADING BINARY
echo Making Normal Loading Binary
zcc +zx -v -m -zorg=23552 -startup=31 -clib=sdcc_iy pietro_bros.o pietro_loader.asm -o pietro_bros -pragma-include:zpragma.inc
appmake +inject -b pietro_bros_NIRVANAP.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627
appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
appmake +zx -b pietro_scr.bin -o pietro_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org 56323 --noloader --noheader
appmake +zx -b pietro_bros_CODE.bin -o pietro.tap --org 23552 --noloader --noheader
appmake +zx -b pietro_bros_BANK_06.bin -o pietro_ay.tap --org 49152 --noloader --noheader
copy /b loader.tap + mcload.tap + mcloader.tap + pietro_scr.tap + nirvanap.tap + pietro.tap + pietro_ay.tap bin\pietro_release.tap

@rem MAKE ZX7 COMPRESSED LOADING BINARY
echo Making ZX7 Compressed Loading Binary
zx7 -f pietro_scr.bin
zx7 -f nirvanap_final.bin
zx7 -f pietro_bros_CODE.bin
zx7 -f pietro_bros_BANK_06.bin
FOR %%A IN ("pietro_scr.bin.zx7") DO set LEN_SCREEN=%%~zA
FOR %%A IN ("nirvanap_final.bin.zx7") DO set LEN_NIRVANAP=%%~zA
FOR %%A IN ("pietro_bros_CODE.bin.zx7") DO set LEN_PIETRO=%%~zA
FOR %%A IN ("pietro_bros_BANK_06.bin.zx7") DO set LEN_BANK_06=%%~zA
echo PUBLIC LEN_SCREEN, LEN_NIRVANAP, LEN_PIETRO, LEN_BANK_06 > zx7_pietro_sizes.asm
echo defc LEN_SCREEN = %LEN_SCREEN% >> zx7_pietro_sizes.asm
echo defc LEN_NIRVANAP = %LEN_NIRVANAP% >> zx7_pietro_sizes.asm
echo defc LEN_PIETRO = %LEN_PIETRO% >> zx7_pietro_sizes.asm
echo defc LEN_BANK_06 = %LEN_BANK_06% >> zx7_pietro_sizes.asm
zcc +zx -v -m -zorg=23552 -startup=31 -clib=sdcc_iy -Ca-DPCOMPRESS pietro_bros.o pietro_loader.asm zx7_pietro_sizes.asm -o pietro_bros -pragma-include:zpragma.inc
appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
appmake +zx -b pietro_scr.bin.zx7 -o pietro_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin.zx7 -o nirvanap.tap --org 56323 --noloader --noheader
appmake +zx -b pietro_bros_CODE.bin.zx7 -o pietro.tap --org 23552 --noloader --noheader
appmake +zx -b pietro_bros_BANK_06.bin.zx7 -o pietro_ay.tap --org 49152 --noloader --noheader
copy /b loader.tap + mcload.tap + mcloader.tap + pietro_scr.tap + nirvanap.tap + pietro.tap + pietro_ay.tap bin\pietro_release_zx7.tap

@rem CLEANUP
echo Cleanup
del *.o *.lis *.bin *.tap *.font *.zx7 pietro_bros zcc_proj.lst zcc_opt.def > nul 2>&1

set ENDTIME=%TIME%

@rem Change formatting for the start and end times
for /F "tokens=1-4 delims=:.," %%a in ("%STARTTIME%") do (
 set /A "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

for /F "tokens=1-4 delims=:.," %%a in ("%ENDTIME%") do (
 set /A "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)
@rem Calculate the elapsed time by subtracting values
set /A elapsed=end-start

@rem Format the results for output
set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100
if %hh% lss 10 set hh=0%hh%
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%

set DURATION=%hh%:%mm%:%ss%,%cc%
echo Elapsed   %DURATION% 
@rem LAUNCH
