@echo off
bas2tap -sPietro -a10 loader.bas loader_bas.tap
copy /b loader_bas.tap + pietro_scr.tap + pietro_font.tap + nirvana_code.tap loader.tap
move loader.tap ..\
rem pause