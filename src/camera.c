#include <camera.h>

whitgl_fvec space_camera_point(whitgl_fvec p, space_camera cam)
{
	whitgl_fvec half_screen_size = whitgl_fvec_scale_val(cam.screen_size, 0.5);
	p = whitgl_fvec_sub(p, cam.pos);
	p = whitgl_fvec_scale_val(p, cam.scale);
	p = whitgl_fvec_add(p, half_screen_size);
	return p;
}
