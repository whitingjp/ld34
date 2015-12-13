#include "player.h"

#include <whitgl/sound.h>
#include <whitgl/input.h>
#include <resource.h>

space_player space_player_update(space_player p, whitgl_bool can_thrust)
{
	if(can_thrust && p.was_in_menu)
		p.launch_timer = 1;
	p.was_in_menu = !can_thrust;
	if(!p.e.active)
	{
		whitgl_loop_volume(SOUND_POWER_L, 0);
		whitgl_loop_volume(SOUND_POWER_R, 0);
		p.dead_timer = whitgl_fclamp(p.dead_timer + 0.01, 0, 1);
		return p;
	}
	whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
	whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
	p.launch_timer = whitgl_fclamp(p.launch_timer-0.02, 0, 1);
	if(p.launch_timer > 0)
	{
		l = true;
		r = true;
	}
	if(!can_thrust)
	{
		l = false;
		r = false;
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
	if(!p.e.active)
		return;

	const space_sprite ship_lengine =
	{
		{{-0.2,0.45},{-0.2,0.45}},
		{{0,1}},
		2,
		1,
		{0x9f,0xfd,0x3b,0xff},
	};

	const space_sprite ship_rengine =
	{
		{{0.2,0.45},{0.2,0.45}},
		{{0,1}},
		2,
		1,
		{0x9f,0xfd,0x3b,0xff},
	};

	space_entity_draw(p.e, camera);
	space_sprite lengine = ship_lengine;
	lengine.points[1].y += 0.5*p.engine_thrust[0];
	space_sprite_draw(lengine, p.e.pos, p.e.angle, camera);
	space_sprite rengine = ship_rengine;
	rengine.points[1].y += 0.5*p.engine_thrust[1];
	space_sprite_draw(rengine, p.e.pos, p.e.angle, camera);
}
