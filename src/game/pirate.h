#ifndef PIRATE_H_
#define PIRATE_H_

#include <camera.h>
#include <sprite.h>

typedef struct
{
	space_entity e;
	whitgl_fvec speed;
	whitgl_float angle_speed;
	whitgl_float was_on[2];
	whitgl_float engine_thrust[2];
} space_pirate;
static const space_pirate space_pirate_zero =
{
	{
		false,
		{
			{{0.0,-0.5},{0.5,0.5},{-0.5,0.5},{0.0,0.3}},
			{{0,1},{1,2},{0,2},{1,3},{2,3},{0,3}},
			4,
			6,
			{0xf6,0x88,0x1c,0xff},
		},
		{0,-4},0,false,
	}, {0,0}, 0, {false,false}, {0,0}
};

space_pirate space_pirate_update(space_pirate pirate, whitgl_fvec target);
void space_pirate_draw(space_pirate pirate, space_camera camera);

#endif // PIRATE_H_
