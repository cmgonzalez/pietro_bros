/*
	This file is part of Pietro Bros.

	Pietro Bros is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Pietro Bros is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Pietro Bros.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <input.h>
#include "pietro.h"
#include "globals.h"

unsigned char spec128;

//###############################################################################################
//#                                                                                             #
//# CONTROL VARIABLES                                                                           #
//#                                                                                             #
//###############################################################################################

const char *joynames[] = { "SJ1", "SJ2", "KB1", "KB2", "KEM", "CUR", "FUL" };
uint16_t (*joyfunc1)(udk_t *);			// pointer to joystick function Player 1
uint16_t (*joyfunc2)(udk_t *);			// pointer to joystick function Player 1
udk_t k1;
udk_t k2;
unsigned char dirs;
unsigned char tbuffer[7];			    // temporary buffer

const JOYFUNC control_method[7] = {
   (JOYFUNC)(in_stick_sinclair1),
   (JOYFUNC)(in_stick_sinclair2),
   (JOYFUNC)(in_stick_keyboard),
   (JOYFUNC)(in_stick_keyboard),
   (JOYFUNC)(in_stick_kempston),
   (JOYFUNC)(in_stick_cursor),
   (JOYFUNC)(in_stick_fuller)
};


//SPRITES GAME ARRAYS
unsigned char class[8];					//CLASS OF SPRITE
unsigned char state[8];					//SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8];				//SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char tile[8];					//TILE
unsigned char lin[8];					//LINE
unsigned char col[8];					//COLUMNlisto
unsigned char colint[8];				//INTERNAL COLUMN/TILE INCREMENT
unsigned int  spr_timer[8];				//SPRITE GENERAL TIMER MILISECONDS
//unsigned int  spr_timer_c[8];			//SPRITE COLITIONS CHECK TIMER MILISECONDS
unsigned int  last_time[8];				//LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8];				//START JUMP LINE

// PLAYER ONLY
unsigned char hit_lin[2];				//HIT BRICK LINE
unsigned char hit_col[2];				//HIT BRICK COL
unsigned char player_jump_c[2];			//JUMP CNT TIME y = a*t^2 + b*t + c https://www.wired.com/2016/12/lets-go-physics-jumping-super-mario-run/
unsigned char sliding[2];				//SLIDING COUNTER

unsigned int  player_score[2];			//SCORE ARRAYS

unsigned int  player_next_extra[2];		//SCORE ARRAYS
unsigned int  player_joy[2];			//JOYSTICK ARRAYS

unsigned char	index_player;
unsigned char	sprite_other_player;
unsigned char	sprite;
unsigned char	tile_offset;
unsigned char	tile_offset_other_player;
unsigned char	hit_count;

unsigned char	s_tile0;
unsigned char	s_tile1;
unsigned char	s_lin0;
unsigned char	s_lin1;
unsigned char	s_col0;
unsigned char	s_col1;
unsigned char	sprite_lin_inc_mul;
unsigned int	loop_count;
unsigned int	index1;
unsigned char	tmp;
unsigned char	tmp0;
unsigned char	tmp1;
unsigned char	tmp_uc;
signed char		tmp_sc;
unsigned int	tmp_ui;
unsigned char	enemies;
unsigned char	zx_val_asm;
unsigned char	attrib[4];
unsigned char	attrib_hl[4];
unsigned char	s_state;

unsigned int	curr_time;
unsigned int	entry_time;
unsigned int	frame_time;
unsigned int	osd_update_time;
unsigned int	osd_show_time;
unsigned char	spr_count;
unsigned char game_ugly_back;
unsigned char game_ugly_fix_cnt;


//###############################################################################################
//#                                                                                             #
//# GAME VARIABLES                                                                              #
//#                                                                                             #
//###############################################################################################
unsigned char	game_pow;					//LEVEL OF POW ON THE SCREEN 0 = NONE
unsigned char game_brick_tile;
unsigned char game_two_player;
unsigned char game_start_phase;
unsigned char game_god_mode;
unsigned char game_inmune;
unsigned char	game_debug;

unsigned char	game_sound;
unsigned char	game_over;
unsigned char	game_lives[2];
unsigned int	game_time_flipped;
unsigned int	game_time_fireball_start;
unsigned char	spr_water_clear;
unsigned int	spr_water_time;
unsigned char	game_bonus;
unsigned char	game_osd;
unsigned char	game_type;
unsigned char	game_menu_sel;


unsigned int	game_score_top;
//###############################################################################################
//#                                                                                             #
//# PHASES VARIABLES                                                                            #
//#                                                                                             #
//###############################################################################################
unsigned char phases[] = {
	//SHELLCREPPER,SIDESTEPPER,FIGHTERFLY,BRICK TILE
	  3, 0, 0, TILE_BRICK1, 		//Phase  1: Turtle x3
	  5, 0, 0, TILE_BRICK1, 		//Phase  2: Turtle x5
	  0, 0, 0, TILE_BRICK2, 		//Phase  3: Bonus
	  0, 4, 0, TILE_BRICK3, 		//Phase  4: Crab x4
	  2, 4, 0, TILE_BRICK3, 		//Phase  5: Turtle x2,  Crab x4
	  0, 0, 4, TILE_BRICK4, 		//Phase  6: Fly x4
	  0, 2, 3, TILE_BRICK4, 		//Phase  7: Crab x2,  Fly x3
	  0, 0, 0, TILE_BRICK_FREEZE, 	//Phase  8: Bonus Freeze
	  4, 0, 1, TILE_BRICK2, 		//Phase  9: Turtle x4,  Fly x1,  Slipice
	  0, 4, 1, TILE_BRICK1, 		//Phase 10: Crab x4,  Fly x1,  Slipice
	  0, 4, 2, TILE_BRICK2, 		//Phase 11: Crab x4,  Fly x2,  Slipice
	  0, 2, 3, TILE_BRICK3,			//Phase 12+ random 8, 10, 11 <- NORMAL MODE!
	  0, 0, 0, TILE_BRICK1,			//Phase 13: Bonus
	  8, 0, 0, TILE_BRICK2,			//Phase 14: Fly x 8
	  0, 8, 0, TILE_BRICK3,			//Phase 15:
	  0, 0, 8, TILE_BRICK1,			//Phase 16
	  4, 4, 2, TILE_BRICK2,			//Phase 17
	  0, 0, 0, TILE_BRICK_FREEZE,	//Phase 18: Bonus
	  8, 2, 0, TILE_BRICK1,			//Phase 19
	  0, 6, 3, TILE_BRICK2,			//Phase 20
	  0, 2, 4, TILE_BRICK3,			//Phase 21
	  2, 2, 6, TILE_BRICK1,			//Phase 22
	  0, 0, 0, TILE_BRICK2,			//Phase 23: Bonus
	  6, 4, 0, TILE_BRICK3,			//Phase 24
	  6, 4, 2, TILE_BRICK_FREEZE,	//Phase 25
	  6, 4, 4, TILE_BRICK_FREEZE,	//Phase 26
	  6, 4, 6, TILE_BRICK_FREEZE,	//Phase 27
	  0, 0, 0, TILE_BRICK_FREEZE,	//Phase 28: Bonus
	  4, 4, 4, TILE_BRICK_FREEZE,	//Phase 29
	  8, 8, 8, TILE_BRICK_FREEZE,	//Phase 30
	 12,12,12, TILE_BRICK_FREEZE,	//Phase 31
	 16,16,16, TILE_BRICK_FREEZE	//Phase 32: HELL
};

//PHASE RELATED VARIABLES
unsigned char phase_quota[3];
unsigned char phase_left;
unsigned char phase_tot;
unsigned char phase_pop;
unsigned char phase_coins;
unsigned char phase_end;
unsigned char phase_curr;
unsigned char phase_angry;
unsigned char phase_bonus_total[2];
unsigned char screen_paper;
unsigned char screen_ink;
//SCORES OSD
unsigned char score_osd_lin[2];		//TO CLEAR POINTS ON SCREEN
unsigned char score_osd_col[2];		//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_update_time[2];	//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_tile[2];	//TO CLEAR POINTS ON SCREEN

//###############################################################################################
//#                                                                                             #
//# SCREEN GAME MAP                                                                             #
//#                                                                                             #
//###############################################################################################

unsigned char lvl_1[] ={
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,

  TILE_PIPE5  , TILE_PIPE5  , TILE_PIPE2  , TILE_PIPE2  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE3  , TILE_PIPE3  , TILE_PIPE7 , TILE_PIPE7 ,
	TILE_PIPE5  , TILE_PIPE5  , TILE_PIPE2  , TILE_PIPE2  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE3  , TILE_PIPE3  , TILE_PIPE7 , TILE_PIPE7 ,

  TILE_PIPE4  , TILE_PIPE4  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE6 , TILE_PIPE6 ,
	TILE_PIPE4  , TILE_PIPE4  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE6 , TILE_PIPE6 ,

  TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK , TILE_BRICK  ,
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,

  TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,

  TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,

  TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK , TILE_BRICK  ,
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,

  TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,
	TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY , TILE_EMPTY  ,

  TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_POW1   , TILE_POW1   , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK  , TILE_BRICK , TILE_BRICK  ,
	TILE_PIPE1A , TILE_PIPE1A , TILE_PIPE2A , TILE_PIPE2A , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_POW1   , TILE_POW1   , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE3A , TILE_PIPE3A , TILE_PIPE1A, TILE_PIPE1A ,

  TILE_PIPE1A , TILE_PIPE1A , TILE_PIPE2A , TILE_PIPE2A , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE3A , TILE_PIPE3A , TILE_PIPE1A, TILE_PIPE1A ,
	TILE_PIPE1B , TILE_PIPE1B , TILE_PIPE2B , TILE_PIPE2B , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE3B , TILE_PIPE3B , TILE_PIPE1B, TILE_PIPE1B ,

  TILE_PIPE1B , TILE_PIPE1B , TILE_PIPE2B , TILE_PIPE2B , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_EMPTY  , TILE_PIPE3B , TILE_PIPE3B , TILE_PIPE1B, TILE_PIPE1B

};



//###############################################################################################
//#                                                                                             #
//# ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ                                           #
//#                                                                                             #
//###############################################################################################
unsigned char sprite_speed[] = {
	 0, // 0 NOT USED
	 4, // 1 SHELLCREEPER_GREEN
	 3, // 2 SHELLCREEPER_RED
	 1, // 3 SHELLCREEPER_BLUE
	 4, //04 SIDESTEPPER_RED
	 3, //05 SIDESTEPPER_GREEN
	 1, //06 SIDESTEPPER_MAGENTA
	 4, //07 SLIPICE
	 3, //08 COIN_1
	 3, //09 FIGHTERFLY
	 2, //10 FIREBALL_RED
	 1, //11 FIREBALL_GREEN
	 3, //12 COIN_2
	 0, //13
	 0, //14
	 0, //15
	 PLAYER_SPEED, //16 PLAYER
};
//TEMPORARY SPEED FOR SPEED UP
unsigned char sprite_speed_alt[8];

/*HALL OF FAME*/
HOF_ENTRY hof[10] = {
	{"CRI", 10000},
	{"GON", 9000},
	{"ALV", 8000},
	{"ABR", 7000},
	{"EIN", 6000},
	{"SAU", 5000},
	{"FEL", 4000},
	{"GON", 3000},
	{"ALE", 2000},
	{"CAL", 1000}
};

const unsigned char hall_valids[42] = "\x01" "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.~ {";
unsigned char initials[8] = "AAA\0AAA";

const unsigned int fix_1[10] = {
                         //64,66,92,94,
                         //128,158,
                         204,210,
                         362,372,
                         384,386,
                         412,414,
                         522,532,
                         //544,546,572,574,
                         //608,610,636,638
                       };
