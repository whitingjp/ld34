#ifndef MENU_H_
#define MENU_H_

#include <whitgl/math.h>
#include <game/game.h>

typedef struct
{
	whitgl_float transition;
} space_menu;
static const space_menu space_menu_zero = {0};

space_menu space_menu_update(space_menu menu, space_game game);
void space_menu_draw(space_menu menu, whitgl_ivec screen_size);

#endif // MENU_H_
