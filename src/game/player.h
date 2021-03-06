#ifndef PLAYER_H_
#define PLAYER_H_

#include <camera.h>
#include <sprite.h>

#include <mission.h>

typedef struct
{
	space_entity e;
	whitgl_fvec speed;
	whitgl_float angle_speed;
	whitgl_float was_on[2];
	whitgl_float engine_thrust[2];
	whitgl_int docked;
	mission_trade hold;
	whitgl_float launch_timer;
	whitgl_float was_in_menu;
	whitgl_float dead_timer;
} space_player;
static const space_player space_player_zero =
{
	{
		true,
		{
			{{0.0,-0.5},{0.5,0.5},{-0.5,0.5},{0.0,0.3}},
			{{0,1},{1,2},{0,2},{1,3},{2,3},{0,3}},
			4,
			6,
			{0x9f,0xfd,0x3b,0xff},
		},
		{0,0},0,false,
	}, {0,0}, 0, {false,false}, {0,0}, -1,
	{GOOD_NONE, 5},
	0,
	false,
	0,
};

space_player space_player_update(space_player player, whitgl_bool can_thrust);
void space_player_draw(space_player player, space_camera camera);

#endif // PLAYER_H_
