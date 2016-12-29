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

#ifdef __SCCZ80
static unsigned char NIRVANAP_isr[3] @ (56698+328*NIRVANAP_TOTAL_ROWS);
#endif

unsigned int game_calc_index( unsigned char f_lin , unsigned char f_col ) {
	return f_col + ((f_lin >> 3) << 5);
}

void game_draw_pow(void) {
	NIRVANAP_halt();
	NIRVANAP_fillT(PAPER, 120,15);
	if (game_pow == 3) NIRVANAP_drawT( TILE_POW1	  , 120, 15 );
	if (game_pow == 2) NIRVANAP_drawT( TILE_POW1 + 12 , 120, 15 );
	if (game_pow == 1) NIRVANAP_drawT( TILE_POW1 + 24 , 120, 15 );
}


void game_back_fix1(void) {
	#ifdef __SDCC
	NIRVANAP_drawT(TILE_PIPE5, 16, 0);
	NIRVANAP_drawT(TILE_PIPE4, 32, 0);
	NIRVANAP_drawT(TILE_PIPE2, 16, 2);
	NIRVANAP_fillT(PAPER, 32, 2);
	NIRVANAP_fillT(PAPER, 16, 4);
	NIRVANAP_fillT(PAPER, 32, 4);
	#endif
}

void game_back_fix2(void) {
	#ifdef __SDCC
	NIRVANAP_drawT(TILE_PIPE3, 16, 28);
	NIRVANAP_drawT(TILE_PIPE7, 16, 30);
	NIRVANAP_fillT(PAPER, 32, 28);
	NIRVANAP_drawT(TILE_PIPE6, 32, 30);
	NIRVANAP_fillT(PAPER, 16, 26);
	NIRVANAP_fillT(PAPER, 32, 26);
	#endif
}

void game_back_fix3(void) {
	#ifdef __SDCC
	NIRVANAP_drawT(TILE_PIPE1A, 136, 0);
	NIRVANAP_drawT(TILE_PIPE1B, GAME_LIN_FLOOR, 0);
	NIRVANAP_drawT(TILE_PIPE2A, 136, 2);
	NIRVANAP_drawT(TILE_PIPE2B, GAME_LIN_FLOOR, 2);
	NIRVANAP_fillT(PAPER, 136, 4);
	NIRVANAP_fillT(PAPER, GAME_LIN_FLOOR, 4);
	#endif
}

void game_back_fix4(void) {
	#ifdef __SDCC
	NIRVANAP_drawT(TILE_PIPE1A, 136, 30);
	NIRVANAP_drawT(TILE_PIPE1B, GAME_LIN_FLOOR, 30);
	NIRVANAP_drawT(TILE_PIPE3A, 136, 28);
	NIRVANAP_drawT(TILE_PIPE3B, GAME_LIN_FLOOR, 28);
	NIRVANAP_fillT(PAPER, 136, 26);
	NIRVANAP_fillT(PAPER, GAME_LIN_FLOOR, 26);
	#endif
}

void game_back_fix5(void) {
#ifdef __SDCC
	index1 = game_calc_index( hit_lin[index_player] - 8 , hit_col[index_player] );
	index2 = index1 + 1;

	tmp_uc = 0;
	if (lvl_1[index1] >= IDX_BRICK && lvl_1[index2] == 0) {
		tmp_uc = hit_col[index_player] + 1;
	}
	if (lvl_1[index1] == 0 && lvl_1[index2] >= IDX_BRICK) {
		tmp_uc = hit_col[index_player] - 1;
	}
	if (tmp_uc != 0) {
		NIRVANAP_fillT(PAPER, hit_lin[index_player] - 10, tmp_uc);
	}
#endif
}

//Clear Screen

void game_cortina_brick(void) {
#ifdef __SDCC
	zx_print_ink(INK_YELLOW);
	zx_print_paper(PAPER_RED);
	tmp = 0;
	tmp0 = 23;
	while (tmp < 13) {
		game_fill_row(tmp,35);
		game_fill_row(tmp0,35);
		tmp0--;
		tmp++;
		z80_delay_ms(25);
	}
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	zx_print_paper(PAPER_BLACK);
#endif
}

void game_cortina_pipes(void) {

	unsigned char s_col1,s_lin1;
	for (s_col1 = 0; s_col1 < 32; s_col1+= 2) {
		for (s_lin1 = 16; s_lin1 <= 176; s_lin1+= 32) {
			if ( s_lin1 == 16 || s_lin1 == 80 || s_lin1 == 144 ) NIRVANAP_halt(); //synchronize with interrupts every 2*3=6 draws
			if ( s_col1 < 30 ) NIRVANAP_drawT(179, s_lin1, s_col1 + 2); //PIPE END RIGHT
			NIRVANAP_drawT(TILE_EMPTY, s_lin1+16,s_col1);
			NIRVANAP_drawT(172, s_lin1,s_col1);		 //PIPE CENTER
		}
	}
	for (s_col1 = 0; s_col1 < 32; s_col1+= 2) {
		for (s_lin1 = 16; s_lin1 <= 176; s_lin1+= 32) {
			if ( s_lin1 == 16 || s_lin1 == 80 || s_lin1 == 144 ) NIRVANAP_halt(); //synchronize with interrupts every 2*2=4 draws
			NIRVANAP_drawT(TILE_EMPTY, s_lin1,s_col1);		//CLEAR
			if ( s_col1 < 30 ) NIRVANAP_drawT(167, s_lin1, s_col1 + 2); //PIPE END LEFT
		}
	}
	NIRVANAP_halt();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);

}

void game_draw_clear(void) {
	intrinsic_di();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	//TODO AN ASM ROUTINE TO CLEAR THE SCREEN FAST (NIRVANA)
	for (s_lin1 = 16; s_lin1 <= 162; s_lin1+= 16) {
		for (s_col1 = 0; s_col1 < 32; s_col1+= 2) {
			NIRVANAP_drawT_raw(TILE_EMPTY, s_lin1, s_col1);
		}
	}
	NIRVANAP_start();
	intrinsic_ei();

}

void game_draw_back(void) {
	for (s_lin1 = 16; s_lin1 < 182; s_lin1 = s_lin1 + 8) {
		for (s_col1 = 0; s_col1 < 32; s_col1 = s_col1 + 2) {
			tmp_ui = game_calc_index(s_lin1,s_col1);
			if (lvl_1[tmp_ui] >= GAME_MAP_PLATFORM ) {
				NIRVANAP_drawT(game_brick_tile, s_lin1, s_col1);
			}
		}
	}
#ifdef GAME_LOW_MEM
	game_back_fix1();
	game_back_fix2();
	game_back_fix3();
	game_back_fix4();
	game_draw_pow();

	zx_print_ink(INK_YELLOW);
	zx_print_paper(PAPER_RED);
	game_fill_row(20,35);//BRICK ROW
	game_fill_row(21,35);//BRICK ROW
	zx_print_paper(PAPER_BLACK);
	zx_print_ink(INK_BLACK);
	game_fill_row(12,32);//CLEAR ROW
	game_fill_row(22,32);//CLEAR ROW
	game_fill_row(23,32);//CLEAR ROW
	zx_print_ink(INK_RED);
	zx_print_str(22, 1, "<"); //LIVE P1 HUT
	zx_print_ink(INK_YELLOW);
	zx_print_str(23, 1, "\\"); //LIVE P1 FACE
	if (game_two_player) {
		zx_print_str(23, 30, "^"); //LIVE P2 FACE
		zx_print_ink(INK_WHITE);
		zx_print_str(22, 30, ">"); //LIVE P2 HUT
	}
	/* PHASE OSD BOTTOM*/
	zx_print_ink(INK_WHITE);
	//game_phase_print(23);
	game_print_lives();
#endif
}

void game_phase_print_score_back(void) {
#ifdef GAME_LOW_MEM
	zx_print_ink(INK_RED);
	zx_print_str(0, 11, "$%|"); //TOP
	zx_print_ink(INK_BLUE);
	zx_print_str(0, 1, "[|"); //I
	if (game_two_player) {
		zx_print_ink(INK_GREEN);
		zx_print_str(0, 22, "]|"); //II
	}
#endif
}


void game_print_lives(void) {
	zx_print_ink(INK_WHITE);
	zx_print_chr(23, 3, game_lives[0]); //LIVE P1
	if (game_two_player) zx_print_chr(23, 26,game_lives[1]); //LIVE P2
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
	phase_angry = 0;
	game_bonus = 0;
	entry_time = 0;
	zx_set_clock(0);
	frame_time = 0;
	score_osd_col[0] = 0xFF;
	score_osd_col[1] = 0xFF;
	player_lock[0] = 1;
	player_lock[1] = 1;
	/* PHASE OSD START */
	game_draw_clear();
	game_cortina_pipes();
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
	spr_count = 0;
	/*EXTRA ZEROS FOR SCORES*/
	game_phase_print_score_back();
	zx_print_ink(INK_WHITE);
	//zx_print_int(0, 4 , 0);
	//zx_print_int(0,15 , 0);
	if (game_two_player) zx_print_int(0,25 , 0);
	/*PRINT SCORE*/
	game_print_score();

	//GAME TYPES/BONUS
	if (game_type < 2) { //A-B
		phase_left = game_phase_calc();
		if (phase_left == 0) {
			//INIT A BONUS STAGE
			phase_bonus_total[0] = 0;
			phase_bonus_total[1] = 0;
			phase_left = 6;
			game_bonus = 1;
			/* FIXED COINS - WE CAN HAVE ONLY 6! (8-2PLAYERS)*/
			enemy_init( 0, 144 , 13, COIN_2	, 0);
			enemy_init( 1, 144 , 17, COIN_2	, 0);
			enemy_init( 2,	80 ,  6, COIN_2	, 0);
			enemy_init( 3,	80 , 24, COIN_2	, 0);
			enemy_init( 4,	16 ,  5, COIN_2	, 0);
			enemy_init( 5,	16 , 25, COIN_2	, 0);
		}
	} else {
		phase_left = 0;
		phase_curr = 255;
		game_brick_tile = TILE_BRICK_RESTART;
		spr_idx[18] = TILE_BRICK_RESTART;
	}
	/*PHASE TUNE*/
	ay_reset();
	if (game_bonus == 0) ay_midi_play(pb_midi_phase_1);
	/*PRINT PHASE MESSAGE*/
	game_phase_print(12);
	/* PLAYER INIT */
	player_init(SPR_P1,GAME_LIN_FLOOR,10,TILE_P1_STANR);
	if (game_two_player) player_init(SPR_P2,GAME_LIN_FLOOR,20,TILE_P1_STANR +24+12);
	/*DRAW MAZE*/
	game_draw_back();
	zx_print_str(23, 11, "PHASE");
	zx_print_chr(23, 18, phase_curr+1);
}

void game_phase_print(unsigned char f_row) {
#ifdef __SDCC
	zx_print_str(f_row, 11, "PHASE");
	if (phase_curr < 31) {
		zx_print_chr(f_row, 18, phase_curr+1);
	} else {
		zx_print_str(f_row, 18, "HELL");
	}
	
	game_colour_message( f_row, 11, 22, 300 );
#endif
}

void game_loop(void) {
	zx_print_str(22,7,"   THANKS ALVIN   ");
	zx_print_str(23,7,"    AND  EINAR    ");
	ay_fx_play(ay_effect_10);
#ifdef __SDCC
	sound_coin();
	z80_delay_ms(200);
	zx_print_str(22,7,"                  ");
	ay_reset();
	/*RESTORE POW ON MAP*/
	lvl_1[495] = 17;
	lvl_1[495 + 1] = 17;
	lvl_1[495 + 32] = 17;
	lvl_1[495 + 33] = 17;
	game_lives[0] = 3;
	if (game_two_player) {
		game_lives[1] = 3;
	} else {
		game_lives[1] = 0;
	}
	
	player_score[0] = 0;
	player_score[1] = 0;
	player_next_extra[0] = GAME_EXTRA_LIFE;
	player_next_extra[1] = GAME_EXTRA_LIFE;
	game_water_clear = 255; //255 = NO NEED TO CLEAR THE BRICK UDG ROW
	if (game_type ==0) {
		game_time_flipped = TIME_FLIPPED_A;
		game_time_fireball_start = TIME_FIREBALL_A;
	} else {
		game_time_flipped = TIME_FLIPPED_B;
		game_time_fireball_start = TIME_FIREBALL_B;
	}
	/* SCREEN INIT */
	game_over = 0;
	game_pow = 3;
	/* PHASE INIT */
	phase_curr = 0; //TESTING 31
	game_phase_init();
	/* GAME LOOP START */
	loop_count=0;
	dirs = 0x00;
	while (!game_over) {
		/*PLAYER 1 TURN*/
		player_set1();
		player_turn();
		/*PLAYER 2 TURN*/
		player_set2();
		player_turn();
		/*ENEMIES TURN*/
		enemy_turn();
		/*EACH 30 MICROSECONDS APROX - UPDATE PLAYER COLLITION*/
		if (game_check_time(col_time, PLAYER_TCOL_CHECK)) {
			col_time = zx_clock();
			player_set1();
			player_collition();
			/*PLAYER 2*/
			player_set2();
			player_collition();
			/* WATER SPLASH EFFECT CLEAR */
			game_clear_water_splash();
			
			if (game_bonus) game_rotate_attrib();
		}
		if (game_bonus) {
			game_bonus_clock();
			if (!ay_is_playing()) ay_fx_play(ay_effect_19);
		}
		/*EACH SECOND APROX - UPDATE FPS/SCORE/PHASE LEFT/PHASE ADVANCE*/
		if (game_check_time(frame_time,100)) {
			frame_time = zx_clock();
			/*ADD ENEMIES*/
			if ( !game_bonus ) {
				game_enemy_add();
			}
			if (phase_left == 1 && phase_angry == 0) {
				for (sprite = 0; sprite < SPR_P2 ; ++sprite ) {
					if ( class[sprite] > 0 && class[sprite] <= FIGHTERFLY && !BIT_CHK(state[sprite],STAT_KILL) ) {
						phase_angry = 1;
						enemy_evolve();
						enemy_evolve(); //TWICE TO GET THEM BLUE!
						tile[sprite] = spr_tile(sprite);
					}
				}
			}
			if (phase_left == 0 && game_type != 2) {
				if (ay_is_playing() < AY_PLAYING_FOREGROUND) ay_reset();	//SILENCE BACKGROUND SOUND
				z80_delay_ms(800);
				game_kill_all_sprites();					//SPRITES INIT
				if (game_bonus) game_bonus_summary();
				++phase_curr;
				
				if (phase_curr > 31) {
					game_end();								//GAME END
					game_over = 1;
				} else {
					game_phase_init();
				}
			}
		}
		++loop_count;
	}
	game_kill_all_sprites();
	NIRVANAP_halt();
	game_draw_clear();
	zx_print_str(8, 11, "GAME  OVER");
	game_colour_message( 8, 11, 21, 200 );
	game_hall_enter();
	ay_reset();
	game_cortina_pipes();
	game_menu_sel = 0;
	game_menu_paint();
#endif
}

void game_score_osd(void) {
#ifdef __SDCC
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
		NIRVANAP_drawT( score_osd_tile[index_player], score_osd_lin[index_player], score_osd_col[index_player] );
		score_osd_lin[index_player] -=2;
		if ( game_check_time( score_osd_time[index_player] , 50 ) ) {
			NIRVANAP_fillT( PAPER, score_osd_lin[index_player], score_osd_col[index_player] );
			score_osd_col[index_player] = 255;
		}
	}
#endif
}

void game_bonus_clock(void) {
#ifdef __SDCC
	tmp_ui = TIME_BONUS - zx_clock()*2;           // one frame is 0.02 seconds
	if (tmp_ui > TIME_BONUS) tmp_ui = 0;          // if time remaining goes negative
	zx_print_bonus_time(2,14,tmp_ui);
	game_paint_attrib_lin_h(14,14+6,2*8 + 8);
	if (tmp_ui == 0) phase_left = 0;              // end bonus!
#endif
}

void game_bonus_summary(void) {
	game_kill_all_sprites();
	game_draw_clear();
#ifdef __SDCC
	zx_paper_fill(INK_BLACK | PAPER_BLACK);
//	game_phase_print_score_back(1);
	game_phase_print_score_back();
	game_print_score();
	game_bonus_summary_player(0);
	if (game_two_player) game_bonus_summary_player(1);
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
		z80_delay_ms(200);
	}
#endif
}

void game_bonus_summary_player(unsigned char f_index)  {
#ifdef __SDCC
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

		NIRVANAP_drawT( TILE_COIN2+2 , s_lin0 , 14 + 2*(tmp_uc % 3) );
		player_score_add(80);
		++tmp_uc;
		ay_fx_play(ay_effect_09);
		sound_coin();
		z80_delay_ms(50);
	}
	zx_print_str(s_lin1, 22, "X 800");
#endif
}

void game_draw_water_splash( unsigned char f_col) {
#ifdef __SDCC
	if (game_water_clear == 255) {
		/* WATER SPLASH EFFECT */
		zx_print_paper(PAPER_BLACK);
		zx_print_ink(INK_CYAN);
		zx_print_str(20, f_col, "()");//UDG SPLASH
		zx_print_ink(INK_WHITE);
		zx_print_str(21, f_col, "*+");//UDG SPLASH
		game_water_clear = f_col;
		game_water_time = zx_clock();
		zx_print_paper(PAPER_BLACK);
	}
#endif
}

void game_clear_water_splash(void) {
#ifdef __SDCC
	if ( game_water_clear < 32 ) {
		if ( game_check_time( game_water_time , GAME_TIME_WATER_SPLASH ) ) {
			zx_print_ink(INK_YELLOW);
			zx_print_paper(PAPER_RED);
			zx_print_str(20, game_water_clear, "##"); //UDG BRICK
			zx_print_str(21, game_water_clear, "##"); //UDG BRICK
			zx_print_ink(INK_WHITE);
			zx_print_paper(PAPER_BLACK);
			game_water_clear = 255;
			game_back_fix3(); //TESTING
			game_back_fix4(); //TESTING
		}
	}
#endif
}

void game_kill_all_sprites(void) { //TODO MOVE TO PIETRO_SPRITE.C
	for (sprite = 0; sprite < 8 ; ++sprite ) {
		spr_destroy(sprite);
	}
}

unsigned char game_check_maze(int f_index) {
	return lvl_1[f_index] < VAL_COL && lvl_1[f_index+1] < VAL_COL;
}

unsigned char game_enemy_add(void) {
	if (spr_count < ENEMIES_MAX && game_check_time(entry_time ,100) && ( phase_left > 0 || game_type == 2) && (phase_left > 0) ) {
		/* PHASE QUOTA */
		if (game_type == 2) {
			game_enemy_rnd();
		} else {
			game_enemy_quota();
		}
	}
	return 0;
}

unsigned char game_enemy_quota(void) {
	if (phase_left > 0 && ((rand() & 0x7) >= 3) ) { //0->7
		if ( phase_quota[0] ) {
			game_enemy_add1(SHELLCREEPER_GREEN);
			phase_quota[0]--;
			return 0;
		}
		if ( phase_quota[1] ) {
			game_enemy_add1(SIDESTEPPER_RED);
			phase_quota[1]--;
			return 0;
		}
		if ( phase_quota[2] ) {
			game_enemy_add1(FIGHTERFLY);
			phase_quota[2]--;
			return 0;
		}
	}
	/* RANDOM ENEMIES*/
	switch (rand() & 0x7) {	 //0->7
	case 0:
		game_enemy_add1(COIN_1);
		break;
	case 1:
		game_enemy_add1(COIN_1);
		break;
	case 2:
		if ( phase_curr > 8) game_enemy_add1(SLIPICE);
		break;
	case 4:
		if ( game_check_time( 0, game_time_fireball_start ) ) game_enemy_add1(FIREBALL_GREEN);
		break;
	case 5:
		if ( game_check_time( 0, game_time_fireball_start ) ) game_enemy_add1(FIREBALL_RED);
		break;
	};
	return 0;
}

unsigned char game_enemy_rnd(void) {
	if (spr_count < ENEMIES_MAX && game_check_time( entry_time , 100 ) ) {
		entry_time = zx_clock();
		//Agrego enemigo
		game_enemy_add1(rand() % 11 + 1); //TODO OPTIMIZE
	}
	return 0;
}


unsigned char game_enemy_add1(unsigned char f_class) {
	if	(
		(f_class == SHELLCREEPER_BLUE) ||
		(f_class == SIDESTEPPER_MAGENTA) ||
		(f_class == FIREBALL_RED   && game_enemy_add_get_index(FIREBALL_RED	 ) != 255 ) ||
		(f_class == FIREBALL_GREEN && game_enemy_add_get_index(FIREBALL_GREEN) != 255 ) ||
		(f_class == SLIPICE		   && game_enemy_add_get_index(SLIPICE		 ) != 255 &&
		 phase_curr < 8 )
	) {
		return 0;
	}
	//FORCE FOR TEST
	//f_class = SIDESTEPPER_MAGENTA;
	sound_enter_enemy();
	tmp = game_enemy_add_get_index(0);
	tmp0 = rand() & 0x1;

	if (tmp0 == 1) {
		s_lin0 = ENEMY_SLIN_L;
		s_col0 = ENEMY_SCOL_L;
		s_col1 = 2;
		if (f_class == FIREBALL_RED ) s_col1 = 3;
		s_dir0 = DIR_RIGHT;
	} else {
		s_lin0 = ENEMY_SLIN_R;
		s_col0 = ENEMY_SCOL_R;
		s_col1 = 30;
		if (f_class == FIREBALL_RED ) s_col1 = 28;
		s_dir0 = DIR_LEFT;
	}

	tmp_uc = 0;
	if (f_class == FIREBALL_GREEN || f_class == FIREBALL_RED) {
		tmp_uc = rand() % 3;
		if (tmp_uc == 0) {
			tmp_uc = 16;
		}
		if (tmp_uc == 1) {
			tmp_uc = 40;
		}
		if (tmp_uc == 2) {
			tmp_uc = 132;
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
	enemy_init( tmp, s_lin0 + tmp_uc , s_col0, f_class	, s_dir0);

	return 0;
}

unsigned char game_enemy_add_get_index( unsigned char f_search) {
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
			NIRVANAP_drawT( TILE_BRICK_FREEZE , f_lin , f_col );
			if (lvl_1[index1+1] == 0) NIRVANAP_fillT(PAPER,f_lin,f_col+1);
			game_freeze(f_lin, f_col + 1);
			game_freeze(f_lin, f_col - 1);
		}
	}
}

void game_unfreeze_all() {
	for (index1 = 192; index1 <= 512+32; index1++ ) {
		if (lvl_1[index1] == GAME_MAP_PLATFORM_FREEZE ) {
			lvl_1[index1] = GAME_MAP_PLATFORM;
		}
	}
}

void game_freeze_all() {
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


void game_paint_attrib(unsigned char e_r1) {
	//game_paint_attrib_lin(6,24,(14<<3)+8);
	//game_paint_attrib_lin(6,24,(16<<3)+8);
	//game_paint_attrib_lin(6,24,(18<<3)+8);
	for ( tmp0=e_r1; tmp0 < 19; ++tmp0){
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
	NIRVANAP_drawT(	 3, 128,  4 ); //MARIO
	NIRVANAP_drawT( 59, 128, 25 ); //TURTLE
	cont=1;
	while (cont) {
		while ((joyfunc1)(&k1) != 0);
		//MENU
		zx_print_str(14,10,"SOUND 48  ");
		zx_print_str(16,10,"GAME      ");
		zx_print_str(18,10,"BACK      ");
		
		if (game_sound & GAME_SOUND_48_FX_ON) {
			zx_print_str(14,20,"ON ");
		} else {
			zx_print_str(14,20,"OFF");
		};
		
		switch ( game_type ) {	//0->3
		case 0:
			zx_print_str(16,20,"A  ");
			break;
		case 1:
			zx_print_str(16,20,"B  ");
			break;
		case 2:
			zx_print_str(16,20,"RND");
			break;
		}
		
		game_menu_sel = game_menu_handle(10, 2, 14, 18, 0);
		switch ( game_menu_sel ) {
		case 0: //SOUND 48
			ay_fx_play(ay_effect_10);
			sound_coin();
			game_sound ^= GAME_SOUND_48_FX_ON;
			break;
		case 1: //GAME TYPE
			ay_fx_play(ay_effect_10);
			sound_coin();
			++game_type;
			if (game_type > 2) game_type = 0;
			break;
		case 2: //BACK
			game_menu_sel = 0;
			game_menu_paint();
			cont = 0;
			break;
		}
	}
}

void game_menu(void) {
#ifdef __SDCC
	/*PLAY MIDI TITLE*/
	ay_midi_play(pb_midi_title);
	game_menu_paint();
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
#endif
}


void game_menu_paint(void) {

	game_paint_attrib(11);
#ifdef __SDCC
	game_fill_row(0,32);
	//DRAW MENU
	//BLUE TOP
	
	game_menu_e(16, 0,30,156,1);
	//PIETRO LOGO
	tmp = 24;
	for (tmp_uc =0; tmp_uc < 10 ; ++tmp_uc) {
		NIRVANAP_drawT(192 +	   tmp_uc , tmp + 16 , 5 + (tmp_uc*2) );
		NIRVANAP_drawT(192 + 12	 + tmp_uc , tmp + 32 , 5 + (tmp_uc*2) );
		//NIRVANAP_drawT(180 + 24	 + tmp_uc , tmp + 48 , 5 + (tmp_uc*2) );
	}
	NIRVANAP_fillC(INK_RED | PAPER_RED , tmp + 40 , 26);//POINT
	//GREEN BOTTOM
	
	game_menu_e(80, 0,30,159,255);//game_menu_e(159,-1);
	NIRVANAP_drawT(	 3, 128,  4 ); //PIETRO
	NIRVANAP_drawT( 59, 128, 25 ); //TURTLE
#endif
	//MENU
	zx_print_str(14,10, "  1 PLAYER   ");
	zx_print_str(16,10, "  2 PLAYER   ");
	zx_print_str(18,10, "   CONFIG    ");
	zx_print_ink(INK_BLUE);
	zx_print_str(22,7, "CODED BY CGONZALEZ");
	zx_print_str(23,7, "   VERSION  1.2   ");
	tmp_uc = 0; //FIX MENU RETURN
}

void game_menu_e(unsigned char f_col,unsigned char e_c0,unsigned char e_c1,unsigned char e_start,unsigned char f_sign) {

	for (tmp_uc =e_c0; tmp_uc <= e_c1 ; tmp_uc = tmp_uc + 2) {
		if (tmp_uc == e_c0) {
			NIRVANAP_drawT(e_start , f_col, tmp_uc );
			continue;
		}
		if (tmp_uc == e_c1) {
			NIRVANAP_drawT(e_start + 2 , f_col, tmp_uc );
			continue;
		}
		if (tmp_uc == 14 || tmp_uc == 16) {
			continue;
		}
		NIRVANAP_drawT(e_start + 1 , f_col, tmp_uc );
	}
	if ( f_sign == 255 ) {
		f_col = f_col-16;
	} else {
		f_col = f_col+16;
	}
	NIRVANAP_drawT(e_start + 12 , f_col, e_c0 );
	NIRVANAP_drawT(e_start + 14 , f_col, e_c1 );

}

unsigned char game_menu_handle( unsigned char f_col, unsigned char f_inc, unsigned char f_start, unsigned char f_end, unsigned int timeout) {
	entry_time = zx_clock();
	s_lin1 = f_start + (game_menu_sel*f_inc);
	while ((dirs = (joyfunc1)(&k1)) != IN_STICK_FIRE) {
		if ( game_check_time(frame_time,5) ) {
			if (dirs != 0) {
				ay_fx_play(ay_effect_03);  // effect 19 now repeats
				sound_slide();
				game_paint_attrib_lin( f_col, 20 , (s_lin1*8)+8);
				s_lin1 += f_inc;
				if (s_lin1 > f_end	) {
					s_lin1 = f_start;
				}
			}
			//ROTATE ATTRIB ARRAY
			frame_time = zx_clock();
			game_paint_attrib_lin_h( f_col, 20 , (s_lin1*8)+8);
			game_rotate_attrib();
		}
		if (timeout > 0 && game_check_time(entry_time, timeout) && !ay_is_playing() ) {
			game_hall_of_fame();
			game_menu_sel = 0;
			s_lin1 = f_start;
			game_menu_paint();
			entry_time = zx_clock(); 
		}
		
	};
	return	(unsigned char) ( s_lin1 -	f_start	 ) / 2;
}

void game_end() {
	unsigned char f_p1, f_p2;
	game_kill_all_sprites();
	game_draw_clear();
	//game_cortina_pipes();
	ay_midi_play(pb_midi_title);

	//BACKGROUND
	
	for (tmp0 = 4 ; tmp0 < 28 ; tmp0 = tmp0 + 2) NIRVANAP_drawT(TILE_GRASS,  80, tmp0);
	for (tmp0 = 4 ; tmp0 < 28 ; tmp0 = tmp0 + 2) NIRVANAP_drawT(TILE_GRASS,  96, tmp0);
	for (tmp0 = 4 ; tmp0 < 28 ; tmp0 = tmp0 + 2) NIRVANAP_drawT(TILE_GRASS, 112, tmp0);
	NIRVANAP_drawT(TILE_CASTLE      , 80, 24);
	
	game_menu_e(  32, 1, 29, 156,   1);
	game_menu_e( 128, 1, 29, 159, 255);
	f_p1 = 0;
	f_p2 = 0;
	
	if (game_two_player) {
		if (game_lives[0] < 255) f_p1 = 1;
		if (game_lives[1] < 255) f_p2 = 1;
	} else {
		f_p1 = 1;
	}

	//PIETRO
	if ( f_p1 == 1 && f_p2 == 0 ) {
		zx_print_str(19, 1, "PIETRO...");
		NIRVANAP_drawT(TILE_P1_STANR    , 80, 12);
	}
	//LUIZO
	if ( f_p1 == 0 && f_p2 == 1 ) {
		zx_print_str(19, 1, "LUIZO...");
		NIRVANAP_drawT(TILE_P1_STANR+24 , 96, 14);
	}
	//PIETRO AND LUIZO
	if ( f_p1 == 1 && f_p2 == 1) {
		zx_print_str(19, 1, "PIETRO AND LUIZO...");
		NIRVANAP_drawT(TILE_P1_STANR    , 80, 12);
		NIRVANAP_drawT(TILE_P1_STANR+24 , 96, 14);
	}
	game_colour_message( 19, 1, 31, 200 );
	//PIETRO
	if ( f_p1 == 1 && f_p2 == 0 ) {
		NIRVANAP_drawT(TILE_EXTRA      , 64, 18);
		NIRVANAP_drawT(TILE_PRINCESS   , 80, 18);
		zx_print_str(19, 1, "FOUND THE PRINCESS!");
		NIRVANAP_drawT(TILE_EXTRA      , 64, 12);
	}
	//LUIZO
	if ( f_p1 == 0 && f_p2 == 1 ) {
		NIRVANAP_drawT(TILE_EXTRA      , 80, 18);
		NIRVANAP_drawT(TILE_PRINCESS   , 96, 18);
		zx_print_str(19, 1, "FOUND THE PRINCESS!");
		NIRVANAP_drawT(TILE_EXTRA      , 80, 14);
	}	
	//PIETRO AND LUIZO
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
	game_draw_clear();
	//game_cortina_pipes();
}

void game_hall_enter(void) {
#ifdef __SDCC
	unsigned char edit, f_col, f_row;
	unsigned char p1;
	unsigned char p2;
	unsigned char p1_lock;
	unsigned char p2_lock;
	unsigned char p1_sel;
	unsigned char p2_sel;
	unsigned char p1_cnt;
	unsigned char p2_cnt;
	
	//TODO ROTATE DOWN
   if (player_score[0] > player_score[1]) {
      p1 = game_hall_check(0);
      p2 = game_hall_check(1);
   }
   else
   {
      p2 = game_hall_check(1);
      p1 = game_hall_check(0);
   }
	
	p1_lock = 0;
	p2_lock = 0;
	
	hall_flip = 0;
	
	
	p1_cnt = 0;
	p2_cnt = 0;
	p1_sel = 0;
	p2_sel = 0;
	f_row = 12;
	f_col = 6;
	
	game_draw_clear();
	game_paint_attrib(0);
	
	
	if (p1 > 0) {
		game_hall_enter_phs(f_row,f_col);
	}
	if (p2 > 0) {
		game_hall_enter_phs(f_row,f_col+16);
	}
	
	if ( p1 > 0 || p2 > 0 ) {
		edit = 1;
	} else {
		edit = 0;
	}
	
	while (edit){
		if ( game_check_time(frame_time,5) ) {
			
			
			if (p1 > 0) {
				game_hall_print_p(p1_sel,f_row, f_col   , 3,0);
			}
			if (p2 > 0) {
				game_hall_print_p(p2_sel,f_row, f_col+16,39,6);
			}
			
			frame_time = zx_clock();
			game_rotate_attrib();
			hall_flip = !hall_flip;
		}
		dirs = 0;
		if (p1 > 0) {
			dirs = (joyfunc1)(&k1);
			if (!p1_lock && dirs) {
				game_hall_edit_p(&p1, &p1_sel,&p1_cnt,f_col,f_row,0);
				p1_lock = 1;
			} else {
				if (!dirs) p1_lock = 0;
			}
			
		}
		
		if (p2 > 0) {
			dirs = (joyfunc2)(&k1);
			if (!p2_lock && dirs) {
				game_hall_edit_p(&p2, &p2_sel,&p2_cnt,f_col+16,f_row,6);
				p2_lock = 1;
			} else {
				if (!dirs) p2_lock = 0;
			}
		}
		if (p1 == 0 && p2 == 0) {
			edit = 0;
			game_draw_clear();
		}
	}
#endif
}


unsigned char game_hall_check(unsigned char p_index) {
#ifdef __SDCC
	for (tmp=0;tmp<10;++tmp){
		if ( player_score[p_index] > hall_scores[tmp] ) {
				for (tmp0 = 9; tmp0 > tmp; --tmp0 ) {
					hall_scores[tmp0] = hall_scores[tmp0-1];
               strcpy(hall_names[tmp0], hall_names[tmp0-1]);
				}
				hall_scores[tmp] = player_score[p_index];
				return tmp+1;
		}
	}
#endif
	return 0;
}

void game_hall_enter_phs(unsigned char f_row,unsigned char f_col) {
#ifdef GAME_LOW_MEM
	game_menu_e( (f_row<<3)-48, f_col-4,f_col+6, 156,   1);
	game_menu_e( (f_row<<3)+28, f_col-4,f_col+6, 159, 255);
	zx_print_str(f_row-5, f_col-1, "HIGH");
	zx_print_str(f_row-4, f_col-1, " SCORE");
#endif
}

void game_hall_print_p( unsigned char selected, unsigned char f_row,unsigned char f_col, unsigned char f_tile, unsigned char f_inc) {
#ifdef GAME_LOW_MEM
	++f_row;
	zx_print_str( f_row, f_col  , &initals[f_inc    ] );
	zx_print_str( f_row, f_col+1, &initals[f_inc + 2] );
	zx_print_str( f_row, f_col+2, &initals[f_inc + 4] );
	tmp = f_col + selected;
	game_paint_attrib_lin_h( tmp , tmp + 1 , (f_row<<3)+8);

	f_row = (f_row<<3)-16;
	if (f_inc > 0) f_col = f_col + 1;
	if (hall_flip) {
		NIRVANAP_drawT( TILE_EMPTY    , f_row-2, f_col );
		NIRVANAP_drawT( f_tile        , f_row  , f_col );
	} else {
		NIRVANAP_drawT( TILE_EMPTY    , f_row , f_col );
		NIRVANAP_drawT( 22+(f_inc<<2) , f_row-2 , f_col );
	}
#endif
}

void game_hall_edit_p(unsigned char *player, unsigned char *selected, unsigned char *cnt, unsigned char f_col, unsigned char f_row, unsigned char f_inc) {
#ifdef GAME_LOW_MEM
	unsigned char *p;
	unsigned char *out_str;
	unsigned char f_tmp;
	
	out_str = &f_tmp;

	if ( dirs & IN_STICK_FIRE ) {
		++f_row;
		game_paint_attrib_lin( f_col + *selected, f_col + 1 + *selected, (f_row<<3)+8);
		sound_slide();
		if (*cnt != 39) {
			++*selected;
		} else {
			if (*selected > 0) {
				initals[f_inc + ( *selected<<1 )] = 0x41;
				--*selected;
			}
		}
		
		if (*selected == 3) {
			tmp = *player - 1;
			p = hall_names[tmp];
			*p++ = initals[f_inc];
			*p++ = initals[f_inc+2];
			*p = initals[f_inc+4];
			*player = 0;
		}
		*cnt = 0;
		while( hall_valids[*cnt] != initals[f_inc + ( *selected<<1 )]) ++*cnt;
	}

	if ( dirs & IN_STICK_RIGHT ) {
		++*cnt;
		if (*cnt > 39) *cnt = 0;
	}

	if ( dirs & IN_STICK_LEFT ) {
		--*cnt;
		if (*cnt > 39) *cnt = 39;
	}

	out_str = hall_valids + *cnt;
	initals[f_inc + ( *selected<<1 )] = *out_str;
#endif
}

void game_hall_of_fame(void) {
#ifdef __SDCC
	//OUT OF MEMORY

	game_draw_clear();
	game_paint_attrib(0);
	
	game_menu_e(16 ,6, 24,156,1);
	
	game_menu_e(GAME_LIN_FLOOR,6, 24,159,255);//game_menu_e(159,-1);
	//MENU
	for(s_lin1=0; s_lin1 < 10; ++s_lin1){
		zx_print_ink(INK_CYAN);
		zx_print_str( 7 + s_lin1, 10, hall_names[s_lin1] );
		zx_print_str( 7 + s_lin1, 14, "-");
		zx_print_int( 7 + s_lin1, 16, hall_scores[s_lin1] );
		zx_print_str( 7 + s_lin1, 21, "0");
	}
	zx_print_str( 4, 10, "HALL OF FAME");
	game_colour_message( 4, 10,22, 500 );
	game_draw_clear();
#endif
	game_menu_sel = 0;
	ay_midi_play(pb_midi_title);
}

void game_rotate_attrib(void) {
#ifdef __SDCC
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
#endif
}

void game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_microsecs  ) {
	tmp = 1;
	frame_time = zx_clock();
	entry_time = zx_clock();
	while ( tmp && !game_check_time( entry_time, f_microsecs)) {
		if ( game_check_time(frame_time,5) ) {
			//ROTATE ATTRIB ARRAY
			frame_time = zx_clock();
			game_paint_attrib_lin_h( f_col, f_col2 , (f_row<<3)+8);
			game_rotate_attrib();
		}
		while ((joyfunc1)(&k1) != 0) {
			tmp = 0;
		}
	};
	
}

unsigned char game_check_time( unsigned int start, unsigned int lapse) {
	if (zx_clock() - start	> lapse) {
		return 1;
	} else {
		return 0;
	}
}

