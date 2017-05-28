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
#ifndef PIETRO_SPRITE_H
#define PIETRO_SPRITE_H
extern unsigned char    spr_chktime(unsigned char *sprite) __z88dk_fastcall;
extern unsigned char    spr_move_up(void);
extern unsigned char    spr_move_down(void);
extern void             spr_move_horizontal(void);
extern unsigned char    spr_move_right(void);
extern unsigned char    spr_move_left(void);
extern unsigned char    spr_redraw(void);
extern unsigned char    spr_killed( unsigned char f_sprite) __z88dk_fastcall;
extern void             spr_anim_fall( unsigned char f_sprite) __z88dk_fastcall;
extern void             spr_anim_kill( unsigned char f_sprite, unsigned int f_tile);
extern int              spr_tile_dir(unsigned int f_tile, unsigned char f_sprite, unsigned char f_inc);
extern unsigned char    spr_check_over(unsigned char);
extern void             spr_destroy(unsigned char f_sprite) __z88dk_fastcall;
extern void             spr_set_fall(void);
extern int              spr_tile(unsigned char f_sprite) __z88dk_fastcall;
extern void             spr_draw_pow(void);
extern void             spr_back_clr(void);
extern void             spr_back_paint(unsigned int f_inc);
extern void             spr_back_fix(unsigned char f_inc) __z88dk_fastcall;
extern void             spr_brick_anim(unsigned char f_hit) __z88dk_fastcall;
extern void             spr_cortina_brick(void);
extern void             spr_draw_background(void);
extern void             spr_water_splash_clear();
extern void             spr_water_splash_draw( unsigned char f_col) __z88dk_fastcall;
extern void             spr_draw_clear(void);
extern void             spr_draw_index(unsigned int f_index);
extern void             spr_draw_row(unsigned char f_row);
extern unsigned char    spr_calc_hor(unsigned char f_sprite);
extern unsigned char    spr_collision_check(unsigned char f_sprite1, unsigned char f_sprite2, unsigned char f_vert_diff);
extern void			        spr_kill_all(void);
extern void             spr_mapfix(void);
#endif
