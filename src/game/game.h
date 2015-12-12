#ifndef GAME_H_
#define GAME_H_

#include <game/asteroid.h>
#include <game/debris.h>
#include <game/hud.h>
#include <game/player.h>
#include <game/station.h>
#include <game/starfield.h>
#include <camera.h>

#define NUM_STATIONS (3)
#define NUM_ASTEROIDS (32)

typedef struct
{
	space_player player;
	space_station stations[NUM_STATIONS];
	space_starfield starfield;
	space_asteroid asteroids[NUM_ASTEROIDS];
	space_camera camera;
	space_debris debris;
	space_hud_markers hud;
	whitgl_int docked;
} space_game;

space_game space_game_zero(whitgl_ivec screen_size);
space_game space_game_update(space_game game, whitgl_ivec screen_size);
void space_game_draw(space_game game);

#endif // GAME_H_
