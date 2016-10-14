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

#endif
