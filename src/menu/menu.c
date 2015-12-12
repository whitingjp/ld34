#include "menu.h"

#include <stdio.h>
#include <string.h>

#include <resource.h>

space_menu space_menu_update(space_menu menu, space_game game)
{
	if(game.player.docked != -1)
		menu.transition = whitgl_fclamp(menu.transition + 0.05, 0, 1);
	else
		menu.transition = whitgl_fclamp(menu.transition - 0.05, 0, 1);
	if(menu.transition > 0.5)
		menu.num_chars++;
	else
		menu.num_chars = 0;
	return menu;
}

void draw_string(const char* string, whitgl_ivec pos, whitgl_int max_width, whitgl_sprite sprite, whitgl_bool centered)
{
	whitgl_int len = strlen(string);
	if(centered)
		pos.x -= (sprite.size.x*len)/2;
	whitgl_ivec draw_pos = pos;
	while(*string)
	{
		if(*string >= 'a' && *string <= 'z')
		{
			int index = *string-'a';
			whitgl_ivec frame = {index%6, index/6};
			whitgl_sys_draw_sprite(sprite, frame, draw_pos);
		}
		if(*string >= '0' && *string <= '9')
		{
			int index = *string-'0'+26;
			whitgl_ivec frame = {index%6, index/6};
			whitgl_sys_draw_sprite(sprite, frame, draw_pos);
		}
		draw_pos.x += sprite.size.x;
		if(draw_pos.x - pos.x > max_width)
		{
			draw_pos.x = pos.x;
			draw_pos.y += sprite.size.y;
		}
		string++;
	}
}

void space_menu_draw(space_menu menu, whitgl_ivec screen_size)
{
	if(menu.transition <= 0)
		return;
	whitgl_ivec box_size = {156, 180*menu.transition*menu.transition};
	whitgl_iaabb box;
	box.a.x = 12;
	box.a.y = (screen_size.y-box_size.y)/2;
	box.b = whitgl_ivec_add(box.a, box_size);
	whitgl_sys_color col = {0x13,0x89,0x58,0xff};
	whitgl_sys_draw_hollow_iaabb(box, 1, col);
	whitgl_iaabb title_box;
	title_box.a = whitgl_ivec_add(box.a, whitgl_ivec_val(1));
	title_box.b = whitgl_ivec_add(box.b, whitgl_ivec_val(-1));
	if(title_box.b.y-title_box.a.y > 12) title_box.b.y = title_box.a.y+12;
	whitgl_sys_draw_iaabb(title_box, col);

	whitgl_sprite big_font = {IMAGE_FONT, {36,0}, {12,12}};
	whitgl_sprite little_font = {IMAGE_FONT, {0,0}, {6,6}};
	if(title_box.b.y-title_box.a.y == 12)
	{

		whitgl_ivec title_pos = {(title_box.a.x+title_box.b.x)/2, title_box.a.y};
		draw_string("diso", title_pos, box_size.x, big_font, true);
	}
	if(box_size.y == 180)
	{
		whitgl_ivec text_pos = {title_box.a.x+2, title_box.a.y+2+12};
		const char* text =
	//  01234567890123456789012345
		"the quick brown          "
		"fox stole all            "
		"of my cargo              "
		"                         "
		"dammit                   "
		"                         "
		"hey                      "
		"                         "
		"i dont suppose           "
		"you could help           "
		"out                      "
		"                         "
		"i will make it           "
		"worthwhile               ";
		char buffer[1024];
		snprintf(buffer, whitgl_imin(menu.num_chars, 1024), "%s", text);
		draw_string(buffer, text_pos, box_size.x-12, little_font, false);
	}

	whitgl_int button_height = 32;
	whitgl_iaabb lbutton_box = box;
	lbutton_box.b.x -= box_size.x/2;
	lbutton_box.a.y = box.b.y-button_height;
	if(lbutton_box.a.y > box.a.y)
	{
		whitgl_sys_draw_hollow_iaabb(lbutton_box, 1, col);
		whitgl_ivec ltext_pos = {lbutton_box.a.x+box_size.x/4, box.b.y-(button_height-6)/2-6};
		draw_string("sure", ltext_pos, box_size.x-12, little_font, true);
	}


	whitgl_iaabb rbutton_box = box;
	rbutton_box.a.x = lbutton_box.b.x-1;
	rbutton_box.a.y = box.b.y-button_height;
	if(rbutton_box.a.y > box.a.y)
	{
		whitgl_sys_draw_hollow_iaabb(rbutton_box, 1, col);
		whitgl_ivec rtext_pos = {rbutton_box.a.x+box_size.x/4, box.b.y-(button_height-6)/2-6};
		draw_string("no way", rtext_pos, box_size.x-12, little_font, true);
	}
}
