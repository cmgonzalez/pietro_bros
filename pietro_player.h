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
#ifndef PIETRO_PLAYER_H
#define PIETRO_PLAYER_H

extern void          player_init(unsigned char f_sprite, unsigned char f_lin, unsigned char f_col, unsigned char f_tile);
extern void 		     player_calc_slide();
extern unsigned char player_collision(void);
extern unsigned char player_collision_check(void);
extern unsigned char player_check_input(void);
extern void          player_kill(void);
extern void          player_restart(unsigned char f_sprite) __z88dk_fastcall;
extern unsigned char player_move(void);
extern void          player_turn(void);
extern unsigned char player_move_input(void);
extern unsigned char player_hit_brick(void);
extern unsigned char player_hit_pow(void);
extern void          player_coin(unsigned char f_enemies,unsigned char f_score);
extern void          player_score_add(unsigned int f_score) __z88dk_fastcall;
extern unsigned char player_lost_life(void);
extern void          player_hit_brick_clear(void);
extern void          player_push(void);
unsigned char        player_push_check(void);
extern void          player_move_horizontal(void);

#endif
