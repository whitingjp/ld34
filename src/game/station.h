#ifndef SPACE_STATION_H_
#define SPACE_STATION_H_

#include <camera.h>
#include <sprite.h>
#include <mission.h>

typedef struct
{
	const char* name;
	space_entity e;
	whitgl_float angle_speed;
	whitgl_bool defensive;
	mission_index mission_id;
	whitgl_bool known;
} space_station;

space_station space_station_zero(const char* name, whitgl_int num_points, whitgl_float x, whitgl_float y, mission_index mission_id);
space_station space_station_update(space_station s);
void space_station_draw(space_station s, space_camera camera);

#endif // SPACE_STATION_H_
