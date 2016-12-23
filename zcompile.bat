@echo off
@setlocal 
set STARTTIME=%time%
echo Start     %STARTTIME%
del bin\pietro_release_sdcc_low.tap > nul 2>&1

@rem SET ENVIRONMENT VARIABLES FOR Z88DK
SET ZCCCFG=C:\z88dk\lib\config
PATH=C:\z88dk\bin;%PATH%

@rem SET VARIOUS ORG ADDRESSES

set "LADDR=23296"   %= ORG of Loader =%
set "PADDR=23552"   %= ORG of Pietro binary =%
set "NADDR=56323"   %= ORG of Nirvana+ binary (fixed) =%

@rem MAKE BASIC LOADER
echo Creating  Bas Loaders.
cd src_tap
bas2tap -sPietro -a10 loader.bas loader_bas.tap 1>nul
copy /b loader_bas.tap ..\loader.tap 1>nul

@rem COPY SCREEN$
echo Compiling Loading Screen.
copy /b pietro_scr.bin ..\pietro_scr.bin 1>nul
cd ..

@rem COPY FONT
echo Compiling Fonts.
cd src_font
copy /b pietro.font ..\pietro.font 1>nul
cd ..

@rem ASSEMBLE NIRVANA
echo Compiling Nirvana+.
cd src_nirvana
pasmo nirvana+.asm nirvanap.bin
copy /b nirvanap.bin ..\nirvanap.bin 1>nul
cd ..

@rem COMPILE PROGRAM FASTER
@rem zorg overrides org set in zpragma.inc

if "%~1"=="fast" (
  echo Compiling Pietro Bros - Fast Mode.
  zcc +zx -vn -zorg=%PADDR% -startup=31 -SO3 -clib=sdcc_iy --max-allocs-per-node10000 --fsigned-char @zproject.lst -o pietro_bros -pragma-include:zpragma.inc
) else (
  echo Compiling Pietro Bros - Release Mode.
  zcc +zx -vn -zorg=%PADDR% -startup=31 -SO3 -clib=sdcc_iy --max-allocs-per-node200000 --opt-code-size --fsigned-char @zproject.lst -o pietro_bros -pragma-include:zpragma.inc
)

@rem zcc +zx -vn -zorg=%PADDR% -startup=31 -O3 -clib=new @zproject.lst -o pietro_bros -pragma-include:zpragma.inc

@rem INJECT SOME CODE AND RAM VARIABLES INTO NIRVANA HOLE
@rem hole offset = 56718+328*TOTAL_ROWS-56323

appmake +inject -b nirvanap.bin -o nirvanap_final.bin -i pietro_bros_NIRVANA_HOLE.bin --offset 6627

@rem CREATE TAPS OUT OF EACH BINARY
echo Compiling Taps.
appmake +zx -b pietro_bros_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
appmake +zx -b pietro_bros_LOADER.bin -o mcloader.tap --org %LADDR% --noloader --noheader
appmake +zx -b pietro_scr.bin -o pietro_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org %NADDR% --noloader --noheader
appmake +zx -b pietro_bros_CODE.bin -o pietro.tap --org %PADDR% --noloader --noheader
appmake +zx -b pietro_bros_BANK_06.bin -o pietro_ay.tap --org 49152 --noloader --noheader

@rem MAKE FINAL TAP
echo Merging   Taps.
if "%~1"=="fast" (
  copy /b /Y loader.tap + mcload.tap + mcloader.tap + pietro_scr.tap + nirvanap.tap + pietro.tap + pietro_ay.tap bin\pietro_debug.tap  1>nul
) else (
  copy /b /Y loader.tap + mcload.tap + mcloader.tap + pietro_scr.tap + nirvanap.tap + pietro.tap + pietro_ay.tap bin\pietro_release.tap  1>nul
)
echo Cleaning
del loader.tap mcload.tap mcloader.tap nirvanap.tap nirvanap.bin nirvanap_final.bin > nul 2>&1
del pietro.font pietro.tap pietro_bros pietro_bros_CODE.bin > nul 2>&1
del pietro_bros_MCLOAD.bin pietro_bros_LOADER.bin pietro_bros_NIRVANA_HOLE.bin > nul 2>&1
del pietro_scr.bin pietro_scr.tap pietro_bros_BANK_06.bin pietro_ay.tap > nul 2>&1
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
