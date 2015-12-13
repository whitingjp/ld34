#ifndef DEBRIS_H_
#define DEBRIS_H_

#include <camera.h>
#include <sprite.h>

typedef struct
{
	space_entity e;
	whitgl_float timer;
	whitgl_float angle_speed;
	whitgl_fvec speed;
} space_debris_piece;
static const space_debris_piece space_debris_piece_zero = {{false,{},{},0,false}, 0, 0, {0,0}};
#define MAX_PIECES (64)
typedef struct
{
	space_debris_piece pieces[MAX_PIECES];
	whitgl_int next;
} space_debris;

space_debris space_debris_zero();
space_debris space_debris_create(space_debris d, space_entity e, whitgl_fvec speed);
space_debris space_debris_update(space_debris d);
void space_debris_draw(space_debris d, space_camera camera);

#endif // DEBRIS_H_
