/* ----------------------------------------------------------------
/* ----------------------------------------------------------------
 * Pietro - Cristian Gonzalez - cmgonzalez@gmail.com
 *
 * This program can be compiled in Windows (with z88dk installed) as follows:
 * (if necessary set up environment variables in the batch file)
 *
 * zcompile.bat
 *
 */


/*###############################################################################################
  #																								#
  # NOTES																						#
  #-SCORE TABLE--------------------------------------											#
  #| Flipping an enemy | 10 |																	#
  #| Kicking an enemy off the floor | 800 |														#
  #| Hitting Slipice | 500 |																	#
  #| Hitting a Red Fireball | 1000 |															#
  #| Hitting a Green Fireball | 200 |															#
  #| Bonus Coin | 800 |																			#
  #| Collecting all Bonus Coins (1st phase) | 3000 |											#
  # Collecting all Bonus Coins (2nd phase+) | 5000 |											#
  # =================================================											#
  #| Extra Life - 20000 points |																#
  # -------------------------------------------------											#
  #																								#
  # A-Game - When enemy gets knocked on its back, it takes 20 seconds for						#
  # it to change color and get back up															#
  # B-Game - When enemy gets knocked on its back, it takes 15 seconds for						#
  # it to change color and get back up															#
  # A-Game - Fireballs start to appear 80 seconds after the level begins						#
  # B-Game - Fireballs start to appear 60 seconds after the level begins						#
  #																								#
  ###############################################################################################*/

/*###############################################################################################
  #																								#
  # MAIN																						#
  #																								#
  ###############################################################################################*/

#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>
#include "nirvana+.h"
#include "pietro.h"
#include "pietro_ay.h"
#include "pietro_game.h"
#include "pietro_zx.h"
#include "macros.h"

int main(void) {
	unsigned int counter;
	
	//INTERRUPTS ARE DISABLED
	
	//RESET AY CHIP
	
	ay_reset();
	
	//ATTRIB NORMAL
	
	attrib[0]= BRIGHT | PAPER_BLACK | INK_WHITE;
	attrib[1]= BRIGHT | PAPER_BLACK | INK_YELLOW;
	attrib[2]= PAPER_BLACK | INK_YELLOW;
	attrib[3]= PAPER_BLACK | INK_WHITE;
	
	//ATTRIB HIGHLIGHT
	
	attrib_hl[0]= PAPER_BLACK | INK_RED;
	attrib_hl[1]= PAPER_BLACK | INK_YELLOW;
	attrib_hl[2]= PAPER_BLACK | INK_GREEN;
	attrib_hl[3]= PAPER_BLACK | INK_CYAN;
	
	//GAME OPTIONS
	
	//ENABLE SOUND BASED ON DETECTED MODEL
	
	game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON) : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);
	
	//GAME TYPE A
	
	game_type = 0;
	
	//Keyboard Handling P1
	
	k1.fire	 = in_key_scancode('0');
	k1.left	 = in_key_scancode('6');
	k1.right = in_key_scancode('7');

#ifdef __LLVM
	joyfunc1 = (uint16_t (*)(udk_t *))(in_stick_sinclair1);
#endif

#ifdef __SDCC
	joyfunc1 = (uint16_t (*)(udk_t *))(in_stick_sinclair1);
#endif

#ifdef __SCCZ80
	joyfunc1 = in_stick_sinclair1;
#endif

	//Keyboard Handling P2

	k2.fire	 = in_key_scancode('5');
	k2.left	 = in_key_scancode('1');
	k2.right = in_key_scancode('2');

#ifdef __LLVM
	joyfunc2 = (uint16_t (*)(udk_t *))(in_stick_sinclair2);
#endif

#ifdef __SDCC
	joyfunc2 = (uint16_t (*)(udk_t *))(in_stick_sinclair2);
#endif

#ifdef __SCCZ80
	joyfunc2 = in_stick_sinclair2;
#endif

	//Delay (NO NIRVANA)

	zx_border(INK_BLACK);
	
	//Wait for Keypress and Randomize

	in_wait_nokey();
	for (counter = 0x1234; !in_test_key(); ++counter) ;
	srand(counter);

	//Init	SCREEN

	game_cortina_brick();

	//INIT GAME

	game_start_timer();

	//INIT NIRVANA

	NIRVANAP_tiles(btiles);
	NIRVANAP_start();
	
	frame_time = zx_clock();

	//GAME MENU
	//game_hall_enter();

	game_menu();

	//GAME END

	NIRVANAP_stop();
	return 0;
}
