#ifndef SPACE_ASTEROID_H_
#define SPACE_ASTEROID_H_

#include <camera.h>
#include <sprite.h>

typedef struct
{
	space_entity e;
	whitgl_fvec speed;
	whitgl_float angle_speed;
} space_asteroid;

space_asteroid space_asteroid_zero();
space_asteroid space_asteroid_update(space_asteroid a);
void space_asteroid_draw(space_asteroid a, space_camera camera);

#endif // SPACE_ASTEROID_H_
