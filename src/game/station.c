#include "station.h"

#include <whitgl/sys.h>

space_station space_station_zero(const char* name, whitgl_int num_points, whitgl_float x, whitgl_float y, mission_index mission_id)
{
	space_station s;
	s.name = name;
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
	whitgl_sys_color col = {0x9f,0xfd,0x3b,0xff};
	sprite.col = col;
	s.e = space_entity_zero;
	s.e.active = true;
	s.e.sprite = sprite;
	s.e.pos.x = x;
	s.e.pos.y = y;
	s.e.angle = whitgl_randfloat()*whitgl_pi*2;
	s.angle_speed = 0.01;
	s.mission_id = mission_id;
	s.known = false;
	return s;
}

space_station space_station_update(space_station s)
{
	if(!s.e.active)
		return s;
	whitgl_float target_angle_speed = s.defensive ? 0.2 : 0.01;
	s.angle_speed = s.angle_speed*0.98 + target_angle_speed*0.02;
	s.e.angle = whitgl_fwrap(s.e.angle + s.angle_speed, 0, whitgl_pi*2);
	return s;
}
void space_station_draw(space_station s, space_camera camera)
{
	if(!s.e.active)
		return;
	space_entity_draw(s.e, camera);
	whitgl_fcircle circle;
	circle.pos = space_camera_point(s.e.pos, camera);
	circle.size = camera.scale;
	whitgl_sys_color col = {0x0f, 0x52, 0x3a, 0x60};
	whitgl_sys_draw_fcircle(circle, col, s.e.sprite.num_points);
}
