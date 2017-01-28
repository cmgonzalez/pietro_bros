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
#include <arch/zx.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "nirvana+.h"
#include "pietro.h"
#include "pietro_ay.h"
#include "pietro_enemies.h"
#include "pietro_game.h"
#include "pietro_player.h"
#include "pietro_sound.h"
#include "pietro_sprite.h"
#include "pietro_zx.h"
#include "macros.h"


unsigned int game_calc_index( unsigned char f_lin , unsigned char f_col ) {
	return f_col + ((f_lin >> 3) << 5);
}

void game_print_footer(void) {
	zx_print_str(22, 1, " "); 
	zx_print_str(22,30, " "); 
	zx_print_str(23, 1, "     ");
	zx_print_str(23,30, "     ");
	if (game_lives[0] > 0) {
		zx_print_ink(INK_RED);
		zx_print_str(22, 1, "<"); //live p1 hut	
		zx_print_ink(INK_YELLOW);
		zx_print_str(23, 1, "\\"); //live p1 face
	}
	if (game_lives[1] > 0) {
		zx_print_ink(INK_WHITE);
		zx_print_str(22, 30, ">"); //live p2 hut
		zx_print_ink(INK_YELLOW);
		zx_print_str(23, 30, "^"); //live p2 face
	}
	/* phase osd bottom*/
	game_print_lives();	
	
	zx_print_str(23, 11, "PHASE");
	zx_print_chr(23, 18, phase_curr+1);
}

void game_phase_print_score_back(void) {
	zx_print_ink(INK_RED);
	zx_print_str(0, 11, "$%|"); //top
	zx_print_ink(INK_BLUE);
	zx_print_str(0, 1, "[|"); //I
	if (game_two_player) {
		zx_print_ink(INK_GREEN);
		zx_print_str(0, 22, "]|"); //II
	}
}


void game_print_lives(void) {
	zx_print_ink(INK_WHITE);
	tmp = game_lives[0] - 1;
	if (tmp < 255) {
		zx_print_chr(23, 3, tmp ); //LIVE P1	
	}
	tmp = game_lives[1] - 1;
	if (tmp < 255) {
		zx_print_chr(23, 26, tmp ); //LIVE P2
	}
}


void game_tick(void) {
	++curr_time;
	zx_isr();
}

void game_start_timer(void) {
	NIRVANAP_isr[0] = 205;                                // call
	z80_wpoke(&NIRVANAP_isr[1], (unsigned int)game_tick); // game_tick
}

unsigned char game_phase_calc(void) {
	tmp_uc = phase_curr << 2; //*4
	//DON'T COUNT SLIPICES
	phase_quota[0]	= phases[tmp_uc+0];
	phase_quota[1]	= phases[tmp_uc+1];
	phase_quota[2]	= phases[tmp_uc+2];

	game_brick_tile = phases[tmp_uc+3]; //PHASE TILE IS ON 4 ELEMENT
	game_unfreeze_all();
	if ( game_brick_tile == TILE_BRICK_FREEZE ) {
		game_freeze_all();
	}
	spr_idx[18]		= game_brick_tile; //HACK
	return phase_quota[0] + phase_quota[1] + phase_quota[2];
}


void game_phase_init(void) {
	/*PHASE INIT*/
	phase_end = 0;
	phase_angry = 0;
	phase_coins = 0;
	phase_pop = 0;
	game_bonus = 0;
	entry_time = 0;
	zx_set_clock(0);
	frame_time = 0;
	score_osd_col[0] = 0xFF;
	score_osd_col[1] = 0xFF;
	/* PHASE OSD START */
	spr_draw_clear();
	spr_cortina_pipes();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	spr_count = 0;


	//GAME TYPES/BONUS
	if (game_type != GAME_RANDOM_TYPE) { 
		/* A or B Game */
		phase_left = game_phase_calc();
		phase_tot = phase_left;
		if (phase_left == 0) {
			/* Init a bonus stage */
			phase_bonus_total[0] = 0;
			phase_bonus_total[1] = 0;
			phase_left = 6;
			game_bonus = 1;
			/* Fixed coins - We can have only 6 by Nirvana Sprites Max (8-2players)*/
			enemy_init( 0, 144 ,  8, COIN_2	, 0);
			enemy_init( 1, 144 , 22, COIN_2	, 0);
			enemy_init( 2,	80 ,  6, COIN_2	, 0);
			enemy_init( 3,	80 , 24, COIN_2	, 0);
			enemy_init( 4,	16 ,  5, COIN_2	, 0);
			enemy_init( 5,	16 , 25, COIN_2	, 0);
		}
	} else {
		/* Random Game */
		phase_left = 255;
		phase_curr = 255;
		game_brick_tile = TILE_BRICK_RESTART;
		spr_idx[18] = TILE_BRICK_RESTART;
	}
	/*PHASE TUNE*/
	ay_reset();
	if (game_bonus == 0) ay_midi_play(pb_midi_phase_1);
	game_print_header();
	/*PRINT PHASE MESSAGE*/
	game_phase_print(12);
	/* PLAYER INIT */
	if (game_lives[0]) player_init(SPR_P1,GAME_LIN_FLOOR,10,TILE_P1_STANR);
	if (game_lives[1]) player_init(SPR_P2,GAME_LIN_FLOOR,20,TILE_P1_STANR +24+12);

	/*Draw Platforms*/
	spr_draw_back();
	game_print_header();
	game_print_footer();
}

void game_print_header(void) {
	game_phase_print_score_back();
	zx_print_ink(INK_WHITE);
//	if (game_two_player) zx_print_int(0,25 , 0);
	/* Print score */
	game_print_score();
}


void game_phase_print(unsigned char f_row) __z88dk_fastcall {
	zx_print_str(f_row, 11, "PHASE");
	if (phase_curr < 31) {
		if (!game_bonus) {
			zx_print_chr(f_row, 17, phase_curr+1);	
		} else {
			zx_print_str(f_row, 17, "BONUS");	
		}
		
	} else {
		if (game_type != GAME_RANDOM_TYPE) {
			zx_print_str(f_row, 17, "HELL");
		} else {
			zx_print_str(f_row, 17, "INFITY");
		}
	}
	game_colour_message( f_row, 11, 24, 300 );
}

void game_loop(void) {
	zx_print_str(22,7,"   THANKS ALVIN   ");
	zx_print_str(23,7,"    AND  EINAR    ");
	ay_fx_play(ay_effect_10);
	sound_coin();
	z80_delay_ms(200);
	zx_print_str(22,7,"                  ");
	ay_reset();
	/*restore pow on map*/
	lvl_1[POW_INDEX     ] = 17;
	lvl_1[POW_INDEX +  1] = 17;
	lvl_1[POW_INDEX + 32] = 17;
	lvl_1[POW_INDEX + 33] = 17;
	game_lives[0] = 4;
	if (game_two_player) {
		game_lives[1] = 4;
	} else {
		game_lives[1] = 0;
	}

	player_score[0] = 0;
	player_score[1] = 0;
	player_next_extra[0] = GAME_EXTRA_LIFE;
	player_next_extra[1] = GAME_EXTRA_LIFE;
	spr_water_clear = 255; //255 = no need to clear the brick udg row
	if (game_type ==0) {
		game_time_flipped = TIME_FLIPPED_A;
		game_time_fireball_start = TIME_FIREBALL_A;
	} else {
		game_time_flipped = TIME_FLIPPED_B;
		game_time_fireball_start = TIME_FIREBALL_B;
	}
	/* screen init */
	game_over = 0;
	game_pow = 3;
	/* phase init */
	phase_curr = 0; //TESTING - Default 0
	game_phase_init();
	/* game loop start */
	sprite_lin_inc_mul = 0;
	loop_count=0;
	dirs = 0x00;
	game_unfreeze_all();
	game_joystick_set();
	while (!game_over) {
		/*player 1 turn*/
		player_set1();
		player_turn();
		/*player 2 turn*/
		player_set2();
		player_turn();
		/*enemies turn*/
		enemy_turn();
		/*bonus tick tack sound*/
		if (game_bonus) {
			game_bonus_clock();
			if (phase_left > 0 && !ay_is_playing()) ay_fx_play(ay_effect_19);
		}
		/* clear water effect */
		spr_water_splash_clear();
		
		/*each second aprox - update fps/score/phase left/phase advance*/
		if (game_check_time(frame_time, TIME_EVENT)) {
			frame_time = zx_clock();
			/*add enemies*/
			if ( !game_bonus ) {
				game_enemy_add();
			}
			
			/* angry last enemy on the screen */
			if (phase_left == 1 && phase_angry == 0) {
				for (sprite = 0; sprite < SPR_P2 ; ++sprite ) {
					if ( class[sprite] > 0 && class[sprite] <= FIGHTERFLY && !BIT_CHK(state[sprite],STAT_KILL) ) {
						phase_angry = 1;
						/*evolve twice to get them blue!*/
						enemy_evolve();
						enemy_evolve(); 
						tile[sprite] = spr_tile(sprite);
					}
				}
			}
			
			/* end of phase */
			if (phase_end == 1 && game_type != GAME_RANDOM_TYPE) {
				/*silence background sound*/
				z80_delay_ms(400);
				if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_reset();
				/*sprites init*/
				game_kill_all_sprites();					
				/*bonus summary*/
				if (game_bonus) game_bonus_summary();
				/*increment phase*/
				++phase_curr;
				if (phase_curr > 31) {
					/*game end*/
					game_end();								
					game_over = 1;
				} else {
					/*next phase*/
					game_phase_init();
				}
			}
		}
		++loop_count;
		
	}
	z80_delay_ms(800);
	game_kill_all_sprites();
	NIRVANAP_halt();
	spr_draw_clear();
	zx_print_str(8, 11, "GAME OVER");
	game_colour_message( 8, 11, 21, 200 );
	game_hall_enter();
	ay_reset();
	game_menu_sel = 0;
	game_menu_paint();
	game_joystick_set_menu();
}

void game_score_osd(void) {
	if (score_osd_col[index_player] == 255 ) {
		if ( hit_count > 0 ) {

			score_osd_col[index_player] = col[sprite];
			score_osd_lin[index_player] = lin[sprite];
			score_osd_time[index_player] = zx_clock();

			if ( hit_count == 1) {
				score_osd_tile[index_player] = TILE_800;
			} else	if ( hit_count < 8) {
				score_osd_tile[index_player] = TILE_NICE;
			} else {
				score_osd_tile[index_player] = TILE_EXTRA;
			}
		}
	} else {
		tmp = score_osd_lin[index_player] - 2;
		index1 = game_calc_index( tmp , score_osd_col[index_player]  );
		if ( tmp > 8 && index1 > 0 && lvl_1[index1] == 0 && lvl_1[index1+1] == 0   ) {
			NIRVANAP_halt(); // synchronize with interrupts
			NIRVANAP_drawT( score_osd_tile[index_player], score_osd_lin[index_player], score_osd_col[index_player] );
			score_osd_lin[index_player] = tmp;
		}
		
		if ( game_check_time( score_osd_time[index_player] , 50 ) ) {
			NIRVANAP_fillT( PAPER, score_osd_lin[index_player], score_osd_col[index_player] );
			score_osd_col[index_player] = 255;
		}
	}
}

void game_bonus_clock(void) {
	tmp_ui = TIME_BONUS - zx_clock()*2;           // one frame is 0.02 seconds
	if (tmp_ui > TIME_BONUS) {
		tmp_ui = 0;          // if time remaining goes negative
		phase_left = 0;
		phase_end = 1;
		ay_reset();
	} else {
		if (phase_left > 0) {
			zx_print_bonus_time(2,14,tmp_ui);
			if ( game_check_time(frame_time,5) ) {
				/* rotate attrib array */
				frame_time = zx_clock();
				game_paint_attrib_lin_h(14,14+6,2*8 + 8);
				game_rotate_attrib();
			}
		}
		/* end bonus! */
	}
}

void game_bonus_summary(void) {
	game_kill_all_sprites();
	spr_draw_clear();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
//	game_phase_print_score_back(1);
	game_phase_print_score_back();
	game_print_score();
	if (game_lives[0] > 0) game_bonus_summary_player(0);
	if (game_lives[1] > 0) game_bonus_summary_player(1);
	game_paint_attrib_lin( 0, 31, 18*8 + 8 );
	if ( phase_bonus_total[0] == 6 && phase_bonus_total[1] == 0) {
		sound_jump();
		if (phase_curr == 3) {
			zx_print_str(18, 6, "PERFECT!! 3000PTS'");
			index_player = 0;
			player_score_add(300);
			index_player = 1;
			player_score_add(300);
		} else {
			zx_print_str(18, 6, "PERFECT!! 5000PTS'");
			index_player = 0;
			player_score_add(500);
			index_player = 1;
			player_score_add(500);
		}
		game_colour_message( 18, 16, 26, 200 );
	} else {
		zx_print_str(18, 12, "NO BONUS");
		z80_delay_ms(1000);
	}
}

void game_bonus_summary_player(unsigned char f_index) __z88dk_fastcall {
	NIRVANAP_halt(); // synchronize with interrupts
	if (f_index==0) {
		s_lin1 = 6;
		zx_print_str(s_lin1, 7, "PIETRO");
		NIRVANAP_drawT( TILE_P1_STANR , s_lin1*8 + 4 , 4 );
		index_player = 0;
	} else {
		s_lin1 = 12;
		zx_print_str(s_lin1, 7, "LUIZO");
		NIRVANAP_drawT( TILE_P1_STANR + 24 , s_lin1*8 + 4 , 4 );
		index_player = 1;
	}

	game_paint_attrib_lin( 7, 31, s_lin1*8 + 8 );

	tmp_uc = 0;
	while ( tmp_uc < phase_bonus_total[f_index] ) {
		if (tmp_uc < 3) {
			s_lin0 = (s_lin1*8)-8;
		} else {
			s_lin0 = (s_lin1*8)+8;
		}

		NIRVANAP_halt(); // synchronize with interrupts
		NIRVANAP_drawT( TILE_COIN2+2 , s_lin0 , 14 + 2*(tmp_uc % 3) );
		player_score_add(80);
		++tmp_uc;
		ay_fx_play(ay_effect_09);
		sound_coin();
		z80_delay_ms(50);
	}
	zx_print_str(s_lin1, 22, "X 800");
}


void game_kill_all_sprites(void) { //TODO MOVE TO PIETRO_SPRITE.C
	for (sprite = 0; sprite < 8 ; ++sprite ) {
		spr_destroy(sprite);
	}
}

unsigned char game_check_maze(int f_index) __z88dk_fastcall {
	return lvl_1[f_index] < VAL_COL && lvl_1[f_index+1] < VAL_COL;
}

unsigned char game_enemy_add(void) {
	
	if (game_type == GAME_RANDOM_TYPE) {
		game_enemy_rnd();
	} else {
		if ( spr_count < ENEMIES_MAX && 
			 game_check_time(entry_time ,100) && 
			 phase_left > 0 ) 
		{
			/* phase quota */
			game_enemy_quota();
		}	
	}
	
	return 0;
}

unsigned char game_enemy_quota(void) {
	
	tmp = rand() & 0x1;
	if (!phase_pop || phase_left == 1) {
		/* Force Enemy Popup */
		tmp = 0;
	}
	if (phase_left > 0 && tmp == 0 ) { //50%
		if ( phase_quota[2] ) {
			game_enemy_add1(FIGHTERFLY);
			phase_quota[2]--;
			return 0;
		}	
		if ( phase_quota[1] ) {
			game_enemy_add1(SIDESTEPPER_RED);
			phase_quota[1]--;
			return 0;
		}
		if ( phase_quota[0] ) {
			game_enemy_add1(SHELLCREEPER_GREEN);
			phase_quota[0]--;
			return 0;
		}
	}
	
	/* Other random enemies*/
	if (phase_coins > 0) {
		game_enemy_add1(COIN_1);
		return 0;
	}

	switch (rand() & 0x3) {	 //0,1,2,3
	case 0:
		if ( phase_curr > 7) game_enemy_add1(SLIPICE);
		break;
	case 1:
		if ( game_check_time( 0, game_time_fireball_start ) ) game_enemy_add1(FIREBALL_GREEN);
		break;
	case 2:
		if ( game_check_time( 0, game_time_fireball_start ) ) game_enemy_add1(FIREBALL_GREEN);
		break;
	case 3:
		if ( game_check_time( 0, game_time_fireball_start ) ) game_enemy_add1(FIREBALL_RED);
		break;
	};
	return 0;
}

unsigned char game_enemy_rnd(void) {
	if (spr_count < ENEMIES_MAX && game_check_time( entry_time , 100 ) ) {
		entry_time = zx_clock();
		/* adds an enemy */
		game_enemy_add1(rand() % 11 + 1); //TODO OPTIMIZE
	}
	return 0;
}


unsigned char game_enemy_add1(unsigned char f_class) __z88dk_fastcall {
	if	(
		(f_class == SHELLCREEPER_BLUE) ||
		(f_class == SIDESTEPPER_MAGENTA) ||
		(f_class == FIREBALL_RED   && game_enemy_add_get_index(FIREBALL_RED	 ) != 255 ) ||
		(f_class == FIREBALL_GREEN && game_enemy_add_get_index(FIREBALL_GREEN) != 255 ) ||
		(f_class == SLIPICE		   && game_enemy_add_get_index(SLIPICE		 ) != 255 )
	) {
		return 0;
	}
	//force for test an enemy
	++phase_pop;
	sound_enter_enemy();
	tmp = game_enemy_add_get_index(0);
	tmp0 = rand() & 0x1;

	if (tmp0 == 1) {
		s_lin0 = ENEMY_SLIN_L;
		s_col0 = ENEMY_SCOL_L;
		s_col1 = 2;
		if (f_class == FIREBALL_RED ) s_col1 = ENEMY_FIREBALL_START_COL_L;
		tmp1 = DIR_RIGHT;
	} else {
		s_lin0 = ENEMY_SLIN_R;
		s_col0 = ENEMY_SCOL_R;
		s_col1 = 30;
		if (f_class == FIREBALL_RED ) s_col1 = ENEMY_FIREBALL_START_COL_R;
		tmp1 = DIR_LEFT;
	}

	if (f_class == COIN_1) {
		--phase_coins;
	}
	
	tmp_uc = 0;
	if (f_class == FIREBALL_GREEN || f_class == FIREBALL_RED) {
		tmp_uc = rand() % 3;
		if (tmp_uc == 0) {
			tmp_uc = ENEMY_FIREBALL_START_TOP;  /* ROW TOP */
		}
		if (tmp_uc == 1) {
			tmp_uc = ENEMY_FIREBALL_START_MID;  /* ROW MIDDLE */
		}
		if (tmp_uc == 2) {
			tmp_uc = ENEMY_FIREBALL_START_BOT; /* ROW BOTTOM */
		}
		s_col0 = s_col1;
	}
	if ( ay_is_playing() < AY_PLAYING_FOREGROUND ) {
		if (f_class != COIN_1) {
			ay_fx_play(ay_effect_04);
		} else {
			ay_fx_play(ay_effect_07);
		}
		
	}
	entry_time = zx_clock();
	enemy_init( tmp, s_lin0 + tmp_uc , s_col0, f_class	, tmp1);

	return 0;
}

unsigned char game_enemy_add_get_index( unsigned char f_search) __z88dk_fastcall {
	for (enemies = 0; enemies <= 5 ; ++enemies ) {
		if ( class[enemies] == (unsigned char)f_search ) {
			return enemies;
		}
	}
	return 255;
}

void game_freeze(unsigned char f_lin, unsigned char f_col ) {
	if (f_col < 31) {
		tmp = 1;
		index1 = game_calc_index ( f_lin, f_col );

		if (lvl_1[index1] == GAME_MAP_PLATFORM ) {
			lvl_1[index1] = GAME_MAP_PLATFORM_FREEZE;
			NIRVANAP_halt(); // synchronize with interrupts
			NIRVANAP_drawT( TILE_BRICK_FREEZE , f_lin , f_col );
			if (lvl_1[index1+1] == 0) NIRVANAP_fillT(PAPER,f_lin,f_col+1);
			game_freeze(f_lin, f_col + 1);
			game_freeze(f_lin, f_col - 1);
		}
	}
}

void game_unfreeze_all(void) {
	for (index1 = 192; index1 <= 512+32; index1++ ) {
		if (lvl_1[index1] == GAME_MAP_PLATFORM_FREEZE ) {
			lvl_1[index1] = GAME_MAP_PLATFORM;
		}
	}
}

void game_freeze_all(void) {
	for (index1 = 192; index1 <= 512+32; index1++ ) {
		if (lvl_1[index1] == GAME_MAP_PLATFORM ) {
			lvl_1[index1] = GAME_MAP_PLATFORM_FREEZE;
		}
	}
}

void game_print_score(void) {
	zx_print_ink(INK_WHITE);
	zx_print_paper(PAPER_BLACK);
	zx_print_int(0,3 , player_score[0]);
	zx_print_int(0,14 , game_score_top); //SCORE TOP
	if (game_two_player) zx_print_int(0,24 , player_score[1]);
}


void game_paint_attrib(unsigned char e_r1) __z88dk_fastcall {
	for ( tmp0=e_r1; tmp0 <= 19; ++tmp0){
		game_paint_attrib_lin(1,31,(tmp0<<3)+8);
	}
}

void game_paint_attrib_lin(unsigned char f_start,unsigned char f_end,unsigned char f_lin) {
	for (tmp_uc = f_start; tmp_uc < f_end ; ++tmp_uc) {
		NIRVANAP_paintC(attrib, f_lin, tmp_uc);
	}
}

void game_paint_attrib_lin_h(unsigned char f_start,unsigned char f_end,unsigned char f_lin) {
	for (tmp_uc =f_start; tmp_uc < f_end ; ++tmp_uc) {
		NIRVANAP_paintC(attrib_hl, f_lin, tmp_uc);
	}
}

void game_menu_config(void) {
	char cont;
	game_paint_attrib(11);
	cont=1;
	while (cont) {
		while ((joyfunc1)(&k1) != 0);
		//MENU
        zx_print_str(14,10,spec128 ? "SOUND AY  " : "SOUND 48  ");
		zx_print_str(15,10,"GAME      ");
		zx_print_str(16,10,"P1 CTRL   ");
		zx_print_str(17,10,"P2 CTRL   ");
		zx_print_str(18,10,"BACK      ");
		
		/*Sound 48/AY*/
        zx_print_str(14,20,(game_sound & (GAME_SOUND_48_FX_ON | GAME_SOUND_AY_FX_ON)) ? "ON " : "OFF");
	
		/*Game Type*/
		switch ( game_type ) {	//0->3
		case 0:
			zx_print_str(15,20,"A  ");
			break;
		case 1:
			zx_print_str(15,20,"B  ");
			break;
		case 2:
			zx_print_str(15,20,"RND");
			break;
		}
		/*P1 Control*/
		zx_print_str(16,20,joynames[player_joy[0]]);
		/*P2 Control*/
		zx_print_str(17,20,joynames[player_joy[1]]);
		
		game_menu_sel = game_menu_handle(10, 1, 14, 18, 0);
		ay_fx_play(ay_effect_10); 
		sound_coin();

		switch ( game_menu_sel ) {
		case 0: //Sound 48/AY
            game_sound ^= spec128 ? GAME_SOUND_AY_FX_ON : GAME_SOUND_48_FX_ON;
			break;
		case 1: //Game type
			++game_type;
			if (game_type > GAME_RANDOM_TYPE) game_type = 0;
			break;
		case 2: //P1 Controls
			game_joystick_change(0);
			break;
		case 3: //P2 Controls
			game_joystick_change(1);
			break;
		case 4: //Back
			game_menu_sel = 0;
			game_menu_paint();
			cont = 0;
			//z80_delay_ms(250);
			break;
		}
	}
}

void game_joystick_change(unsigned char f_player_index){
		++player_joy[f_player_index];
		if (player_joy[f_player_index] == 7) player_joy[f_player_index] = 0; /* Rotate Joystick*/
		
		if (f_player_index == 0 && player_joy[f_player_index] == 1) ++player_joy[0];  /* player 1 cant have SJ2*/
		if (f_player_index == 1 && player_joy[f_player_index] == 0) ++player_joy[1];  /* player 2 cant have SJ1*/
		
		
		if (player_joy[0] == player_joy[1]) {
			if (f_player_index == 0) {
				player_joy[1] = 1; /* default */
			} else {
				player_joy[0] = 0; /* default */
			}
		} 
}

void game_joystick_set_menu(void){
	/* Default Values for menu */
	joyfunc1 = (uint16_t (*)(udk_t *))(in_stick_sinclair1);	
	joyfunc2 = (uint16_t (*)(udk_t *))(in_stick_sinclair2);
}

void game_joystick_set(void){
	joyfunc1 = control_method[ player_joy[0] ];
	joyfunc2 = control_method[ player_joy[1] ];
}


void game_menu(void) {
	/*PLAY MIDI TITLE*/
	game_menu_top_paint();
	game_menu_paint();
	ay_reset();
	ay_midi_play(pb_midi_title);
	while (1) { //ETHERNAL LOOP
		while ((joyfunc1)(&k1) != 0);
		tmp_uc = game_menu_handle(12, 2, 14, 18, 1000);
		switch ( tmp_uc ) {
		case 0: //1 PLAYER
			game_two_player = 0;
			game_loop();
			break;
		case 1: //2 PLAYER
			game_two_player = 1;
			game_loop();
			break;
		case 2: //OPTIONS
			game_menu_config();
			break;
		}
	}
}

void game_menu_top_paint(void) {
	spr_draw_clear();
	game_paint_attrib(0);
	//Draws menu
	
	//game_fill_row(0,32);
	//Blue frame top
	game_menu_e(16, 0,30,156,1);
	//Green frame bottom
	game_menu_e(80, 0,30,159,255); //game_menu_e(159,-1);
	//Pietro logo
	
	tmp = 24;
	intrinsic_di();
	for (tmp_uc =0; tmp_uc < 10 ; ++tmp_uc) {
		NIRVANAP_drawT_raw(192 +	   tmp_uc , tmp + 16 , 5 + (tmp_uc*2) );
		NIRVANAP_drawT_raw(192 + 12	 + tmp_uc , tmp + 32 , 5 + (tmp_uc*2) );
		//NIRVANAP_drawT(180 + 24	 + tmp_uc , tmp + 48 , 5 + (tmp_uc*2) );
	}
	intrinsic_ei();
	
}

void game_menu_paint(void) {
	NIRVANAP_halt();
	NIRVANAP_fillC(INK_RED | PAPER_RED , tmp + 40 , 26);//POINT
	//Menu
	
	game_fill_row(0,32);
	game_fill_row(0,32);	
	zx_print_str(14,10, "  1 PLAYER   ");
	game_fill_row(15,32);
	zx_print_str(16,10, "  2 PLAYER   ");
	game_fill_row(17,32);
	zx_print_str(18,10, "   CONFIG    ");
	game_fill_row(19,32);
	zx_print_ink(INK_BLUE);
	zx_print_str(22,7, "CODED BY CGONZALEZ");
	zx_print_str(23,7, "   VERSION  1.4  ");
	tmp_uc = 0; //fix menu return
	game_paint_attrib(11);
}

void game_menu_e(unsigned char f_col,unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign) {
	intrinsic_di();
	for (tmp_uc =e_c0; tmp_uc <= e_c1 ; tmp_uc = tmp_uc + 2) {
		if (tmp_uc == e_c0) {
			NIRVANAP_drawT_raw(e_start , f_col, tmp_uc );
			continue;
		}
		if (tmp_uc == e_c1) {
			NIRVANAP_drawT_raw(e_start + 2 , f_col, tmp_uc );
			continue;
		}
		if (tmp_uc == 14 || tmp_uc == 16) {
			continue;
		}
		NIRVANAP_drawT_raw(e_start + 1 , f_col, tmp_uc );
	}
	if ( f_sign == 255 ) {
		f_col = f_col-16;
	} else {
		f_col = f_col+16;
	}
	NIRVANAP_drawT_raw(e_start + 12 , f_col, e_c0 );
	NIRVANAP_drawT_raw(e_start + 14 , f_col, e_c1 );
	intrinsic_ei();

}

unsigned char game_menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout) {
	entry_time = zx_clock();
	s_lin1 = f_start + (game_menu_sel*f_inc);
	while ((dirs = (joyfunc1)(&k1)) != IN_STICK_FIRE) {
		if ( game_check_time(frame_time,5) ) {
			if (dirs != 0) {
				ay_fx_play(ay_effect_03);  /* effect 19 now repeats */
				sound_slide();
				game_paint_attrib_lin( f_col, 20 , (s_lin1*8)+8);
				s_lin1 += f_inc;
				if (s_lin1 > f_end	) {
					s_lin1 = f_start;
				}
			}
			/* rotate attrib array */
			frame_time = zx_clock();
			game_paint_attrib_lin_h( f_col, 20 , (s_lin1*8)+8);
			game_rotate_attrib();
		}
		if (timeout > 0 && game_check_time(entry_time, timeout) && !ay_is_playing() ) {
			game_hall_of_fame();
			game_menu_back(f_start);
		}
		
	};
	return	(unsigned char) ( s_lin1 -	f_start	 ) / f_inc;
}

void game_end(void) {
	unsigned char f_p1, f_p2;
	game_kill_all_sprites();
	spr_draw_clear();
	ay_midi_play(pb_midi_title);
	/* background */
	for (tmp0 = 4 ; tmp0 < 28 ; tmp0 = tmp0 + 2) NIRVANAP_drawT(TILE_GRASS,  80, tmp0);
	for (tmp0 = 4 ; tmp0 < 28 ; tmp0 = tmp0 + 2) NIRVANAP_drawT(TILE_GRASS,  96, tmp0);
	for (tmp0 = 4 ; tmp0 < 28 ; tmp0 = tmp0 + 2) NIRVANAP_drawT(TILE_GRASS, 112, tmp0);
	NIRVANAP_drawT(TILE_CASTLE      , 80, 24);
	
	game_menu_e(  32, 1, 29, 156,   1);
	game_menu_e( 128, 1, 29, 159, 255);
	f_p1 = 0;
	f_p2 = 0;
	
	if (game_two_player) {
		if (game_lives[0] > 0) f_p1 = 1;
		if (game_lives[1] > 0) f_p2 = 1;
	} else {
		f_p1 = 1;
	}

	/*pietro*/
	if ( f_p1 == 1 && f_p2 == 0 ) {
		zx_print_str(19, 1, "PIETRO...");
		NIRVANAP_drawT(TILE_P1_STANR    , 80, 12);
	}
	/*luizo*/
	if ( f_p1 == 0 && f_p2 == 1 ) {
		zx_print_str(19, 1, "LUIZO...");
		NIRVANAP_drawT(TILE_P1_STANR+24 , 96, 14);
	}
	/* pietro and luizo */
	if ( f_p1 == 1 && f_p2 == 1) {
		zx_print_str(19, 1, "PIETRO AND LUIZO...");
		NIRVANAP_drawT(TILE_P1_STANR    , 80, 12);
		NIRVANAP_drawT(TILE_P1_STANR+24 , 96, 14);
	}
	game_colour_message( 19, 1, 31, 200 );
	/* pietro */
	if ( f_p1 == 1 && f_p2 == 0 ) {
		NIRVANAP_drawT(TILE_EXTRA      , 64, 18);
		NIRVANAP_drawT(TILE_PRINCESS   , 80, 18);
		zx_print_str(19, 1, "FOUND THE PRINCESS!");
		NIRVANAP_drawT(TILE_EXTRA      , 64, 12);
	}
	/* luizo */
	if ( f_p1 == 0 && f_p2 == 1 ) {
		NIRVANAP_drawT(TILE_EXTRA      , 80, 18);
		NIRVANAP_drawT(TILE_PRINCESS   , 96, 18);
		zx_print_str(19, 1, "FOUND THE PRINCESS!");
		NIRVANAP_drawT(TILE_EXTRA      , 80, 14);
	}	
	/* pietro and luizo */
	if ( f_p1 == 1 && f_p2 == 1 ) {
		zx_print_str(19, 1, "FOUND THE PRINCESSES!");
		NIRVANAP_drawT(TILE_EXTRA      , 64, 18);
		NIRVANAP_drawT(TILE_PRINCESS   , 80, 18);
		
		NIRVANAP_drawT(TILE_EXTRA      , 80, 16);
		NIRVANAP_drawT(TILE_PRINCESS   , 96, 16);
		
		NIRVANAP_drawT(TILE_EXTRA      , 64, 12);
		NIRVANAP_drawT(TILE_EXTRA      , 80, 14);
	}
	game_colour_message( 19, 1, 31, 200 );
	zx_print_str(19, 1, "THANK YOU FOR PLAYING!");
	game_colour_message( 19, 1, 31, 200 );
	zx_print_str(19, 1, "SEE YOU!              ");
	game_colour_message( 19, 1, 31, 200 );
	spr_draw_clear();
}

#define HOF_ROW      12
#define HOF_P1_COL    6
#define HOF_P2_COL   22

HOF_ENTRY *game_hall_check(unsigned char p) __z88dk_fastcall
{	
	if (player_score[p] > hof[9].score) {
		hof[9].score = player_score[p];
		return &hof[9];
	}
	else if (player_score[p] > hof[8].score) {
		hof[8].score = player_score[p];
		return &hof[8];
	}
	
	return NULL;
}

int compare_scores(const HOF_ENTRY *e1, const HOF_ENTRY *e2)
{
	// sort in descending order
	return e2->score - e1->score;
}

void game_hall_enter(void) {
	HOF_ENTRY *p1, *p2;
	
	spr_draw_clear();
	game_paint_attrib(0);
	
	if (player_score[0] > player_score[1]) {
		p1 = game_hall_check(0);
		p2 = game_hall_check(1);
	}
	else
	{
		p2 = game_hall_check(1);
		p1 = game_hall_check(0);
	}

	if (p1 || p2)
	{
		unsigned char lock0  = 0;    // ignore input from player 1 until cleared
		unsigned char lock1  = 0;    // ignore input from player 2 until cleared
		unsigned char frame  = 0;    // pietro sprite jump frame (up/down)
		unsigned char index0 = 0;    // player 1 edit index into initials[] array + 1 == 0 if not editing
		unsigned char index1 = 0;    // player 2 edit index into initials[] array + 1 == 0 if not editing

		if (p1)
		{
			index0 = 1;
			game_hall_enter_phs(0);
		}
		
		if (p2)
		{
			index1 = 5;
			game_hall_enter_phs(1);
		}
		
		zx_print_str( 2,10, "HALL OF FAME");
		zx_print_str(18, 6, " CONGRATULATIONS !");
		zx_print_str(19, 6, "ENTER YOUR INITIALS");
		
		while ((unsigned char)(index0 + index1))
		{
			if (game_check_time(frame_time, 5))
			{
				frame = !frame;
				
				if (index0) game_hall_print_p(index0, frame);
				if (index1) game_hall_print_p(index1, frame);
				
				game_rotate_attrib();
				frame_time = zx_clock();
			}
			
			dirs = (joyfunc1)(&k1);
			if ((lock0 == 0) && index0 && ((index0 = game_hall_edit_p(index0)) == 0))
				strcpy(p1->name, &initials[0]);
			lock0 = dirs;
			
			dirs = (joyfunc2)(&k2);
			if ((lock1 == 0) && index1 && ((index1 = game_hall_edit_p(index1)) == 0))
				strcpy(p2->name, &initials[4]);
			lock1 = dirs;
		}
		
		ay_reset();
		spr_draw_clear();
		_insertion_sort_(hof, sizeof(hof)/sizeof(hof[0]), sizeof(hof[0]), compare_scores);
	}
	
	game_hall_of_fame();
	game_menu_back(0);
}

void game_menu_back(unsigned char f_start) __z88dk_fastcall
{
	game_menu_sel = 0;
	s_lin1 = f_start;
	game_menu_top_paint();
	game_menu_paint();
	entry_time = zx_clock(); 
}

void game_hall_enter_phs(unsigned char p) __z88dk_fastcall
{
	if (p == 0)
	{
		game_menu_e((HOF_ROW<<3)-48, HOF_P1_COL-4, HOF_P1_COL+6, 156,   1);
		game_menu_e((HOF_ROW<<3)+28, HOF_P1_COL-4, HOF_P1_COL+6, 159, 255);
		zx_print_str(HOF_ROW-5, HOF_P1_COL-1, "HIGH");
		zx_print_str(HOF_ROW-4, HOF_P1_COL-1, " SCORE");
	}
	else
	{
		game_menu_e((HOF_ROW<<3)-48, HOF_P2_COL-4, HOF_P2_COL+6, 156,   1);
		game_menu_e((HOF_ROW<<3)+28, HOF_P2_COL-4, HOF_P2_COL+6, 159, 255);
		zx_print_str(HOF_ROW-5, HOF_P2_COL-1, "HIGH");
		zx_print_str(HOF_ROW-4, HOF_P2_COL-1, " SCORE");
	}
}

void game_hall_print_p(unsigned char index, unsigned char frame)
{
	if (index < 5)
	{
		// player 1
		zx_print_str(HOF_ROW+1, HOF_P1_COL, &initials[0]);
		game_paint_attrib_lin_h(HOF_P1_COL-1+index, HOF_P1_COL+index, ((HOF_ROW+1)<<3)+8);
		
		NIRVANAP_halt();
		if (frame)
		{
			NIRVANAP_drawT(TILE_EMPTY, ((HOF_ROW+1)<<3)-16-2, HOF_P1_COL);
			NIRVANAP_drawT(3         , ((HOF_ROW+1)<<3)-16  , HOF_P1_COL);
		}
		else
		{
			NIRVANAP_drawT(TILE_EMPTY, ((HOF_ROW+1)<<3)-16  , HOF_P1_COL);
			NIRVANAP_drawT(22        , ((HOF_ROW+1)<<3)-16-2, HOF_P1_COL);
		}
	}
	else
	{
		// player 2
		zx_print_str(HOF_ROW+1, HOF_P2_COL, &initials[4]);
		game_paint_attrib_lin_h(HOF_P2_COL-5+index, HOF_P2_COL-4+index, ((HOF_ROW+1)<<3)+8);
		
		NIRVANAP_halt();
		if (frame)
		{
			NIRVANAP_drawT(TILE_EMPTY, ((HOF_ROW+1)<<3)-16-2, HOF_P2_COL+1);
			NIRVANAP_drawT(39        , ((HOF_ROW+1)<<3)-16  , HOF_P2_COL+1);
		}
		else
		{
			NIRVANAP_drawT(TILE_EMPTY, ((HOF_ROW+1)<<3)-16  , HOF_P2_COL+1);
			NIRVANAP_drawT(46        , ((HOF_ROW+1)<<3)-16-2, HOF_P2_COL+1);
		}
	}
}

unsigned char game_hall_edit_p(unsigned char index) __z88dk_fastcall
{
	--index;
	
	if (dirs & IN_STICK_FIRE)
	{
		if (index < 4)
			game_paint_attrib_lin(HOF_P1_COL + index, HOF_P1_COL+1 + index, ((HOF_ROW+1)<<3)+8);
		else
			game_paint_attrib_lin(HOF_P2_COL-4 + index, HOF_P2_COL-3 + index, ((HOF_ROW+1)<<3)+8);
		
		sound_slide();
		ay_fx_play(ay_effect_01);
		
		if (initials[index++] == '{')
		{
			// backspace
			if (--index & 0x3)
				initials[index--] = 'A';
		}

		if ((index & 0x3) == 0x3)
			return 0;
	}
	else if (dirs & (IN_STICK_LEFT | IN_STICK_RIGHT))
	{
		const unsigned char *p;
		
		p = strchr(hall_valids, initials[index]);
		
		if (dirs & IN_STICK_LEFT)
		{
			if (*--p == '\x01')
				p = hall_valids + sizeof(hall_valids) - 2;
		}
		else if (*++p == '\0')
			p = hall_valids + 1;
		
		initials[index] = *p;
	}

	return index+1;
}

void game_hall_of_fame(void) {
	//OUT OF MEMORY
	spr_draw_clear();
	game_paint_attrib(0);
	
	game_menu_e(16 ,6, 24,156,1);
	
	game_menu_e(GAME_LIN_FLOOR,6, 24,159,255);//game_menu_e(159,-1);
	//MENU
	for(s_lin1=0; s_lin1 < 10; ++s_lin1){
		zx_print_ink(INK_CYAN);
		zx_print_str( 7 + s_lin1, 10, hof[s_lin1].name);
		zx_print_int( 7 + s_lin1, 16, hof[s_lin1].score);
		zx_print_str( 7 + s_lin1, 14, "-");
		zx_print_str( 7 + s_lin1, 21, "0");
	}
	zx_print_str( 4, 10, "HALL OF FAME");
	game_colour_message( 4, 10,22, 500 );
	spr_draw_clear();
	game_menu_sel = 0;
	ay_midi_play(pb_midi_title);
}

void game_rotate_attrib(void) {
	//OUT OF MEMORY
	// UP
	// tmp_uc = attrib_hl[0];
	// attrib_hl[0] = attrib_hl[1];
	// attrib_hl[1] = attrib_hl[2];
	// attrib_hl[2] = attrib_hl[3];
	// attrib_hl[3] = tmp_uc;
	//DOWN
	tmp_uc = attrib_hl[3];
	attrib_hl[3] = attrib_hl[2];
	attrib_hl[2] = attrib_hl[1];
	attrib_hl[1] = attrib_hl[0];
	attrib_hl[0] = tmp_uc;
}

void game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_microsecs  ) {
	tmp = 1;
	frame_time = zx_clock();
	entry_time = zx_clock();
	while (tmp && !game_check_time( entry_time, f_microsecs)) {
		if ( game_check_time(frame_time,5) ) {
			//ROTATE ATTRIB ARRAY
			frame_time = zx_clock();
			game_paint_attrib_lin_h( f_col, f_col2 , (f_row<<3)+8);
			game_rotate_attrib();
		}
		while (((joyfunc1)(&k1) | (joyfunc2)(&k2)) != 0)
			tmp = 0;
	};
}

unsigned char game_check_time( unsigned int start, unsigned int lapse) {
	if (zx_clock() - start	> lapse) {
		return 1;
	} else {
		return 0;
	}
}

