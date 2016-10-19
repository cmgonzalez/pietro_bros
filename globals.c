#include <input.h>
#include "pietro.h"
#include "globals.h"

unsigned char spec128;

//###############################################################################################
//#                                                                                             #
//# CONTROL VARIABLES                                                                           #
//#                                                                                             #
//###############################################################################################
char *joynames[] = { "Keyboard QAOPM", "Kempston", "Sinclair 1", "Sinclair 2" };

#ifdef __SDCC
uint16_t (*joyfunc1)(udk_t *);			// pointer to joystick function Player 1 
uint16_t (*joyfunc2)(udk_t *);			// pointer to joystick function Player 1
#endif


#ifdef __SCCZ80
void *joyfunc1;							// pointer to joystick function Player 1
void *joyfunc2;							// pointer to joystick function Player 1
#endif
udk_t k1;
udk_t k2;
unsigned char dirs;

unsigned char tbuffer[7];			// temporary buffer

//SPRITES GAME ARRAYS
unsigned char class[8];					//CLASS OF SPRITE
unsigned char state[8];					//SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8];				//SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char tile[8];					//TILE
unsigned char lin[8];					//LINE
unsigned char col[8];					//COLUMN
unsigned char colint[8];				//INTERNAL COLUMN/TILE INCREMENT
unsigned int  spr_timer[8];				//SPRITE GENERAL TIMER MILISECONDS
unsigned int  spr_timer_c[8];			//SPRITE COLITIONS CHECK TIMER MILISECONDS
unsigned int  last_time[8];				//LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8];				//START JUMP LINE

// PLAYER ONLY 
unsigned char hit_lin[2];				//HIT BRICK LINE
unsigned char hit_col[2];				//HIT BRICK COL
unsigned char sliding[2];				//SLIDING COUNTER
unsigned int  player_score[2];			//SCORE ARRAYS
unsigned int  player_next_extra[2];		//SCORE ARRAYS

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
unsigned char	s_dir0;

unsigned int	loop_count;
unsigned int	index1;
unsigned int	index2;
unsigned int	index3;
unsigned int	index4;
unsigned int	index_d;
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
unsigned int	score_top;
unsigned int	curr_time;
unsigned int	entry_time;
unsigned int	frame_time;
unsigned int	col_time;
unsigned int	frame_loop_count;
unsigned char	spr_count;
//###############################################################################################
//#                                                                                             #
//# AY SOUND VARIABLES                                                                          #
//#                                                                                             #
//###############################################################################################
unsigned char	ay_background_sound;		//PROGRAM LOGIC DETERMINES BACKGROUND EFFECT
unsigned char	ay_background_sound_old;
unsigned char	ay_playing_background;		//FLAG INDICATES IF CURRENT SOUND IS A BACKGROUND
unsigned char	*ay_background_effect;		//CURRENT BACKGROUND EFFECT
//###############################################################################################
//#                                                                                             #
//# GAME VARIABLES                                                                              #
//#                                                                                             #
//###############################################################################################
unsigned char	game_pow;					//LEVEL OF POW ON THE SCREEN 0 = NONE
unsigned char 	game_brick_tile;
unsigned char 	game_two_player;
unsigned char	game_sound;
unsigned char	game_over;
unsigned char	game_show_fps;
unsigned char	game_lives[2];
unsigned char	game_lives_update;
unsigned int	game_time_flipped;
unsigned int	game_time_fireball_start;
unsigned char	game_water_clear;
unsigned int	game_water_time;
unsigned char	game_bonus;
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
	  3,0,0,TILE_BRICK1,		//Phase  1: Turtle x3
	  5,0,0,TILE_BRICK1,		//Phase  2: Turtle x5
	  0,0,0,TILE_BRICK2,		//Phase  3: Bonus
	  0,4,0,TILE_BRICK3,		//Phase  4: Crab x4
	  2,4,0,TILE_BRICK3,		//Phase  5: Turtle x2, Crab x4
	  0,0,4,TILE_BRICK4,		//Phase  6: Fly x4
	  0,2,3,TILE_BRICK4,		//Phase  7: Crab x2, Fly x3
	  0,0,0,TILE_BRICK_FREEZE,	//Phase  8: Bonus Freeze
	  4,0,1,TILE_BRICK2,		//Phase  9: Turtle x4, Fly x1, Slipice
	  0,4,1,TILE_BRICK2,		//Phase 10: Crab x4, Fly x1, Slipice
	  0,4,2,TILE_BRICK2,		//Phase 11: Crab x4, Fly x2, Slipice
	  0,2,3,TILE_BRICK2			//Phase 12+ random 8,10,11
};

//PHASE RELATED VARIABLES
unsigned char phase_quota[3];
unsigned char phase_left;
unsigned char phase_curr;
unsigned char phase_angry;
unsigned char phase_bonus_total[2];
unsigned char screen_paper;
unsigned char screen_ink;

//SCORES OSD
unsigned char score_osd_lin[2];		//TO CLEAR POINTS ON SCREEN 
unsigned char score_osd_col[2];		//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_time[2];	//TO CLEAR POINTS ON SCREEN
unsigned int  score_osd_tile[2];	//TO CLEAR POINTS ON SCREEN

//###############################################################################################
//#                                                                                             #
//# SCREEN GAME MAP                                                                             #
//#                                                                                             #
//###############################################################################################
unsigned char lvl_1[] ={ 
//	 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 5, 5, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 7, 7,
	 5, 5, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 7, 7,
	 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
	 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6,
	18,18,18,18,18,18,18,18,18,18,18,18,18,18, 0, 0, 0, 0,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,18,18,18,18,18,18,18,18,18,18,18,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	18,18,18,18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,18,18,18,18,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17,17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	18,18,18,18,18,18,18,18,18,18,18,18, 0, 0, 0,17,17, 0, 0, 0,18,18,18,18,18,18,18,18,18,18,18,18,
	 8, 8, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10, 8, 8,
	 8, 8, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,10, 8, 8,
	11,11,12,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,13,13,11,11,
	11,11,12,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,13,13,11,11
};

//###############################################################################################
//#                                                                                             #
//# SCREEN TILE FROM MAP INDEX ARRAY                                                            #
//#                                                                                             #
//###############################################################################################
unsigned int spr_idx[] = {
	TILE_EMPTY, 		//0
	TILE_PIPE1, 		//1
	TILE_PIPE2, 		//2
	TILE_PIPE3, 		//3
	TILE_PIPE4, 		//4
	TILE_PIPE5, 		//5
	TILE_PIPE6, 		//6
	TILE_PIPE7, 		//7
	TILE_PIPE1A, 		//8
	TILE_PIPE2A, 		//9
	TILE_PIPE3A, 		//10
	TILE_PIPE1B, 		//11
	TILE_PIPE2B, 		//12
	TILE_PIPE3B, 		//13
	0, 					//14
	0, 					//15
	0 , 				//16 COLITION
	TILE_POW1, 			//17 COLITION 
	TILE_BRICK1, 		//18 COLITION
	0, 		 			//19 COLITION
	TILE_BRICK_FREEZE	//20 COLITION
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
	 2, // 3 SHELLCREEPER_BLUE 
	 4, //04 SIDESTEPPER_RED 
	 3, //05 SIDESTEPPER_GREEN 
	 2, //06 SIDESTEPPER_MAGENTA 
	 4, //07 SLIPICE 
	 3, //08 COIN_1 
	 4, //09 FIGHTERFLY 
	 4, //10 FIREBALL_RED 
	 3, //11 FIREBALL_GREEN 
	 3, //12  COIN_2 
	 0, //13  
	 0, //14  
	 0, //15  
	 2, //16 PLAYER 
};
//TEMPORARY SPEED FOR SPEED UP
unsigned char sprite_speed2[8];

/*HALL OF FAME*/
unsigned int hall_scores[] = {
	10000,
	 9000,
	 8000,
	 7000,
	 6000,
	 5000,
	 4000,
	 3000,
	 2000,
	 1000,
};

unsigned char * hall_names[] = {
	"CRI",
	"GON",
	"ALV",
	"ABR",
	"EIN",
	"SAU",
	"FEL",
	"GON",
	"ALE",
	"CAL",
	"LOV",
};

unsigned char *initals = "A\0A\0A\0A\0A\0A\0";
unsigned char *hall_valids = "ABCEDFGHIJKLMNOPQRSTUVWXYZ1234567890.~ {";
unsigned char hall_flip;
