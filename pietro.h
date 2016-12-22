#ifndef PIETRO_H
#define PIETRO_H

/*				ENEMIES INDEXES						*/

#define SHELLCREEPER_GREEN							1
#define SHELLCREEPER_RED							2
#define SHELLCREEPER_BLUE							3
#define SIDESTEPPER_RED								4
#define SIDESTEPPER_GREEN							5
#define SIDESTEPPER_MAGENTA							6
#define SLIPICE										7
#define COIN_1										8
#define FIGHTERFLY									9
#define FIREBALL_RED								10
#define FIREBALL_GREEN								11
#define COIN_2										12
#define PLAYER										16

/*				PLAYER 1 TILES						*/

#define TILE_P1_STANR								3
#define TILE_P1_RIGHT								0
#define TILE_P1_SLIDR								4
#define TILE_P1_JUMPR								7
#define TILE_P1_KILL								10
#define TILE_P1_HITR								11
//#define TILE_P1_LEFT								12
//#define TILE_P1_SLIDL								16
//#define TILE_P1_JUMPL								19
//#define TILE_P1_STANL								15

/*				ENEMIES TILES						*/

#define TILE_SHELLCREEPER_GREEN						48
#define TILE_SHELLCREEPER_RED						60
#define TILE_SHELLCREEPER_BLUE						72
#define TILE_SIDESTEPPER_RED						84
#define TILE_SIDESTEPPER_GREEN						96
#define TILE_SIDESTEPPER_MAGENTA						108
#define TILE_SLIPICE								123
#define TILE_FIGHTERFLY								120
#define TILE_FIREBALL_RED							144
#define TILE_FIREBALL_GREEN							150

#define TILE_COIN1									180
#define TILE_COIN2									183
#define TILE_800_COIN								186
#define TILE_DOLLAR									187
#define TILE_800									188
#define TILE_NICE									189
#define TILE_EXTRA									190

#define TILE_PRINCESS								202
#define TILE_CASTLE									214
#define TILE_GRASS									140

/*				SCREEN TILES						*/
#define TILE_BRICK0									132
#define TILE_BRICK1									135
#define TILE_BRICK2									136
#define TILE_BRICK3									137
#define TILE_BRICK4									138
#define TILE_BRICK_FREEZE							139
#define TILE_BRICK_RESTART							132
#define TILE_BRICK_FILL								135


#define TILE_PIPE1									101
#define TILE_PIPE2									179
#define TILE_PIPE3									167
#define TILE_PIPE4									177
#define TILE_PIPE5									165
#define TILE_PIPE6									178
#define TILE_PIPE7									166
#define TILE_PIPE1A									163
#define TILE_PIPE2A									164
#define TILE_PIPE3A									162
#define TILE_PIPE1B									175
#define TILE_PIPE2B									176
#define TILE_PIPE3B									174

#define TILE_POW1									191
#define TILE_EMPTY									169


/*				PLAYER INDEXES						*/

#define SPR_P1										7
#define SPR_P2										6

/*				SCREEN VARIABLES					*/

#define SPR_COLINT									3	//INTERNAL COLUM INCREMENT

//START COLUMN

#define SCR_COLI									1

//COLUMNS MAX

#define SCR_COLS									32
#define SCR_COLS_M									30

//START ROW

#define SCR_ROWI									0

//ROWS MAX

#define SCR_ROWS									23

//LINS MAX

#define SCR_LINS									184

//SCREEN PAPER

#define PAPER										000
#define LIN_INC										4
#define GAME_LIN_FLOOR								152
#define ST_STAND_R									0
#define ST_STAND_L									1
#define ST_TURN_R									10
#define ST_TURN_L									11
#define VAL_COL										16
#define IDX_BRICK									18
#define IDX_POW										17

/*DEBUG*/


#define FULL_SPEED									0

/*STARTING POSITIONS*/

#define ENEMY_SCOL_R								26
#define ENEMY_SLIN_R								16
#define ENEMY_SCOL_L								4
#define ENEMY_SLIN_L								16
#define DIR_LEFT									255
#define DIR_RIGHT									1

/*OBJECTS STATS (BIT POSITION NUMBER)*/
#define STAT_JUMP									0
#define STAT_FALL									1
#define STAT_DIRR									2
#define STAT_DIRL									3
#define STAT_HIT									4
#define STAT_KILL									5
#define STAT_ANGRY									6
#define STAT_UPGR									7
/*OBJECTS STATS ALT (BIT POSITION NUMBER)*/
#define STAT_LOCK									0
#define STAT_TURN									1
#define STAT_PUSH									2



/*GAME VARIABLES*/
#define TIME_BONUS									2000   //20 SECONDS IN HUNDREDTHS (ORIG IS 30, TO EASY 4 PIETRO)

#define TIME_FIREBALL_RED							1500 //30 SECONDS (50HZ)

#define TIME_FIREBALL_A								2000 //40 - ORIG 80 SECONDS (50HZ)
#define TIME_FIREBALL_B								500  //15 - ORIG 60 SECONDS (50HZ)

#define TIME_FLIPPED_A								500  //15 - ORIG 20 SECONDS (50HZ)
#define TIME_FLIPPED_B								250  //7  - ORIG 15 SECONDS (50HZ)

#define PLAYER_SLIDE_NORMAL							4
#define PLAYER_SLIDE_ICE							16
#define PLAYER_MAX_JUMP								48
#define PLAYER_HIT_BRICK_TIME						8


#define ENEMIES_MAX									6   //MAX QUANTITY OF ENEMIES ON SCREEN (0->6)
#define GAME_EXTRA_LIFE								2000
#define GAME_TIME_WATER_SPLASH						15

#define MAZE_BRICK									18
#define MAZE_BRICK_FREEZE							20
/*LOW MEMORY USAGE FOR FASTER COMPILATION*/
#define GAME_LOW_MEM								0

#endif

#include "globals.h"
