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

extern unsigned char spec128;

extern unsigned char btiles[];
extern char *joynames[];
#ifdef __SDCC
extern uint16_t (*joyfunc1)(udk_t *);
extern uint16_t (*joyfunc2)(udk_t *);
#endif
#ifdef __SCCZ80
extern void *joyfunc1;
extern void *joyfunc2;
#endif
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
extern unsigned char jump_lin[8];
extern unsigned int  spr_timer[8];
extern unsigned int  spr_timer_c[8];
extern unsigned char sliding[2];
extern unsigned int  last_time[8];
extern unsigned int  player_score[2];
extern unsigned int  player_next_extra[2];
extern unsigned char player_lock[2];

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
extern unsigned int		loop_count;
extern unsigned int		index1;
extern unsigned int		index2;
extern unsigned int		index3;
extern unsigned int		index_d;
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
extern unsigned int		col_time;
extern unsigned int		frame_loop_count;
extern unsigned char	spr_count;
extern unsigned char	game_pow;
extern unsigned char 	game_brick_tile;
extern unsigned char 	game_two_player;

#define GAME_SOUND_48_FX_ON       0x01
#define GAME_SOUND_48_FX_OFF      0xfe
#define GAME_SOUND_48_MUS_ON      0x02
#define GAME_SOUND_48_MUS_OFF     0xfd
#define GAME_SOUND_AY_FX_ON       0x04
#define GAME_SOUND_AY_FX_OFF      0xfb
#define GAME_SOUND_AY_MUS_ON      0x08
#define GAME_SOUND_AY_MUS_OFF     0xf7
extern unsigned char	game_sound;

extern unsigned char	game_over;
extern unsigned char	game_lives[2];
extern unsigned char	game_lives_update;
extern unsigned int		game_score_top;
extern unsigned int		game_time_flipped;
extern unsigned int		game_time_fireball_start;
extern unsigned char	spr_water_clear;
extern unsigned int		spr_water_time;
extern unsigned char	game_bonus;
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

typedef struct {
	   unsigned char name[4];
		unsigned int  score;
} HOF_ENTRY;

extern HOF_ENTRY hof[10];
extern const unsigned char hall_valids[42];
extern unsigned char initials[8];


#endif
