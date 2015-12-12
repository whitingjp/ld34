#ifndef SPACE_STATION_H_
#define SPACE_STATION_H_

#include <camera.h>
#include <sprite.h>

typedef struct
{
	space_sprite sprite;
	whitgl_fvec pos;
	whitgl_float angle;
} space_station;

space_station space_station_zero();
space_station space_station_update(space_station s);
void space_station_draw(space_station s, space_camera camera);

#endif // SPACE_STATION_H_
