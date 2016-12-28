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
#include "nirvana+.h"
#include "pietro.h"
#include "pietro_game.h"
#include "pietro_player.h"
#include "pietro_sprite.h"
#include "pietro_zx.h"
#include "macros.h"

unsigned char spr_chktime( unsigned char *sprite ) {
	//if (FULL_SPEED) return 1;
	if (sprite_speed2[*sprite] == 0) {
		tmp = sprite_speed[class[*sprite]];
	} else {
		
		if ( !BIT_CHK(state[*sprite], STAT_ANGRY) ) {
			tmp = sprite_speed2[*sprite];
		} else {
			if (tmp > 1) {
				tmp = sprite_speed2[*sprite]-1;
			} else {
				tmp = 0;
			}
		}
		
	}
	if ( game_check_time(last_time[*sprite],tmp)) {
		last_time[*sprite] = zx_clock();
		return 1;
	}
	return 0;
}

unsigned char spr_move_up( void ){
	//12
	//43
	if ((lin[sprite] & 7) == 0) {
		index1 = game_calc_index( lin[sprite] - LIN_INC , col[sprite]);
		if ( !game_check_maze( index1 ) ) {
			if (sprite >= SPR_P2 ) {
				index2 = index1 + 1;
				if(!player_hit_brick()) {
					player_hit_pow();
				};
				return 1;
			}
			spr_set_fall();
			return 0;
		}
	}
	
	
	lin[sprite] = lin[sprite] - LIN_INC; 
	if (lin[sprite] == 0) { 
		spr_set_fall();
		NIRVANAP_fillT(18, s_lin0,s_col0);
	}
	return 0;
}

unsigned char spr_move_down( void ){
	//12
	//43
	if ((lin[sprite] & 7) == 0) {
		if ( !game_check_maze( game_calc_index( lin[sprite] + 16 , col[sprite] ) ) ) {
			BIT_CLR(s_state, STAT_FALL);
			if (sprite >= SPR_P2 ) {
				tile[sprite] = spr_tile_dir(TILE_P1_STANR + tile_offset,sprite,12);
			}
			return 0;
		}
	}
	lin[sprite] = lin[sprite] + LIN_INC;
	if (lin[sprite] > GAME_LIN_FLOOR) {
		BIT_CLR(s_state, STAT_FALL);
		lin[sprite] = GAME_LIN_FLOOR;
	}
	return 0;
}

void spr_move_horizontal(void ){
	if (BIT_CHK(state[sprite], STAT_DIRR)) {
		spr_move_right();
	} else {
		if (BIT_CHK(state[sprite], STAT_DIRL)) {
			spr_move_left();
		}
	}
}

unsigned char spr_move_right( void ){
	++colint[sprite];
	if (colint[sprite] == SPR_COLINT) {
		if ( BIT_CHK(state[sprite], STAT_JUMP) || BIT_CHK(state[sprite], STAT_FALL)) {
			if ( spr_collition_check(DIR_RIGHT) ) {
				colint[sprite] = SPR_COLINT -1 ;
				return 0;
			}
		}
		colint[sprite] = 0;
		++col[sprite];
		if (col[sprite] > SCR_COLS_M) {
			col[sprite]= 0;
		}
	}
	return 0;
}

unsigned char spr_move_left( void ){
	--colint[sprite];
	if (colint[sprite] == 255) {
		if ( BIT_CHK(state[sprite], STAT_JUMP) || BIT_CHK(state[sprite], STAT_FALL)) {
			if ( spr_collition_check(DIR_LEFT) ) {
				colint[sprite] = 0;
				return 0;
			}
		}
		colint[sprite] = SPR_COLINT - 1;
		--col[sprite];
		if (col[sprite] == 255) {
			col[sprite] = SCR_COLS_M;
		}
	}
	return 0;
}

void spr_redraw( void ){
	if ( (lin[sprite] !=  s_lin0) || (col[sprite] != s_col0) ) { //MOVIMIENTO DE CARACTER
		s_tile1 = tile[sprite] + colint[sprite];
		NIRVANAP_spriteT(sprite, s_tile1, lin[sprite], col[sprite]);
		if ( !spr_check_over() ) {
			NIRVANAP_fillT(PAPER, s_lin0, s_col0);
		}
	} else {
		s_tile1 = tile[sprite] + colint[sprite];
		if (  s_tile1 != s_tile0 ) { //MOVIMIENTO INTERNO
			NIRVANAP_spriteT(sprite, s_tile1, lin[sprite], col[sprite]);
		}
	}
	
}

unsigned char spr_killed( unsigned char f_sprite) {
		switch(class[f_sprite]) {
		case SLIPICE :
			spr_anim_kill(f_sprite,TILE_SLIPICE+6);
			break;
		case COIN_2: 
			spr_anim_kill(f_sprite,TILE_800_COIN);
			break;
		case COIN_1: 
			spr_anim_kill(f_sprite,TILE_800_COIN);
			break;
		case FIREBALL_RED:
			spr_anim_kill(f_sprite,spr_tile(f_sprite)+3);
			break;
		case FIREBALL_GREEN:
			spr_anim_kill(f_sprite,spr_tile(f_sprite)+3);
			break;
		default:
			spr_anim_fall(f_sprite);
			break;
	}
	return 0;
}

void spr_anim_fall( unsigned char f_sprite) {
	if ( lin[f_sprite] > 48 && lin[f_sprite] < GAME_LIN_FLOOR && (lin[f_sprite] & 5) == 0) {
		index1 = game_calc_index ( lin[f_sprite]-16 , col[f_sprite]);
		tmp  = lvl_1[index1] == 18 || lvl_1[index1] == 20;
		tmp0 = lvl_1[index1+1] == 18 || lvl_1[index1+1] == 20;
		if (tmp || tmp0) {
			s_col1 = col[f_sprite];
			s_tile1 = spr_idx[lvl_1[index1]];
			s_lin1 = lin[f_sprite] - 16;
			
			if ( !tmp && tmp0 ) {
				s_col1 = col[f_sprite] + 1;
				s_tile1 = spr_idx[lvl_1[index1+1]];
			}
			if (tmp && !tmp0) {
				s_col1 = col[f_sprite] - 1;
				s_tile1 = spr_idx[lvl_1[index1]];
			}
			NIRVANAP_drawT( s_tile1 , s_lin1, s_col1 );
		}
	}

	NIRVANAP_fillT(PAPER, lin[f_sprite] - 8, col[f_sprite]);
	NIRVANAP_spriteT(f_sprite, tile[f_sprite], lin[f_sprite], col[f_sprite]);
	//FIX POW
	if ( lin[f_sprite] == 140 || lin[f_sprite] == 148 ) game_draw_pow();
	//FIX PIPES 
	if (lin[f_sprite] > 144) {
		s_col1 = col[f_sprite];
		game_draw_water_splash(s_col1);
		if (f_sprite >= SPR_P2)  {
			//PLAYERS
			if ( player_lost_life() ) {
				NIRVANAP_fillT(PAPER, lin[f_sprite], col[f_sprite]);
				player_restart(f_sprite);
			} else {
				spr_destroy(sprite); //TWO PLAYER GAMES
			}
		} else {
			//ENEMIES
			spr_destroy(f_sprite);
		}
		if (s_col1 <= 4  ) game_back_fix3();
		if (s_col1 >= 26 ) game_back_fix4();
	} else {
		lin[f_sprite] = lin[f_sprite] + 4;
	}
}

void spr_anim_kill(unsigned char f_sprite, unsigned int f_tile) {
	if ( game_check_time(last_time[f_sprite],10) ) {
		last_time[f_sprite] = zx_clock();
		tmp0 = zx_clock() - spr_timer[f_sprite];
		tmp = 2;
		if ( tmp0 <= 40 ) {
			tmp = 1;
		}
		if ( tmp0 <= 20 ) {
			tmp = 0;
		}
		if ( tmp0 <= 60 ) {
			NIRVANAP_spriteT(f_sprite, f_tile + tmp, lin[f_sprite], col[f_sprite]);
		} else {
			spr_destroy(f_sprite);
		}
	}
}

unsigned char spr_collition_check(unsigned char f_dir) {
	index1 = game_calc_index( lin[sprite] , col[sprite] );
	if (f_dir == DIR_RIGHT) {
		if (col[sprite]==31) return 0;
		index1 += 2;
	} else {
		if (col[sprite]==0) return 0;
		index1 -= 1;
	}
	if ( lvl_1[ index1] > VAL_COL ) return 1;
	index2   = index1 + SCR_COLS;
	if ( lvl_1[ index2] > VAL_COL ) return 1;
	index3   = index1 + 2*SCR_COLS;
	if ( lvl_1[ index3] > VAL_COL ) return 1;
	return 0;
}

unsigned char spr_check_over( void ){
	if (s_lin0 >= 136) {
		if( s_col0 < 4) {
			NIRVANAP_halt();
			game_back_fix3();
			return 1;
		}
		if( s_col0 > 26) {
			NIRVANAP_halt();
			game_back_fix4();
			return 1;
		}
	} else {
		if (s_lin0 <= 40) {
			if( s_col0 < 4) {
				NIRVANAP_halt();
				game_back_fix1();
				return 1;
			}
			if( s_col0 > 26) {
				NIRVANAP_halt();
				game_back_fix2();
				return 1;
			}
		}
	}
	return 0;
}

void spr_destroy(unsigned char f_sprite) {
	spr_count--;
	s_lin0 = lin[f_sprite];
	s_col0 = col[f_sprite];
	tile[sprite] = TILE_EMPTY;
	col[sprite] = 0;
	lin[sprite] = 0;
	class[f_sprite] = 0;
	state[f_sprite] = 0;
	NIRVANAP_spriteT(f_sprite, TILE_EMPTY, 0,0);
	NIRVANAP_fillT(PAPER, s_lin0, s_col0);
	if (!game_over)	spr_check_over();
}

void spr_set_fall( void ) {
	BIT_CLR(s_state, STAT_JUMP);
	BIT_SET(s_state, STAT_FALL);
}

int spr_tile(unsigned char f_sprite){
	if ( BIT_CHK(state[f_sprite],STAT_ANGRY) ) {
		tmp = 3;
	} else {
		tmp = 0;
	}
	
	if ( BIT_CHK(state_a[f_sprite], STAT_TURN) ) {
		tmp = 9;
	}
	
	
	switch(class[f_sprite]) {
		case SHELLCREEPER_GREEN:
			return spr_tile_dir(TILE_SHELLCREEPER_GREEN, f_sprite,3);
			break;
		case SHELLCREEPER_RED :
			return spr_tile_dir(TILE_SHELLCREEPER_RED, f_sprite,3);
			break;
		case SHELLCREEPER_BLUE:
			return spr_tile_dir(TILE_SHELLCREEPER_BLUE, f_sprite,3);
			break;
		case SIDESTEPPER_RED :
			return (TILE_SIDESTEPPER_RED + tmp);
			break;
		case SIDESTEPPER_GREEN :
			return (TILE_SIDESTEPPER_GREEN + tmp);
			break;
		case SIDESTEPPER_MAGENTA :
			return (TILE_SIDESTEPPER_MAGENTA + tmp);
			break;
		case SLIPICE :
			return TILE_SLIPICE;
			break;
		case COIN_1: 
			return TILE_COIN1;
			break;
		case COIN_2: 
			return TILE_COIN2;
			break;
		case FIGHTERFLY:
			return TILE_FIGHTERFLY;
			break;
		case FIREBALL_RED:
			return TILE_FIREBALL_RED;
			break;
		case FIREBALL_GREEN:
			return TILE_FIREBALL_GREEN;
			break;
	}
	return 0;
}

int spr_tile_dir( unsigned int f_tile, unsigned char f_sprite, unsigned char f_inc) {
	if ( BIT_CHK(state[f_sprite], STAT_HIT) ) {
		return f_tile + 6;
	}
	
	if ( BIT_CHK(state_a[f_sprite], STAT_TURN) ) {
		return f_tile + 9;
	}
	
	if ( BIT_CHK(state[f_sprite], STAT_DIRR) ) {
		return f_tile;
	} else {
		return f_tile + f_inc;
	}
}
