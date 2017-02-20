==============================================================================

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

==============================================================================

BUILD INSTRUCTIONS:

* Install or update to the current Z88DK

https://github.com/z88dk/z88dk#installation
Git clone if you will regularly update z88dk.
Nightly build if you just want to unzip the current build.

Pietro Bros uses zsdcc so linux users should make sure they also build zsdcc.
zsdcc is included among the pre-built Windows and OSX binaries already.

* Configure the nirvana+ library

Edit file "z88dk/libsrc/_DEVELOPMENT/target/zx/config_nirvanap.m4"
Change "define(`__NIRVANAP_TOTAL_ROWS', 23)" to "define(`__NIRVANAP_TOTAL_ROWS', 19)"

To build the Pentagon version instead of the regular Spectrum version:

Edit file "z88dk/libsrc/_DEVELOPMENT/target/zx/config_target.m4"
Change "define(`__SPECTRUM', 1)" to "define(`__SPECTRUM', 32)"

* Rebuild the zx library so that nirvana+ changes take effect

Open a shell and go to directory "z88dk/libsrc/_DEVELOPMENT"
Run "Winmake zx" (windows) or "make TARGET=zx" (anything else)

* Build Pietro Bros!

Open a shell in the pietro_bros home directory
Run "zcompile" (windows) or "make" then "make zx7" (anything else)

==============================================================================

RUNTIME MEMORY MAP:

23296 - 23551     Stack (256 bytes)
23552 - 55786ish  Pietro Game
56323 - 65378     Nirvana+ Engine (nirvana hole contains program variables)

128K ONLY BANK 6:

49152 - 55587     AY Music, Sound Effects and Players

==============================================================================
