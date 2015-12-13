#ifndef MENU_H_
#define MENU_H_

#include <whitgl/math.h>
#include <game/game.h>
#include <mission.h>

typedef enum
{
	STATE_QUESTION,
	STATE_ACCEPTED,
	STATE_REJECTED,
} space_menu_state;

typedef struct
{
	whitgl_float transition;
	whitgl_int num_chars;
	whitgl_float buttons[3];
	mission_index mission_id;
	whitgl_bool have_required;
	whitgl_bool has_choice;
	whitgl_bool can_launch;
	space_menu_state state;

} space_menu;
static const space_menu space_menu_zero = {0, 0,{0,0,0},NUM_MISSIONS,false,false,false, STATE_QUESTION};

space_menu space_menu_update(space_menu menu, space_game game, space_station* station, space_player* player);
void space_menu_draw(space_menu menu, whitgl_ivec screen_size);

#endif // MENU_H_
