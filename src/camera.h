#ifndef CAMERA_H_
#define CAMERA_H_

#include <whitgl/math.h>

typedef struct
{
	whitgl_fvec pos;
	whitgl_float scale;
	whitgl_fvec screen_size;
} space_camera;

whitgl_fvec space_camera_point(whitgl_fvec p, space_camera cam);

#endif //CAMERA_H_
