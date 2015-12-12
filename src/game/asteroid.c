#include "asteroid.h"

space_asteroid space_asteroid_zero()
{
	space_asteroid a;

	whitgl_int num_points = whitgl_randint(12)+6;
	space_sprite sprite;
	whitgl_int i;
	whitgl_float scale = whitgl_randfloat()*1+0.5;
	for(i=0; i<num_points; i++)
	{
		whitgl_fvec point = {0, scale*(whitgl_randfloat()+0.8)};
		sprite.points[i] = whitgl_rotate_point_around_point(point, whitgl_fvec_zero, (whitgl_pi*2)*((float)i/num_points));
	}
	for(i=0; i<num_points; i++)
	{
		whitgl_ivec line = {i, (i+1)%num_points};
		sprite.lines[i] = line;
	}
	sprite.num_points = num_points;
	sprite.num_lines = num_points;
	whitgl_sys_color col = {0x9f,0xfd,0x3b,0xff};
	sprite.col = col;
	a.e.sprite = sprite;
	a.e.pos.x = (whitgl_randfloat()-0.5)*200;
	a.e.pos.y = (whitgl_randfloat()-0.5)*200;
	a.e.angle = 0;
	a.angle_speed = (whitgl_randfloat()-0.5)/12;
	a.speed.x = (whitgl_randfloat()-0.5)/12.0;
	a.speed.y = (whitgl_randfloat()-0.5)/12.0;
	return a;
}
space_asteroid space_asteroid_update(space_asteroid a, whitgl_fvec player_pos)
{
	a.e.angle = whitgl_fwrap(a.e.angle + a.angle_speed, 0, whitgl_pi*2);
	a.e.pos = whitgl_fvec_add(a.e.pos, a.speed);
	whitgl_int bound = 100;
	a.e.pos.x = whitgl_fwrap(a.e.pos.x, player_pos.x - bound, player_pos.x + bound);
	a.e.pos.y = whitgl_fwrap(a.e.pos.y, player_pos.y - bound, player_pos.y + bound);
	return a;
}
void space_asteroid_draw(space_asteroid a, space_camera camera)
{
	space_entity_draw(a.e, camera);
}
