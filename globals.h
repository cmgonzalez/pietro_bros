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
#ifndef GLOBALS_H
#define GLOBALS_H
#include <input.h>


/* Enemies indexes */
#define SHELLCREEPER_GREEN            1
#define SHELLCREEPER_RED              2
#define SHELLCREEPER_BLUE             3
#define SIDESTEPPER_RED               4
#define SIDESTEPPER_GREEN             5
#define SIDESTEPPER_MAGENTA           6
#define SLIPICE                       7
#define COIN_1                        8
#define FIGHTERFLY                    9
#define FIREBALL_RED                  10
#define FIREBALL_GREEN                11
#define COIN_2                        12
#define PLAYER                        16
/* Player 1 tiles */
#define TILE_P1_STANR                 3
#define TILE_P1_RIGHT                 0
#define TILE_P1_SLIDR                 4
#define TILE_P1_JUMPR                 7
#define TILE_P1_KILL                  10
#define TILE_P1_HITR                  11
/* Enemies tiles */
#define TILE_SHELLCREEPER_GREEN       48
#define TILE_SHELLCREEPER_RED         60
#define TILE_SHELLCREEPER_BLUE        72
#define TILE_SIDESTEPPER_RED          84
#define TILE_SIDESTEPPER_GREEN        96
#define TILE_SIDESTEPPER_MAGENTA      108
#define TILE_SLIPICE                  123
#define TILE_FIGHTERFLY               120
#define TILE_FIREBALL_RED             144
#define TILE_FIREBALL_GREEN           150
#define TILE_COIN1                    180
#define TILE_COIN2                    183
/* Screen tiles */
#define TILE_BRICK0                   132
#define TILE_BRICK1                   135
#define TILE_BRICK2                   136
#define TILE_BRICK3                   137
#define TILE_BRICK4                   138
#define TILE_BRICK_FREEZE             139
#define TILE_BRICK_RESTART            132
#define TILE_BRICK_FILL               135
#define TILE_PIPE1                    101
#define TILE_PIPE2                    179
#define TILE_PIPE3                    167
#define TILE_PIPE4                    177
#define TILE_PIPE5                    165
#define TILE_PIPE6                    178
#define TILE_PIPE7                    166
#define TILE_PIPE1A                   163
#define TILE_PIPE2A                   164
#define TILE_PIPE3A                   162
#define TILE_PIPE1B                   175
#define TILE_PIPE2B                   176
#define TILE_PIPE3B                   174
/* Misc tiles */
#define TILE_800_COIN                 186
#define TILE_DOLLAR                   187
#define TILE_800                      188
#define TILE_NICE                     189
#define TILE_EXTRA                    190
#define TILE_PRINCESS                 202
#define TILE_CASTLE                   214
#define TILE_GRASS                    140
#define TILE_POW1                     191
#define TILE_EMPTY                    169
/* Player indexes */
#define SPR_P1                        7
#define SPR_P2                        6
/* Screen variables */
#define SPR_COLINT                    3   /* Internal colum increment */
#define SCR_COLI                      1   /* Start column */
#define SCR_COLS                      32  /* Columns max */
#define SCR_COLS_M                    30  /* Columns max */
#define SCR_ROWI                      0   /* Start row */
#define SCR_ROWS                      23  /* Rows max */
#define SCR_LINS                      184 /* Lins max */
#define PAPER                         000 /* Screen paper */
#define GAME_LIN_FLOOR                152
#define GAME_LIN_TOP_PLATFORM         48
#define GAME_ENEMY_COL_CHECK_TIME     16

#define ST_STAND_R                    0
#define ST_STAND_L                    1
#define ST_TURN_R                     10
#define ST_TURN_L                     11
#define VAL_COL                       16
#define IDX_POW                       17
#define POW_INDEX                     527
/* Starting positions*/
#define DIR_LEFT                      255
#define DIR_RIGHT                     1
/*Stats (bit position number)*/
#define STAT_JUMP                     0
#define STAT_FALL                     1
#define STAT_DIRR                     2
#define STAT_DIRL                     3
#define STAT_HIT                      4
#define STAT_KILL                     5
#define STAT_ANGRY                    6
#define STAT_UPGR                     7
/*Stats alt (bit position number)*/
#define STAT_LOCK                     0
#define STAT_TURN                     1
#define STAT_PUSH                     2
#define STAT_LDIRL                    3
#define STAT_LDIRR                    4
#define STAT_INERT                    5
#define STAT_RESTART                  6
#define STAT_HITBRICK                 7
/* General */
#define SPRITE_FALL_SPEED             3  /* Speed for falling from the screen after a kill */
#define SPRITE_RESTART_SPEED          8
#define SPRITE_LIN_INC                2
#define SPRITE_HEIGHT                 16
/* Player */
#define PLAYER_SLIDE_POW              3
#define PLAYER_SLIDE_NORMAL           6
#define PLAYER_SLIDE_ICE              12
#define PLAYER_MAX_JUMP               10  /* Max Jump Time 10 frames */
#define PLAYER_HIT_BRICK_TIME         4
#define PLAYER_VCOL_MARGIN            14 //VERTICAL COLLISION MARGIN
#define PLAYER_HCOL_MARGIN            2  //HORIZONTAL COLLISION MARGIN
#define PLAYER_HCOL_MARGIN_INT        4  //HORIZONTAL COLLISION MARGIN INTERNAL (1 COL = 3 INT)
#define PLAYER_SPEED                  2
#define PLAYER_JUMP_SPEED             2
#define PLAYER_FALL_SPEED             2
#define PLAYER_INERT_TIME             8  /* Time for Player Inertia */
#define PLAYER_ANIM_HIT_TIME          40 /* Time for Player Hit Sprite display after kill */
#define PLAYER_TOP_SUSPEND            6


/* Enemies */
#define ENEMIES_MAX                   6   //MAX QUANTITY OF ENEMIES ON SCREEN (0->6)
#define ENEMIES_MAXJUMP               12 //MAX JUMP WHEN HIT
#define ENEMY_JUMP_SPEED              1
#define ENEMY_FALL_SPEED              1
#define ENEMY_KILLED_SPEED            8
#define ENEMY_FIREBALL_START_COL_L    3
#define ENEMY_FIREBALL_START_COL_R    28
#define ENEMY_FIREBALL_START_TOP      16
#define ENEMY_FIREBALL_START_MID      56
#define ENEMY_FIREBALL_START_BOT      132
#define ENEMY_SCOL_R                  26
#define ENEMY_SLIN_R                  16
#define ENEMY_SCOL_L                  4
#define ENEMY_SLIN_L                  16
/* Game times */
#define TIME_WATER_SPLASH             15
#define TIME_EVENT                    100
#define TIME_BONUS                    1600   //15 SECONDS IN HUNDREDTHS (ORIG IS 30, TO EASY 4 PIETRO)
#define TIME_FIREBALL_RED             1500 //30 SECONDS (50HZ)
#define TIME_FIREBALL_A               2000 //40 - ORIG 80 SECONDS (50HZ)
#define TIME_FIREBALL_B               500  //15 - ORIG 60 SECONDS (50HZ)
#define TIME_FLIPPED_A                500  //15 - ORIG 20 SECONDS (50HZ)
#define TIME_FLIPPED_B                250  //7  - ORIG 15 SECONDS (50HZ)
#define GAME_EXTRA_LIFE               2000
#define GAME_RANDOM_TYPE              2
/* Map tiles */
#define GAME_MAP_PLATFORM             18
#define GAME_MAP_PLATFORM_FREEZE      20
#define GAME_MAP_TOTAL_POS            672

/* Sound Mode Variables */
#define GAME_SOUND_48_FX_ON          0x01
#define GAME_SOUND_48_FX_OFF         0xfe
#define GAME_SOUND_48_MUS_ON         0x02
#define GAME_SOUND_48_MUS_OFF        0xfd
#define GAME_SOUND_AY_FX_ON          0x04
#define GAME_SOUND_AY_FX_OFF         0xfb
#define GAME_SOUND_AY_MUS_ON         0x08
#define GAME_SOUND_AY_MUS_OFF        0xf7




typedef struct {
	unsigned char name[4];
	unsigned int  score;
} HOF_ENTRY;

typedef uint16_t (*JOYFUNC)(udk_t *);
#endif


extern unsigned char spec128;
extern unsigned char btiles[];
extern uint16_t (*joyfunc1)(udk_t *);
extern uint16_t (*joyfunc2)(udk_t *);
extern udk_t k1;
extern udk_t k2;
extern unsigned char dirs;
extern unsigned char tbuffer[7];
extern unsigned char class[8];
extern unsigned char state[8];
extern unsigned char state_a[8];
extern unsigned char tile[8];
extern unsigned char lin[8];
extern unsigned char col[8];
extern unsigned char colint[8];
extern unsigned char hit_lin[2];
extern unsigned char hit_col[2];
extern unsigned char player_jump_c[2];
extern unsigned char jump_lin[8];
extern unsigned int  spr_timer[8];
extern unsigned int  spr_timer_c[8];
extern unsigned char sliding[2];
extern unsigned int  last_time[8];
extern unsigned int  player_score[2];
extern unsigned int  player_next_extra[2];
extern unsigned int  player_joy[2];

extern unsigned char	sprite;
extern unsigned char	sprite_other_player;
extern unsigned char	index_player;
extern unsigned char	tile_offset;
extern unsigned char	tile_offset_other_player;
extern unsigned char	hit_count;
extern unsigned char	s_tile0;
extern unsigned char	s_tile1;
extern unsigned char	s_lin0;
extern unsigned char	s_lin1;
extern unsigned char	s_col0;
extern unsigned char	s_col1;
extern unsigned char	sprite_lin_inc_mul;
extern unsigned int		loop_count;
extern unsigned int		index1;
extern unsigned int		index2;
extern unsigned int		index3;
//extern unsigned int		index_d;
extern unsigned char	tmp;
extern unsigned char	tmp0;
extern unsigned char	tmp1;
extern unsigned char	tmp_uc;
extern signed char		tmp_sc;
extern unsigned int		tmp_ui;
extern unsigned char	enemies;
extern unsigned char	zx_val_asm;
extern unsigned char	attrib[4];
extern unsigned char	attrib_hl[4];
extern unsigned char	s_state;
extern unsigned int		curr_time;
extern unsigned int		entry_time;
extern unsigned int		frame_time;
extern unsigned int  	osd_time;
extern unsigned char	spr_count;
extern unsigned char	  game_ugly_fix_cnt;

extern unsigned char	game_pow;
extern unsigned char 	game_brick_tile;
extern unsigned char 	game_two_player;
extern unsigned char  game_start_phase;
extern unsigned char  game_god_mode;
extern unsigned char  game_inmune;
extern unsigned char	game_sound;

extern unsigned char	game_over;
extern unsigned char	game_lives[2];
extern unsigned int		game_score_top;
extern unsigned int		game_time_flipped;
extern unsigned int		game_time_fireball_start;
extern unsigned char	spr_water_clear;
extern unsigned int		spr_water_time;
extern unsigned char	game_bonus;
extern unsigned char	game_osd;
extern unsigned char	game_type;
extern unsigned char	game_menu_sel;


extern unsigned char screen_paper, screen_ink;
extern unsigned char sprite_speed[];
extern unsigned char sprite_speed_alt[8];
extern unsigned char lvl_1[];
extern unsigned int  spr_idx[];
extern unsigned char phases[];
extern unsigned char phase_quota[3];
extern unsigned char phase_left;
extern unsigned char phase_tot;
extern unsigned char phase_pop;
extern unsigned char phase_coins;
extern unsigned char phase_end;
extern unsigned char phase_curr;
extern unsigned char phase_angry;
extern unsigned char phase_bonus_total[2];
extern unsigned char score_osd_lin[2];
extern unsigned char score_osd_col[2];
extern unsigned int  score_osd_time[2];
extern unsigned int  score_osd_tile[2];

extern HOF_ENTRY hof[10];
extern const unsigned char hall_valids[42];

extern const JOYFUNC control_method[7];
extern unsigned char initials[8];
extern const char *joynames[];
