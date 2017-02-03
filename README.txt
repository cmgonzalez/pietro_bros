README - Pietro Bros v1.2

Pietro and Luizo were transported to pipe world after eating some rancid 
mushrooms on his pizza.

Help them to escape from this hell...

http://imgur.com/a/d6fQZ

http://i.imgur.com/5Yk0gfs.png
http://i.imgur.com/YF5cm2y.png
http://i.imgur.com/HrkpQEG.png
http://i.imgur.com/ZP7TAFS.png


Based on Mario Bros from the NES.
One or Two player Game
32 Levels of Fun.
Three modes of Play.
NIRVANA+ Multicolour.
48 Sounds.
AY Music and Effects
Sinclair Joystick Only.
Spectrum 48kb, 128kb and Pentagon Compatible.
More than a year of work.


This games was build on z88dk and using NIRVANA+ for the multicolour graphic
engine.

I really like to thanks Alvin Albrecht for his help, most of the magic come
from his fingers. Also Einar Saukas for his marvelous Nirvana Library!


This is my christmas gift to all the Spectrum lovers!

Merry Christmas, Cheers!

2016 Cristian M. Gonzalez

Version 1.2


BUILD INSTRUCTIONS:

* Configure Nirvana+

Using latest z88dk, edit z88dk/libsrc/_DEVELOPMENT/target/zx/clib_target_cfg.asm
to set "__NIRVANAP_TOTAL_ROWS" equal to 19 and set "__spectrum" to 3 if
building the pentagon version.

* Build ZX Library

Open shell, cd to z88dk/libsrc/_DEVELOPMENT and run "Wimake zx" on windows or
"make TARGET=zx" on other machines.

* Build Pietro Bros

cd to this directory, type "make".  The tap file will be generated into the
bin subdirectory.
