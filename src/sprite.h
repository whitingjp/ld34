#ifndef SPRITE_H_
#define SPRITE_H_

#include <whitgl/math.h>
#include <camera.h>

#define MAX_POINTS (32)
#define MAX_LINES (32)
typedef struct
{
	whitgl_fvec points[MAX_POINTS];
	whitgl_ivec lines[MAX_LINES];
	whitgl_int num_points;
	whitgl_int num_lines;
} space_sprite;

typedef struct
{
	space_sprite sprite;
	whitgl_fvec pos;
	whitgl_float angle;
} space_entity;

void space_sprite_draw(space_sprite sprite, whitgl_fvec position, whitgl_float angle, space_camera camera);
void space_entity_draw(space_entity e, space_camera camera);
whitgl_bool space_entity_colliding(space_entity a, space_entity b);

#endif // SPRITE_H_
