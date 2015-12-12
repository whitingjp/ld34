#include "sprite.h"

#include <whitgl/sys.h>

void space_sprite_draw(space_sprite sprite, whitgl_fvec position, whitgl_float angle, space_camera camera)
{
	whitgl_int i;
	for(i=0; i<sprite.num_lines; i++)
	{
		whitgl_fvec a = sprite.points[sprite.lines[i].x];
		whitgl_fvec b = sprite.points[sprite.lines[i].y];
		a = whitgl_rotate_point_around_point(a, whitgl_fvec_val(0.5), angle);
		b = whitgl_rotate_point_around_point(b, whitgl_fvec_val(0.5), angle);
		a = whitgl_fvec_add(a, position);
		b = whitgl_fvec_add(b, position);
		a = whitgl_fvec_sub(a, whitgl_fvec_val(0.5));
		b = whitgl_fvec_sub(b, whitgl_fvec_val(0.5));
		whitgl_faabb line;
		line.a = space_camera_point(a, camera);
		line.b = space_camera_point(b, camera);
		whitgl_sys_color col = {0x9f,0xfd,0x3b,0xff};
		whitgl_sys_draw_line(whitgl_faabb_to_iaabb(line), col);
	}
}
