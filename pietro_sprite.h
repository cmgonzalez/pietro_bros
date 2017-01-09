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

extern unsigned char spr_chktime(unsigned char *sprite);
extern unsigned char spr_move_up(void);
extern unsigned char spr_move_down(void);
extern void          spr_move_horizontal(void);
extern unsigned char spr_move_right(void);
extern unsigned char spr_move_left(void);
extern void          spr_redraw(void);
extern unsigned char spr_killed( unsigned char f_sprite);
extern void          spr_anim_fall( unsigned char f_sprite);
extern void          spr_anim_kill( unsigned char f_sprite, unsigned int f_tile);
extern unsigned char spr_collition_check(unsigned char f_dir);
extern int           spr_tile_dir(unsigned int f_tile, unsigned char f_sprite, unsigned char f_inc);
extern unsigned char spr_check_over(void);
extern void          spr_destroy(unsigned char f_sprite);
extern void          spr_set_fall(void);
extern int           spr_tile(unsigned char f_sprite);

extern void			 spr_draw_pow(void);
extern void			 spr_back_fix1(void);
extern void			 spr_back_fix2(void);
extern void			 spr_back_fix3(void);
extern void			 spr_back_fix4(void);
extern void			 spr_brick_anim(unsigned char f_hit);
extern void			 spr_cortina_brick(void);
extern void			 spr_cortina_pipes(void);
extern void			 spr_draw_back(void);
extern void			 spr_water_splash_clear();
extern void			 spr_water_splash_draw( unsigned char f_col);
extern void			 game_draw_clear(void);


#endif
