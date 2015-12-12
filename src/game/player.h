#ifndef PLAYER_H_
#define PLAYER_H_

#include <camera.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_fvec speed;
	whitgl_float angle;
	whitgl_float angle_speed;
	whitgl_float engine_thrust[2];
} space_player;
static const space_player space_player_zero = {{0,0},{0,0},0, 0, {0,0}};

space_player space_player_update(space_player player);
void space_player_draw(space_player player, space_camera camera);

#endif // PLAYER_H_
