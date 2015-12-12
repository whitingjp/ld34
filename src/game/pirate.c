#include "pirate.h"

#include <whitgl/sound.h>
#include <whitgl/input.h>
#include <resource.h>

space_pirate space_pirate_update(space_pirate p, whitgl_fvec target)
{
	if(!p.active)
	{
		return p;
	}
	whitgl_fvec diff = whitgl_fvec_sub(target, p.e.pos);
	whitgl_float target_angle = whitgl_fvec_to_angle(diff)+whitgl_pi/2;
	whitgl_float ang_diff = whitgl_fwrap(target_angle-p.e.angle, -whitgl_pi, whitgl_pi);
	whitgl_bool r, l;
	if(ang_diff > 0.2)
	{
		r = true;
		l = false;
	}
	else if(ang_diff < -0.2)
	{
		r = false;
		l = true;
	}
	else
	{
		l = true;
		r = true;
	}
	if(l&&r)
	{
		whitgl_fvec impulse = {0,-0.002};
		impulse = whitgl_rotate_point_around_point(impulse, whitgl_fvec_zero, p.e.angle);
		p.speed = whitgl_fvec_add(p.speed, impulse);
	}
	if(l)
		p.angle_speed = whitgl_fclamp(p.angle_speed-0.01, -0.2, 0.2);
	if(r)
		p.angle_speed = whitgl_fclamp(p.angle_speed+0.01, -0.2, 0.2);

	p.angle_speed = p.angle_speed*0.9;
	p.e.angle = whitgl_fwrap(p.e.angle+p.angle_speed, 0, whitgl_pi*2);
	p.e.pos = whitgl_fvec_add(p.e.pos, p.speed);
	p.speed = whitgl_fvec_interpolate(p.speed, whitgl_fvec_zero, 0.01);
	whitgl_int i;
	for(i=0; i<2; i++)
	{
		whitgl_bool on = i == 0 ? r : l;
		// if(on && !p.was_on[i])
		// 	whitgl_sound_play(i == 0 ? SOUND_THRUST_L : SOUND_THRUST_R, 1.0);
		// p.was_on[i] = on;
		whitgl_float target = on ? 1 : 0;
		p.engine_thrust[i] = p.engine_thrust[i]*0.6 + 0.4*target;
	}

	return p;
}
void space_pirate_draw(space_pirate p, space_camera camera)
{
	if(!p.active)
		return;

	const space_sprite ship_lengine =
	{
		{{-0.2,0.45},{-0.2,0.45}},
		{{0,1}},
		2,
		1,
		{0xf6,0x88,0x1c,0xff},
	};

	const space_sprite ship_rengine =
	{
		{{0.2,0.45},{0.2,0.45}},
		{{0,1}},
		2,
		1,
		{0xf6,0x88,0x1c,0xff},
	};

	space_entity_draw(p.e, camera);
	space_sprite lengine = ship_lengine;
	lengine.points[1].y += 0.5*p.engine_thrust[0];
	space_sprite_draw(lengine, p.e.pos, p.e.angle, camera);
	space_sprite rengine = ship_rengine;
	rengine.points[1].y += 0.5*p.engine_thrust[1];
	space_sprite_draw(rengine, p.e.pos, p.e.angle, camera);
}
