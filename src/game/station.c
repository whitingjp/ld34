#include "station.h"

#include <sprite.h>

space_station space_station_update(space_station s)
{
	s.angle = whitgl_fwrap(s.angle += 0.01, 0, whitgl_pi*2);
	return s;
}
void space_station_draw(space_station s, space_camera camera)
{
	space_sprite sprite;
	whitgl_float num_points = 11;
	whitgl_int i;
	for(i=0; i<num_points; i++)
	{
		whitgl_fvec point = {0.5, -2};
		sprite.points[i] = whitgl_rotate_point_around_point(point, whitgl_fvec_val(0.5), (whitgl_pi*2)*(i/num_points));
	}
	for(i=0; i<num_points-2; i++)
	{
		whitgl_ivec line = {i, i+1};
		sprite.lines[i] = line;
	}
	sprite.num_points = num_points;
	sprite.num_lines = num_points-2;
	space_sprite_draw(sprite, s.pos, s.angle, camera);
}
