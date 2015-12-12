#include "menu.h"

#include <string.h>

#include <resource.h>

space_menu space_menu_update(space_menu menu, space_game game)
{
	if(game.docked != -1)
		menu.transition = whitgl_fclamp(menu.transition + 0.05, 0, 1);
	else
		menu.transition = whitgl_fclamp(menu.transition - 0.05, 0, 1);
	return menu;
}

void draw_string(const char* string, whitgl_ivec pos, whitgl_sprite sprite, whitgl_bool centered)
{
	whitgl_int len = strlen(string);
	if(centered)
		pos.x -= (sprite.size.x*len)/2;
	while(*string)
	{
		if(*string >= 'a' && *string <= 'z')
		{
			int index = *string-'a';
			whitgl_ivec frame = {index%6, index/6};
			whitgl_sys_draw_sprite(sprite, frame, pos);
		}
		if(*string >= '0' && *string <= '9')
		{
			int index = *string-'0'+26;
			whitgl_ivec frame = {index%6, index/6};
			whitgl_sys_draw_sprite(sprite, frame, pos);
		}
		pos.x += sprite.size.x;
		string++;
	}
}

void space_menu_draw(space_menu menu, whitgl_ivec screen_size)
{
	if(menu.transition <= 0)
		return;
	whitgl_ivec box_size = {96, 128*menu.transition*menu.transition};
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

	if(title_box.b.y-title_box.a.y == 12)
	{
		whitgl_sprite big_font = {IMAGE_FONT, {36,0}, {12,12}};
		whitgl_ivec title_pos = {(title_box.a.x+title_box.b.x)/2, title_box.a.y};
		draw_string("diso", title_pos, big_font, true);
	}
}
