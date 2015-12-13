#include "hud.h"

#include <whitgl/logging.h>

#include <menu/text.h>

void space_hud_draw(space_entity src, space_hud_markers m, space_camera camera)
{

	space_sprite hud_sprite = {
		{{-0.25,-2},{0.25,-2},{-0.5,-1.75},{0.5,-1.75}},
		{{0,1},{0,2},{1,3}},
		4,
		3,
		{0x59, 0xc3, 0x49,0xff}
	};
	whitgl_int i;
	for(i=0; i<m.num; i++)
	{
		whitgl_fvec diff = whitgl_fvec_sub(m.markers[i].e.pos, src.pos);
		whitgl_float ang = whitgl_fvec_to_angle(diff)+whitgl_pi/2;
		whitgl_fvec dir = whitgl_fvec_normalize(diff);
		space_sprite_draw(hud_sprite, src.pos, ang, camera);
		space_entity mini = m.markers[i].e;
		mini.pos = whitgl_fvec_add(src.pos, whitgl_fvec_scale_val(dir, 1.5));
		whitgl_int j;
		for(j=0; j<mini.sprite.num_points; j++)
			mini.sprite.points[j] = whitgl_fvec_scale_val(mini.sprite.points[j], 0.1);
		mini.sprite.col = hud_sprite.col;
		space_entity_draw(mini, camera);

		whitgl_fvec text_pos = mini.pos;
		text_pos = whitgl_fvec_add(text_pos, whitgl_fvec_scale_val(dir, 1.5));
		text_pos = space_camera_point(text_pos, camera);

		text_draw(m.markers[i].name, whitgl_fvec_to_ivec(text_pos), 10000, FONT_SMALL, true, 10000);

	}
}
