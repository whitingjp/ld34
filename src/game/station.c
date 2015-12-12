#include "station.h"

#include <whitgl/sys.h>

space_station space_station_zero(whitgl_int num_points, whitgl_fvec pos)
{
	space_station s;
	space_sprite sprite;
	whitgl_int i;
	for(i=0; i<num_points; i++)
	{
		whitgl_fvec point = {0, -2};
		sprite.points[i] = whitgl_rotate_point_around_point(point, whitgl_fvec_zero, (whitgl_pi*2)*((float)i/num_points));
	}
	for(i=0; i<num_points-2; i++)
	{
		whitgl_ivec line = {i, i+1};
		sprite.lines[i] = line;
	}
	sprite.num_points = num_points;
	sprite.num_lines = num_points-2;
	s.e.sprite = sprite;
	s.e.pos = pos;
	s.e.angle = whitgl_randfloat()*whitgl_pi*2;
	return s;
}

space_station space_station_update(space_station s)
{
	s.e.angle = whitgl_fwrap(s.e.angle += 0.01, 0, whitgl_pi*2);
	return s;
}
void space_station_draw(space_station s, space_camera camera)
{
	space_entity_draw(s.e, camera);
	whitgl_fcircle circle;
	circle.pos = space_camera_point(s.e.pos, camera);
	circle.size = camera.scale;
	whitgl_sys_color col = {0x0f, 0x52, 0x3a, 0x60};
	whitgl_sys_draw_fcircle(circle, col, s.e.sprite.num_points);
}
