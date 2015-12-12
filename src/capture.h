#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <whitgl/math.h>

typedef struct
{
	whitgl_bool capturing;
	whitgl_int timer;
	whitgl_int frame;
	whitgl_int n;
} capture_info;
static const capture_info capture_info_zero = {false, 0, 0, -1};

capture_info capture_info_update(capture_info capture);

#endif // CAPTURE_H_
