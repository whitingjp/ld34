#ifndef SPACE_STATION_H_
#define SPACE_STATION_H_

#include <camera.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_float angle;
} space_station;
static const space_station space_station_zero = {{2,0},0};

space_station space_station_update(space_station s);
void space_station_draw(space_station s, space_camera camera);

#endif // SPACE_STATION_H_
