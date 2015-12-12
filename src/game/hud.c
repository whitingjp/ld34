#include "hud.h"

#include <whitgl/logging.h>

void space_hud_draw(space_entity src, space_hud_markers m, space_camera camera)
{

	space_sprite hud_sprite = {
		{{-0.25,-2},{0.25,-2},{-0.5,-1.75},{0.5,-1.75}},
		{{0,1},{0,2},{1,3}},
		4,
		3,
	};
	whitgl_int i;
	for(i=0; i<m.num; i++)
	{
		whitgl_fvec diff = whitgl_fvec_sub(m.marker[i].pos, src.pos);

		whitgl_float ang = whitgl_fvec_to_angle(diff)+whitgl_pi/2;
		space_sprite_draw(hud_sprite, src.pos, ang, camera);
		// whitgl_entity mini = m[i];
		// mini.pos = src.pos;
		// whitgl_int j;
		// for(j=0; j<
	}
}
