#ifndef CAMERA_H_
#define CAMERA_H_

#include <whitgl/math.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_float scale;
	whitgl_fvec screen_size;
	whitgl_fvec speed;
} space_camera;

#define MAX_FOCI (64)
typedef struct
{
	whitgl_faabb foci[MAX_FOCI];
	whitgl_int num_foci;
} space_camera_focus;

whitgl_fvec space_camera_point(whitgl_fvec p, space_camera cam);
space_camera space_camera_update(space_camera c, space_camera_focus focus, whitgl_ivec screen_size);


#endif //CAMERA_H_
