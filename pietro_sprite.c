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
#include "pietro_enemies.h"
#include "pietro_sprite.h"
#include "pietro_zx.h"
#include "macros.h"

unsigned char spr_chktime( unsigned char *sprite ) __z88dk_fastcall {
	//if (FULL_SPEED) return 1;
	if (sprite_speed_alt[*sprite] == 0) {
		tmp = sprite_speed[class[*sprite]];
	} else {
		tmp = sprite_speed_alt[*sprite];
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
	tmp1 = lin[sprite] - (SPRITE_LIN_INC << sprite_lin_inc_mul); /* x << k == x multiplied by 2 to the power of k */
	if (sprite >= SPR_P2 || class[sprite] == FIREBALL_RED) {
//		if ((tmp1 & 7) == 0) {
			index1 = game_calc_index( tmp1 , col[sprite]);
			//index2 = index1 + 1;
			if ( !game_check_maze( index1 ) ) { // || !game_check_maze( index2 ) ) {
				/* Only Players can hit objects */
				if (sprite >= SPR_P2 && !BIT_CHK( state_a[sprite] , STAT_HITBRICK ) ) {
					if (  !player_hit_pow() ) {
						player_hit_brick();
					}
				}
				return 1;
			}
//		}
	}

	lin[sprite] = tmp1;
	if (lin[sprite] == 0 || lin[sprite] > GAME_LIN_FLOOR ) { /* Signed Variable */
		//spr_set_fall();
		lin[sprite] = 0;
		NIRVANAP_fillT(18, s_lin0,s_col0);
		return 1;
	}
	return 0;
}

unsigned char spr_move_down( void ){
	//12
	//43
	tmp1 = lin[sprite] + (SPRITE_LIN_INC << sprite_lin_inc_mul);
//	if ((tmp1 & 7) == 0) {
		index1 = game_calc_index( tmp1 + SPRITE_HEIGHT, col[sprite] );
		if ( !game_check_maze( index1 ) ) {
			lin[sprite] = 8 * (tmp1 / 8);
			return 1;
		}
//	}
	lin[sprite] = tmp1;
	if (lin[sprite] > GAME_LIN_FLOOR) {
		BIT_CLR(s_state, STAT_FALL);
		lin[sprite] = GAME_LIN_FLOOR;
		return 1;
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
/* 		if ( BIT_CHK(state[sprite], STAT_JUMP) || BIT_CHK(state[sprite], STAT_FALL)) {
			if ( spr_collision_check(DIR_RIGHT) ) {
				colint[sprite] = SPR_COLINT -1 ;
				return 0;
			}
		} */
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
/* 		if ( BIT_CHK(state[sprite], STAT_JUMP) || BIT_CHK(state[sprite], STAT_FALL)) {
			if ( spr_collision_check(DIR_LEFT) ) {
				colint[sprite] = 0;
				return 0;
			}
		} */
		colint[sprite] = SPR_COLINT - 1;
		--col[sprite];
		if (col[sprite] == 255) {
			col[sprite] = SCR_COLS_M;
		}
	}
	return 0;
}

unsigned char spr_redraw( void ){
	s_tile1 = tile[sprite] + colint[sprite];

	if ( (lin[sprite] !=  s_lin0) || (col[sprite] != s_col0) ) {
		/* Column Movement */
		if ( s_lin0 <= 16 ) {
			NIRVANAP_fillT(PAPER, 8, s_col0);
		}
		NIRVANAP_spriteT(sprite, s_tile1, lin[sprite], col[sprite]);

		if ( !spr_check_over() ) {

			if ( BIT_CHK(state[sprite], STAT_JUMP) ) {
				spr_back_fix(16);
				return 0;
			}
			if ( BIT_CHK(state[sprite], STAT_FALL) ) {
				spr_back_fix(0);
				return 0;
			}
			spr_back_clr();
		}


	} else if (  s_tile1 != s_tile0 ) {
		/* Internal Movement */
		NIRVANAP_spriteT(sprite, s_tile1, lin[sprite], col[sprite]);
	}
	return 0;

}

unsigned char spr_killed( unsigned char f_sprite) __z88dk_fastcall {
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

void spr_anim_fall( unsigned char f_sprite) __z88dk_fastcall {

		if (lin[f_sprite] < GAME_LIN_FLOOR) {

			/* Move sprite down screen n draw*/
			s_col0 = col[f_sprite];
			s_lin0 = lin[f_sprite];
			enemy_ugly_fix();
			lin[f_sprite] = s_lin0+ (SPRITE_LIN_INC << 1);
			NIRVANAP_spriteT(f_sprite, tile[f_sprite], lin[f_sprite], s_col0);
			/* Sprite Falling */
			spr_back_fix(0);



		} else {
			s_col1 = col[f_sprite];
			spr_water_splash_draw(s_col1);
			/* Sprite reach floor */
			if (f_sprite >= SPR_P2)  {
				/* Player Die */
				if ( player_lost_life() ) {
					/* Player Lost a Life */
					NIRVANAP_halt();
					NIRVANAP_fillT(PAPER, lin[f_sprite], col[f_sprite]);
					player_restart(f_sprite);
				} else {
					/* Player Dies */
					game_print_footer();
					NIRVANAP_fillT(PAPER, lin[f_sprite], col[f_sprite]);
					col[f_sprite] = 0;
					lin[f_sprite] = 0;
					class[f_sprite] = 0;
					state[f_sprite] = 0;
					NIRVANAP_spriteT(f_sprite, TILE_EMPTY, 0,0);
				}
			} else {

				/*Set end of phase to be readed on game_loop*/
			  if (phase_left <= 0) phase_end = 1;
				/* Enemy dies */
				spr_destroy(f_sprite);

			}
			/* Restore lower pipes */
			if (s_col1 <= 4  ) {
				spr_back_paint(0 + 15 * 32);
				spr_draw_index(12*32);
				spr_draw_index(12*32 + 2);
			}

			if (s_col1 >= 26 ) {
				spr_back_paint(26 + 15 * 32);
				spr_draw_index(12*32 + 28);
				spr_draw_index(12*32 + 30);
      }
		}
//	}
}

void spr_anim_kill(unsigned char f_sprite, unsigned int f_tile) {

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
		spr_draw_pow();
	}

}

/*
unsigned char spr_collision_check(unsigned char f_dir) {
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
*/

unsigned char spr_check_over( void ){
	if (s_lin0 >= 136) {
		if( s_col0 < 4) {
			spr_back_paint(0 + 15 * 32);
			return 1;
		}
		if( s_col0 > 26) {
			spr_back_paint(26 + 15 * 32);
			return 1;
		}
	} else {
		if (s_lin0 <= 40) {
			if( s_col0 < 4) {
				spr_back_paint(0);
				return 1;
			}
			if( s_col0 > 26) {
				spr_back_paint(26);
				return 1;
			}
		}
	}
	return 0;
}

void spr_destroy(unsigned char f_sprite) __z88dk_fastcall {
	spr_count--;
	s_lin0 = lin[f_sprite];
	s_col0 = col[f_sprite];
	tile[f_sprite] = TILE_EMPTY;
	col[f_sprite] = 0;
	lin[f_sprite] = 0;
	class[f_sprite] = 0;
	state[f_sprite] = 0;
	state_a[f_sprite] = 0;
	NIRVANAP_spriteT(f_sprite, TILE_EMPTY, 0,0);
	NIRVANAP_halt();
	NIRVANAP_fillT(PAPER, s_lin0, s_col0);
	if (!game_over)	spr_check_over();
}

void spr_set_fall( void ) {
	BIT_CLR(s_state, STAT_JUMP);
	BIT_SET(s_state, STAT_FALL);
	if (sprite >= SPR_P2) {
		sprite_speed_alt[sprite] = PLAYER_FALL_SPEED;
	} else {
		if ( class[sprite] <= COIN_1 ) sprite_speed_alt[sprite] = ENEMY_FALL_SPEED;
	}

}

int spr_tile(unsigned char f_sprite) __z88dk_fastcall {
	if ( BIT_CHK(state[f_sprite],STAT_ANGRY) ) {
		tmp = 3;
	} else {
		tmp = 0;
	}

	if ( BIT_CHK(state[f_sprite], STAT_HIT) ) {
		tmp = 6;
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
	}
	if ( BIT_CHK(state[f_sprite], STAT_DIRL) ) {
		return f_tile + f_inc;
	}

	if ( BIT_CHK(state_a[f_sprite], STAT_LDIRR) ) {
		return f_tile;
	}
	if ( BIT_CHK(state_a[f_sprite], STAT_LDIRL) ) {
		return f_tile + f_inc;
	}
	return TILE_EMPTY;
}

void spr_draw_back(void) {
	intrinsic_di();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);

	spr_draw_row(6);
	spr_draw_row(11);
	spr_draw_row(12);
	spr_draw_row(16);

	NIRVANAP_fillT(PAPER,128,16); /* FIX POW */

  intrinsic_ei();
	spr_back_paint(0);
	spr_back_paint(26);
	spr_back_paint( 0 + ( 15 * 32) );
	spr_back_paint(26 + ( 15 * 32) );
	spr_draw_pow();


	NIRVANAP_halt();
	zx_print_ink(INK_YELLOW);
	zx_print_paper(PAPER_RED);
	game_fill_row(20,35);//brick row
	game_fill_row(21,35);//brick row
	zx_print_paper(PAPER_BLACK);
	zx_print_ink(INK_BLACK);
	game_fill_row(12,32);//clear row
	game_print_footer();
}

void spr_draw_clear(void) {
	intrinsic_di();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	//todo an asm routine to clear the screen fast (nirvana)
	for (s_lin1 = 16; s_lin1 <= 162; s_lin1+= 16) {
		for (s_col1 = 0; s_col1 < 32; s_col1+= 2) {
			NIRVANAP_drawT_raw(TILE_EMPTY, s_lin1, s_col1);
		}
	}
	intrinsic_ei();
}

void spr_draw_pow(void) {
  s_tile1 = TILE_EMPTY;
	switch(game_pow) {
		case 1:
			s_tile1 = TILE_POW1 + 24;
			break;
		case 2:
			s_tile1 = TILE_POW1 + 12;
			break;
		case 3:
			s_tile1 = TILE_POW1;
			break;
	}

	//NIRVANAP_halt(); // synchronize with interrupts  CG
	//NIRVANAP_fillT(PAPER, 128,15);
	NIRVANAP_drawT( s_tile1 , 128, 15 );
}
void spr_back_clr( void ) {
	NIRVANAP_fillC(PAPER, s_lin0, s_col0);
	NIRVANAP_fillC(PAPER, s_lin0, s_col0+1);
	NIRVANAP_fillC(PAPER, s_lin0+8, s_col0);
	NIRVANAP_fillC(PAPER, s_lin0+8, s_col0+1);
}

void spr_back_fix( unsigned char f_inc ) __z88dk_fastcall {
		index1 = game_calc_index ( s_lin0 + f_inc , s_col0);
		tmp  = lvl_1[index1] == GAME_MAP_PLATFORM || lvl_1[index1] == GAME_MAP_PLATFORM_FREEZE;
		tmp0 = lvl_1[index1+1] == GAME_MAP_PLATFORM || lvl_1[index1+1] == GAME_MAP_PLATFORM_FREEZE;
		spr_back_clr();
		if (tmp || tmp0) {

			/* Restore Platforms */
			/* The platform is made of brick or ice */
			s_col1 = s_col0;
			s_tile1 = spr_idx[lvl_1[index1]];
			s_lin1 = s_lin0 + f_inc;
			/* Calculate the lin from the 8x8 lvl_1 screen map */
			s_lin1 = s_lin1>>3; /* div 8 (2^3) */
			s_lin1 = s_lin1<<3; /* mul 8 (2^3) */
			/* Nirvana can't draw 8x8 :( */
			if ( !tmp && tmp0 ) {
				s_col1 = s_col0 + 1;
				s_tile1 = spr_idx[lvl_1[index1+1]];
			}
			/* Nirvana can't draw 8x8 :( */
			if (tmp && !tmp0) {
				s_col1 = s_col0 - 1;
				s_tile1 = spr_idx[lvl_1[index1]];
			}
			NIRVANAP_drawT( s_tile1 , s_lin1, s_col1 );
		}
		/* Fix Pow */
		if ( s_lin0 >= 112 && s_lin0 <= 144 && s_col0 >= 14 && s_col0 <= 16 ) {
			spr_draw_pow();
		}
}


void spr_back_paint(unsigned int f_inc) {
  NIRVANAP_halt();
	intrinsic_di();
	spr_draw_index(64 + f_inc);
	spr_draw_index(66 + f_inc);
	spr_draw_index(68 + f_inc);

	spr_draw_index(128 + f_inc);
	spr_draw_index(130 + f_inc);
	spr_draw_index(132 + f_inc);
	intrinsic_ei();
	NIRVANAP_halt();
}
/*
void spr_back_fix1(void) {

	NIRVANAP_halt(); // synchronize with interrupts
	NIRVANAP_drawT(TILE_PIPE5, 16, 0);
	NIRVANAP_drawT(TILE_PIPE4, 32, 0);
	NIRVANAP_drawT(TILE_PIPE2, 16, 2);
	NIRVANAP_fillT(PAPER, 32, 2);
	NIRVANAP_fillT(PAPER, 16, 4);
	NIRVANAP_fillT(PAPER, 32, 4);

}

void spr_back_fix2(void) {

	NIRVANAP_halt(); // synchronize with interrupts
	NIRVANAP_drawT(TILE_PIPE3, 16, 28);
	NIRVANAP_drawT(TILE_PIPE7, 16, 30);
	NIRVANAP_drawT(TILE_PIPE6, 32, 30);
	NIRVANAP_fillT(PAPER, 32, 28);
	NIRVANAP_fillT(PAPER, 16, 26);
	NIRVANAP_fillT(PAPER, 32, 26);

}



void spr_back_fix3(void) {
	NIRVANAP_halt(); // synchronize with interrupts
	NIRVANAP_drawT(TILE_PIPE1A, 136, 0);
	NIRVANAP_drawT(TILE_PIPE1B, GAME_LIN_FLOOR, 0);
	NIRVANAP_drawT(TILE_PIPE2A, 136, 2);
	NIRVANAP_drawT(TILE_PIPE2B, GAME_LIN_FLOOR, 2);
	NIRVANAP_fillT(PAPER, 136, 4);
	NIRVANAP_fillT(PAPER, GAME_LIN_FLOOR, 4);
}

void spr_back_fix4(void) {
	NIRVANAP_halt(); // synchronize with interrupts
	NIRVANAP_drawT(TILE_PIPE1A, 136, 30);
	NIRVANAP_drawT(TILE_PIPE1B, GAME_LIN_FLOOR, 30);
	NIRVANAP_drawT(TILE_PIPE3A, 136, 28);
	NIRVANAP_drawT(TILE_PIPE3B, GAME_LIN_FLOOR, 28);
	NIRVANAP_fillT(PAPER, 136, 26);
	NIRVANAP_fillT(PAPER, GAME_LIN_FLOOR, 26);
}
*/

void spr_brick_anim(unsigned char f_hit) __z88dk_fastcall {
	tmp0 = TILE_EMPTY;
	index2 = index1 + 1;
	if (lvl_1[ index1 ] == GAME_MAP_PLATFORM || lvl_1[ index2 ] == GAME_MAP_PLATFORM) {
		tmp0 = game_brick_tile;
	}
	if (lvl_1[ index1 ] == GAME_MAP_PLATFORM_FREEZE || lvl_1[ index2 ] == GAME_MAP_PLATFORM_FREEZE ) {
		tmp0 = TILE_BRICK_FREEZE;
	}

	//index1 = game_calc_index( hit_lin[index_player] - 8 , hit_col[index_player] );
	//index2 = index1 + 1;
	if (f_hit) {
		tmp = hit_lin[index_player]-10;
	} else {
		tmp = hit_lin[index_player]-8;
	}
	tmp_uc = 0;
	if (lvl_1[index1] >= GAME_MAP_PLATFORM && lvl_1[index2] == 0) {
		tmp_uc = hit_col[index_player] + 1;
	}
	if (lvl_1[index1] == 0 && lvl_1[index2] >= GAME_MAP_PLATFORM) {
		tmp_uc = hit_col[index_player];
	}
	/* Draw Plaform */
	if (f_hit) {
		NIRVANAP_drawT( tmp0 , tmp, hit_col[index_player] );
	} else {
		NIRVANAP_fillT( PAPER, hit_lin[index_player]-16, hit_col[index_player]);
		NIRVANAP_drawT( tmp0 , tmp, hit_col[index_player] );
	}
	if (tmp_uc != 0) {
		/* Clear end row Plaform */
		NIRVANAP_fillC(PAPER, tmp, tmp_uc);
		NIRVANAP_halt();
	}
}

/* Clear Screen With Bricks UDG */
void spr_cortina_brick(void) {
	zx_print_ink(INK_YELLOW);
	zx_print_paper(PAPER_RED);
	tmp = 0;
	tmp0 = 23;
	while (tmp < 13) {
		game_fill_row(tmp ,35); /*UDG 35 Brick*/
		game_fill_row(tmp0,35); /*UDG 35 Brick*/
		tmp0--;
		tmp++;
		z80_delay_ms(25);
	}
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	zx_print_paper(PAPER_BLACK);
}
/* Clear Screen With Pipes */


/*
void spr_cortina_pipes(void) {
 	unsigned char s_col1,s_lin1;
	for (s_col1 = 0; s_col1 < 32; s_col1+= 2) {
		for (s_lin1 = 16; s_lin1 <= 176; s_lin1+= 32) {
			if ( s_lin1 == 16 || s_lin1 == 80 || s_lin1 == 144 ) NIRVANAP_halt(); //synchronize with interrupts every 2*3=6 draws
			if ( s_col1 < 30 ) NIRVANAP_drawT(179, s_lin1, s_col1 + 2); //pipe end right
			NIRVANAP_drawT(TILE_EMPTY, s_lin1+16,s_col1);
			NIRVANAP_drawT(172, s_lin1,s_col1);		 //pipe center
		}
	}
	for (s_col1 = 0; s_col1 < 32; s_col1+= 2) {
		for (s_lin1 = 16; s_lin1 <= 176; s_lin1+= 32) {
			if ( s_lin1 == 16 || s_lin1 == 80 || s_lin1 == 144 ) NIRVANAP_halt(); //synchronize with interrupts every 2*2=4 draws
			NIRVANAP_drawT(TILE_EMPTY, s_lin1,s_col1);		//clear
		if ( s_col1 < 30 ) NIRVANAP_drawT(167, s_lin1, s_col1 + 2); //pipe end left
		}
	}

	NIRVANAP_halt();
}
*/



void spr_water_splash_draw( unsigned char f_col) __z88dk_fastcall {
	if (spr_water_clear == 255) {
		/* water splash effect */
		zx_print_paper(PAPER_RED);
		zx_print_ink(INK_CYAN);
		zx_print_str(20, f_col, "()");//udg splash
		zx_print_ink(INK_WHITE);
		zx_print_str(21, f_col, "*+");//udg splash
		spr_water_clear = f_col;
		spr_water_time = zx_clock();
		zx_print_paper(PAPER_BLACK);
	}
}

void spr_water_splash_clear(void) {
	if ( spr_water_clear < SCR_COLS ) {
		if ( game_check_time( spr_water_time , TIME_WATER_SPLASH ) ) {
			zx_print_ink(INK_YELLOW);
			zx_print_paper(PAPER_RED);
			zx_print_str(20, spr_water_clear, "##"); //udg brick
			zx_print_str(21, spr_water_clear, "##"); //udg brick
			zx_print_ink(INK_WHITE);
			zx_print_paper(PAPER_BLACK);
			spr_water_clear = 255;
		}
	}
}

void spr_draw_index(unsigned int f_index) {
  /* n % 2^i = n & (2^i - 1) */
	s_col1 = f_index & 31; //OPTIMIZED % 32
	//s_col1 = index_d;
	s_lin1 = f_index >> 5;
	s_lin1 = s_lin1 << 3;

  //NIRVANAP_drawT(TILE_POW1, s_lin1, s_col1);
	NIRVANAP_drawT_raw(spr_idx[lvl_1[f_index]], s_lin1, s_col1);
}

void spr_draw_row(unsigned char f_row) {
  tmp = 0;
	while (tmp < 32) {
		spr_draw_index( (f_row << 5) + tmp); //TODO OPTIMIZE
		tmp = tmp + 2;
	}
}
/*
void spr_draw_edges(void) {
	NIRVANAP_halt();
  intrinsic_di();
	spr_draw_index(12*32);
	spr_draw_index(12*32 + 2);
	spr_draw_index(12*32 + 28);
	spr_draw_index(12*32 + 30);
	spr_draw_index(6*32 + 12);
	spr_draw_index(6*32 + 18);
  intrinsic_ei();

	NIRVANAP_halt();
  intrinsic_di();
	spr_draw_index(16*32 + 10);
	spr_draw_index(16*32 + 20);
	spr_draw_index(11*32 + 10);
	spr_draw_index(11*32 + 20);
  intrinsic_ei();
}
*/
