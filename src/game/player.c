#include "player.h"

#include <whitgl/input.h>
#include <sprite.h>

static const space_sprite ship_sprite =
{
	{{0.5,0},{1.0,1.0},{0.0,1.0},{0.5,0.8}},
	{{0,1},{1,2},{0,2},{1,3},{2,3},{0,3}},
	4,
	6,
};

space_player space_player_update(space_player p)
{
	whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
	whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
	if(l&&r)
	{
		whitgl_fvec impulse = {0,-0.005};
		impulse = whitgl_rotate_point_around_point(impulse, whitgl_fvec_zero, p.angle);
		p.speed = whitgl_fvec_add(p.speed, impulse);
	}
	if(l)
		p.angle = whitgl_fwrap(p.angle-0.1, 0, whitgl_pi*2);
	if(r)
		p.angle = whitgl_fwrap(p.angle+0.1, 0, whitgl_pi*2);
	p.pos = whitgl_fvec_add(p.pos, p.speed);
	p.speed = whitgl_fvec_interpolate(p.speed, whitgl_fvec_zero, 0.1);
	return p;
}
void space_player_draw(space_player p, space_camera camera)
{
	space_sprite_draw(ship_sprite, p.pos, p.angle, camera);
}
