#include "player.h"

#include <whitgl/sound.h>
#include <whitgl/input.h>
#include <sprite.h>
#include <resource.h>

static const space_sprite ship_sprite =
{
	{{0.5,0},{1.0,1.0},{0.0,1.0},{0.5,0.8}},
	{{0,1},{1,2},{0,2},{1,3},{2,3},{0,3}},
	4,
	6,
};

static const space_sprite ship_lengine =
{
	{{0.3,0.95},{0.3,0.95}},
	{{0,1}},
	2,
	1,
};

static const space_sprite ship_rengine =
{
	{{0.7,0.95},{0.7,0.95}},
	{{0,1}},
	2,
	1,
};

space_player space_player_update(space_player p)
{
	whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
	whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
	if(l&&r)
	{
		whitgl_fvec impulse = {0,-0.002};
		impulse = whitgl_rotate_point_around_point(impulse, whitgl_fvec_zero, p.angle);
		p.speed = whitgl_fvec_add(p.speed, impulse);
	}
	if(l)
		p.angle_speed = whitgl_fclamp(p.angle_speed-0.01, -0.2, 0.2);
	if(r)
		p.angle_speed = whitgl_fclamp(p.angle_speed+0.01, -0.2, 0.2);
	p.angle_speed = p.angle_speed*0.9;
	p.angle = whitgl_fwrap(p.angle+p.angle_speed, 0, whitgl_pi*2);
	p.pos = whitgl_fvec_add(p.pos, p.speed);
	p.speed = whitgl_fvec_interpolate(p.speed, whitgl_fvec_zero, 0.01);
	whitgl_int i;
	for(i=0; i<2; i++)
	{
		whitgl_bool on = i == 0 ? r : l;
		if(on && !p.was_on[i])
			whitgl_sound_play(i == 0 ? SOUND_THRUST_L : SOUND_THRUST_R, 1.0);
		p.was_on[i] = on;
		whitgl_float target = on ? 1 : 0;
		p.engine_thrust[i] = p.engine_thrust[i]*0.6 + 0.4*target;
	}

	whitgl_loop_frequency(SOUND_POWER_L, p.engine_thrust[0]);
	whitgl_loop_frequency(SOUND_POWER_R, p.engine_thrust[1]);

	whitgl_loop_volume(SOUND_POWER_L, p.engine_thrust[0]*0.5);
	whitgl_loop_volume(SOUND_POWER_R, p.engine_thrust[1]*0.5);

	return p;
}
void space_player_draw(space_player p, space_camera camera)
{
	space_sprite_draw(ship_sprite, p.pos, p.angle, camera);
	space_sprite lengine = ship_lengine;
	lengine.points[1].y += 0.5*p.engine_thrust[0];
	space_sprite_draw(lengine, p.pos, p.angle, camera);
	space_sprite rengine = ship_rengine;
	rengine.points[1].y += 0.5*p.engine_thrust[1];
	space_sprite_draw(rengine, p.pos, p.angle, camera);
}
