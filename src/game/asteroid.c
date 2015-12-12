#include "asteroid.h"

space_asteroid space_asteroid_zero()
{
	space_asteroid a;

	whitgl_int num_points = whitgl_randint(12)+6;
	space_sprite sprite;
	whitgl_int i;
	whitgl_float scale = whitgl_randfloat()*3+1;
	for(i=0; i<num_points; i++)
	{
		whitgl_fvec point = {0, scale*(whitgl_randfloat()*1+0.4)};
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
	a.e.pos.x = 4;
	a.e.pos.y = 0;
	a.e.angle = 0;
	a.angle_speed = (whitgl_randfloat()-0.5)/12;
	a.speed = whitgl_fvec_zero;
	return a;
}
space_asteroid space_asteroid_update(space_asteroid a)
{
	a.e.angle = whitgl_fwrap(a.e.angle + a.angle_speed, 0, whitgl_pi*2);
	return a;
}
void space_asteroid_draw(space_asteroid a, space_camera camera)
{
	space_entity_draw(a.e, camera);
}
