#include "hud.h"

#include <stdio.h>

#include <whitgl/logging.h>

#include <menu/text.h>


static const char* mission_good_names[NUM_GOODS] =
{
	"Empty",
	"Calamari",
	"Red Crabs",
	"Tractor",
};


space_hud space_hud_update(space_hud h, mission_trade hold)
{
	h.bounce_timer = whitgl_fclamp(h.bounce_timer-0.05, 0, 1);
	if(hold.creds != h.hold.creds)
		h.bounce_timer = 1;
	if(hold.good != h.hold.good)
		h.bounce_timer = 1;
	h.hold = hold;
	return h;
}

void space_hud_draw(space_entity src, space_hud h, space_camera camera)
{
	space_sprite hud_sprite = {
		{{-0.25,-2},{0.25,-2},{-0.5,-1.75},{0.5,-1.75}},
		{{0,1},{0,2},{1,3}},
		4,
		3,
		{0x59, 0xc3, 0x49,0xff}
	};
	whitgl_int i;
	for(i=0; i<h.mark.num; i++)
	{
		whitgl_fvec diff = whitgl_fvec_sub(h.mark.markers[i].e.pos, src.pos);
		whitgl_float ang = whitgl_fvec_to_angle(diff)+whitgl_pi/2;
		whitgl_fvec dir = whitgl_fvec_normalize(diff);
		space_sprite_draw(hud_sprite, src.pos, ang, camera);
		space_entity mini = h.mark.markers[i].e;
		mini.pos = whitgl_fvec_add(src.pos, whitgl_fvec_scale_val(dir, 1.5));
		whitgl_int j;
		for(j=0; j<mini.sprite.num_points; j++)
			mini.sprite.points[j] = whitgl_fvec_scale_val(mini.sprite.points[j], 0.1);
		mini.sprite.col = hud_sprite.col;
		space_entity_draw(mini, camera);

		whitgl_fvec text_pos = mini.pos;
		text_pos = whitgl_fvec_add(text_pos, whitgl_fvec_scale_val(dir, 1.5));
		text_pos = space_camera_point(text_pos, camera);

		text_draw(h.mark.markers[i].name, whitgl_fvec_to_ivec(text_pos), 10000, FONT_SMALL, true, 10000);
	}

	whitgl_ivec text_pos = {camera.screen_size.x-64+30, 2+h.bounce_timer*10};
	text_draw("hold", text_pos, 10000, FONT_SMALL, true, 10000);
	const char* good_name = mission_good_names[h.hold.good];
	text_pos.y += 8;
	text_draw(good_name, text_pos, 10000, FONT_SMALL_BRIGHT, true, 10000);
	text_pos.y += 12;
	text_draw("credits", text_pos, 10000, FONT_SMALL, true, 10000);
	text_pos.y += 8;
	char buffer[128];
	snprintf(buffer, 128, "$%d", (int)h.hold.creds);
	text_draw(buffer, text_pos, 10000, FONT_SMALL_BRIGHT, true, 10000);
}
