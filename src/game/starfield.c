#include "starfield.h"

#include <whitgl/sys.h>

whitgl_faabb bounds = {{-20,-10},{20,10}};

space_starfield space_starfield_zero()
{
	space_starfield s;
	whitgl_int i;
	for(i=0; i<NUM_STARS; i++)
	{
		whitgl_fvec size = whitgl_fvec_sub(bounds.b, bounds.a);
		s.stars[i].pos.x = bounds.a.x+whitgl_randfloat()*size.x;
		s.stars[i].pos.y = bounds.a.y+whitgl_randfloat()*size.y;
		s.stars[i].depth = whitgl_randfloat();
		s.stars[i].alpha = whitgl_randfloat();
	}
	return s;
}
space_starfield space_starfield_update(space_starfield s, whitgl_fvec speed, space_camera camera)
{
	whitgl_int i;
	whitgl_faabb live_bounds = whitgl_faabb_add(bounds, camera.pos);
	for(i=0; i<NUM_STARS; i++)
	{
		s.stars[i].pos = whitgl_fvec_add(s.stars[i].pos, whitgl_fvec_scale_val(speed, -s.stars[i].depth));
		s.stars[i].pos.x = whitgl_fwrap(s.stars[i].pos.x, live_bounds.a.x, live_bounds.b.x);
		s.stars[i].pos.y = whitgl_fwrap(s.stars[i].pos.y, live_bounds.a.y, live_bounds.b.y);
	}
	return s;
}
void space_starfield_draw(space_starfield s, space_camera camera)
{
	whitgl_int i;
	for(i=0; i<NUM_STARS; i++)
	{
		whitgl_fvec pos = space_camera_point(s.stars[i].pos, camera);
		whitgl_iaabb box;
		box.a = whitgl_fvec_to_ivec(pos);
		box.b = whitgl_ivec_add(box.a, whitgl_ivec_val(1));
		whitgl_sys_color col = {0x59, 0xc3, 0x49, 0xff*s.stars[i].alpha};
		whitgl_sys_draw_iaabb(box, col);
	}
}
