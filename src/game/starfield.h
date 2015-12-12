#ifndef GAME_STARFIELD_H_
#define GAME_STARFIELD_H_

#include <camera.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_float depth;
} space_star;

#define NUM_STARS (256)
typedef struct
{
	space_star stars[NUM_STARS];
} space_starfield;

space_starfield space_starfield_zero();
space_starfield space_starfield_update(space_starfield starfield, whitgl_fvec speed, space_camera camera);
void space_starfield_draw(space_starfield starfield, space_camera camera);

#endif // GAME_STARFIELD_H_
