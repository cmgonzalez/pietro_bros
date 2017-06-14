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

extern void			     menu_back(unsigned char f_start) __z88dk_fastcall;
extern void			     menu_e(unsigned char f_col, unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign);
extern unsigned char menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout);
extern void          menu_top_paint(void);
extern void 		     menu_paint(void);
extern void			     menu(void);
extern void			     menu_config(void);
extern void          hall_of_fame(void);
extern void          hall_enter(void);
extern void          hall_enter_phs(unsigned char p) __z88dk_fastcall;
extern void          hall_enter_prt(unsigned char col) __z88dk_fastcall;
extern HOF_ENTRY    *hall_check(unsigned char p) __z88dk_fastcall;
extern unsigned char hall_edit_p(unsigned char index) __z88dk_fastcall;
extern void          hall_print_p(unsigned char index, unsigned char frame);
extern void          hall_print_p2(unsigned char index, char col, unsigned char frame);
extern void          game_joystick_change(unsigned char f_player_index) __z88dk_fastcall;
extern void          game_joystick_set(void);
extern void          game_joystick_set_menu(void);



#endif
