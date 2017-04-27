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
#ifndef PIETRO_GAME_H
#define PIETRO_GAME_H

#include "globals.h"

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
/* Sprites Speeds */

extern void          game_fill_row(unsigned char f_row, unsigned char f_asc) __z88dk_callee;
extern unsigned int	game_calc_index(unsigned char l, unsigned char c);
extern void          game_print_footer(void);
extern void			 game_print_lives(void);
extern void			 game_tick(void);
extern void			 game_start_timer(void);
extern unsigned char game_phase_calc(void);
extern void			 game_phase_init(void);
extern void			 game_phase_print(unsigned char f_row) __z88dk_fastcall;
extern void			 player_set1(void);
extern void			 player_set2(void);
extern void			 game_loop(void);
extern void			 game_kill_all_sprites(void);
extern unsigned char game_check_maze(int f_index) __z88dk_fastcall;
extern unsigned char game_enemy_add(void);
extern unsigned char game_enemy_rnd(void);
extern unsigned char game_enemy_quota(void);
extern unsigned char game_enemy_add1(unsigned char f_class) __z88dk_fastcall;
extern unsigned char game_enemy_add_get_index(unsigned char f_search) __z88dk_fastcall;
extern void			 game_freeze(unsigned char f_lin, unsigned char f_col);
extern void			 game_freeze_all(void);
extern void			 game_unfreeze_all(void);
extern void			 game_print_score(void);
extern void			 game_menu_back(unsigned char f_start) __z88dk_fastcall;
extern void			 game_menu_e(unsigned char f_col, unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign);
extern unsigned char game_menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout);
extern void          game_menu_top_paint(void);
extern void 		 game_menu_paint(void);
extern void			 game_paint_attrib(unsigned char e_r1) __z88dk_fastcall;
extern void			 game_paint_attrib_lin(unsigned char f_start,unsigned char f_end,unsigned char f_lin);
extern void			 game_paint_attrib_lin_h(unsigned char f_start,unsigned char f_end,unsigned char f_lin);
extern void			 game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_miliseconds	);
extern void			 game_rotate_attrib(void);
extern void			 game_menu(void);
extern void			 game_menu_config(void);
extern unsigned char game_check_time(unsigned int start, unsigned int lapse);
extern void			 game_bonus_summary(void);
extern void			 game_bonus_summary_player(unsigned char f_index) __z88dk_fastcall;
extern void			 game_bonus_clock(void);
extern void			 game_phase_print_score_back(void);
extern void			 game_score_osd(void);
extern void			 game_hall_of_fame(void);
extern void			 game_hall_enter(void);
extern void			 game_end(void);
extern void			 game_print_header(void);
extern void			 game_hall_enter_phs(unsigned char p) __z88dk_fastcall;
extern HOF_ENTRY  *game_hall_check(unsigned char p) __z88dk_fastcall;
extern unsigned char game_hall_edit_p(unsigned char index) __z88dk_fastcall;
extern void			 game_hall_print_p(unsigned char index, unsigned char frame);

extern void      game_joystick_change(unsigned char f_player_index) __z88dk_fastcall;
extern void      game_joystick_set(void);
extern void      game_joystick_set_menu(void);

#endif
