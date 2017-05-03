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
#ifndef PIETRO_ENEMIES_H
#define PIETRO_ENEMIES_H

extern void          enemy_coin1(void);
extern unsigned char enemy_collision(void);
extern unsigned char enemy_collision_check(void);
extern void          enemy_collision_turn_dir( unsigned char f_sprite) __z88dk_fastcall;
extern void          enemy_hit(void);
extern void          enemy_flip_change_dir(unsigned char f_keep) __z88dk_fastcall;
extern void          enemy_flip(unsigned int f_tile) __z88dk_fastcall;
extern void          enemy_flip_sidestepper(unsigned int f_tile) __z88dk_fastcall;
extern void          enemy_turn(void);
extern void          enemy_standard(void);
extern void          enemy_slipice(void);
extern void          enemy_fireball_red(void);
extern void          enemy_fireball_green(void);
extern void          enemy_fighterfly(void);
extern void          enemy_trip(void);
extern void          enemy_trip_move(unsigned char f_slin, unsigned char f_scol, unsigned char f_dir, unsigned char f_dir_alt);
extern void          enemy_standard_hit(void);
extern void          enemy_evolve(unsigned char f_enemy) __z88dk_fastcall;
extern void          enemy_upgrade(unsigned char f_enemy, unsigned char f_class, unsigned int f_tile);
extern void          enemy_walk(void);
extern void          enemy_init(unsigned char f_sprite,unsigned char f_lin,unsigned  char f_col,unsigned  char f_class,unsigned  char f_dir);
extern void          enemy_kill(unsigned char f_sprite) __z88dk_fastcall;
extern void          enemy_ugly_fix(void);
#endif
