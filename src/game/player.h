#ifndef PLAYER_H_
#define PLAYER_H_

#include <camera.h>
#include <sprite.h>

typedef struct
{
	bool active;
	space_entity e;
	whitgl_fvec speed;
	whitgl_float angle_speed;
	whitgl_float was_on[2];
	whitgl_float engine_thrust[2];
	whitgl_int docked;
} space_player;
static const space_player space_player_zero =
{
	true,
	{
		{
			{{0.0,-0.5},{0.5,0.5},{-0.5,0.5},{0.0,0.3}},
			{{0,1},{1,2},{0,2},{1,3},{2,3},{0,3}},
			4,
			6,
			{0x9f,0xfd,0x3b,0xff},
		},
		{0,0},0,false,
	}, {0,0}, 0, {false,false}, {0,0}, -1,
};

space_player space_player_update(space_player player);
void space_player_draw(space_player player, space_camera camera);

#endif // PLAYER_H_
