#ifndef GAME_H_
#define GAME_H_

#include <game/player.h>
#include <game/station.h>
#include <game/starfield.h>
#include <camera.h>

typedef struct
{
	space_player player;
	space_station station;
	space_starfield starfield;
	space_camera camera;
	bool docked;
} space_game;

space_game space_game_zero(whitgl_ivec screen_size);
space_game space_game_update(space_game game, whitgl_ivec screen_size);
void space_game_draw(space_game game);

#endif // GAME_H_
