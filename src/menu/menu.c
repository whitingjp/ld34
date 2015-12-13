#include "menu.h"

#include <stdio.h>
#include <string.h>

#include <whitgl/input.h>
#include <resource.h>

space_menu space_menu_update(space_menu m, space_game game)
{
	if(game.player.docked != -1)
		m.transition = whitgl_fclamp(m.transition + 0.05, 0, 1);
	else
		m.transition = whitgl_fclamp(m.transition - 0.05, 0, 1);
	if(m.transition > 0.5)
		m.num_chars++;
	else
		m.num_chars = 0;

	whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
	whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
	whitgl_float speed = 0.05;
	if(l && !r)
		m.buttons[0] += speed;
	else
		m.buttons[0] -= speed*2;
	if(r && !l)
		m.buttons[1] += speed;
	else
		m.buttons[1] -= speed*2;
	if(l && r)
		m.buttons[2] += speed;
	else
		m.buttons[2] -= speed*2;
	whitgl_int i;
	for(i=0; i<3; i++)
		m.buttons[i] = whitgl_fclamp(m.buttons[i], 0, 1);

	return m;
}

void draw_string(const char* string, whitgl_ivec pos, whitgl_int max_width, whitgl_sprite sprite, whitgl_bool centered)
{
	whitgl_int len = strlen(string);
	if(centered)
		pos.x -= (sprite.size.x*len)/2;
	whitgl_ivec draw_pos = pos;
	while(*string)
	{
		int index = -1;
		if(*string >= 'a' && *string <= 'z')
			index = *string-'a';
		if(*string >= '0' && *string <= '9')
			index = *string-'0'+26;
		if(*string == ',')
			index = 36;
		if(*string == '.')
			index = 37;
		if(*string == ':')
			index = 38;
		if(*string == '$')
			index = 39;
		if(*string == '!')
			index = 40;
		if(*string == '\'')
			index = 41;
		if(*string == '\n')
			draw_pos.x += 10000;
		if(index != -1)
		{
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

void space_menu_draw(space_menu m, whitgl_ivec screen_size)
{
	if(m.transition <= 0)
		return;
	whitgl_ivec box_size = {156, 180*m.transition*m.transition};
	whitgl_iaabb box;
	box.a.x = 12;
	box.a.y = (screen_size.y-box_size.y)/2;
	box.b = whitgl_ivec_add(box.a, box_size);
	whitgl_sys_color col = {0x13,0x89,0x58,0xff};
	whitgl_sys_color inner_col = {0x0f,0x52,0x3a,0xff};
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
		draw_string("diso!!", title_pos, box_size.x, big_font, true);
	}
	if(box_size.y == 180)
	{
		whitgl_ivec text_pos = {title_box.a.x+2, title_box.a.y+2+12};
		const char* text =
	//  01234567890123456789012345
		"the quick brown.\n"
		"fox stole all,\n"
		"of my cargo!\n"
		"\n"
		"dammit$\n"
		"\n"
		"hey\n"
		"\n"
		"i dont suppose\n"
		"you could help\n"
		"out\n"
		"\n"
		"i will' make it\n"
		"worthwhile\n";
		char buffer[1024];
		snprintf(buffer, whitgl_imin(m.num_chars, 1024), "%s", text);
		draw_string(buffer, text_pos, box_size.x-12, little_font, false);
	}

	whitgl_int button_height = 16;
	whitgl_iaabb launch_box = box;
	launch_box.a.y = box.b.y-button_height;
	whitgl_iaabb launch_box_fill = launch_box;
	whitgl_int width = launch_box.b.x - launch_box.a.x;
	launch_box_fill.a.x += width*(1-m.buttons[2])/2;
	launch_box_fill.b.x -= width*(1-m.buttons[2])/2;
	if(launch_box.a.y > box.a.y)
	{
		whitgl_sys_draw_iaabb(launch_box_fill, inner_col);
		whitgl_sys_draw_hollow_iaabb(launch_box, 1, col);
		whitgl_ivec ltext_pos = {launch_box.a.x+box_size.x/2, box.b.y-(button_height-6)/2-6};
		draw_string("launch", ltext_pos, box_size.x-12, little_font, true);
	}


	whitgl_iaabb lbutton_box = launch_box;
	lbutton_box.b.x -= box_size.x/2;
	lbutton_box.a.y -= 24-1;
	lbutton_box.b.y -= 16-1;
	whitgl_iaabb left_box_fill = lbutton_box;
	whitgl_int lwidth = lbutton_box.b.x - lbutton_box.a.x;
	left_box_fill.a.x += lwidth*(1-m.buttons[0]*m.buttons[0]);


	if(lbutton_box.a.y > box.a.y)
	{
		whitgl_sys_draw_iaabb(left_box_fill, inner_col);
		whitgl_sys_draw_hollow_iaabb(lbutton_box, 1, col);
		whitgl_ivec ltext_pos = {lbutton_box.a.x+box_size.x/4, lbutton_box.b.y-(24-6)/2-6};
		draw_string("sure", ltext_pos, box_size.x-12, little_font, true);
	}

	whitgl_iaabb rbutton_box = lbutton_box;
	rbutton_box.a.x += box_size.x/2-1;
	rbutton_box.b.x += box_size.x/2;

	whitgl_iaabb right_box_fill = rbutton_box;
	whitgl_int rwidth = rbutton_box.b.x - rbutton_box.a.x;
	right_box_fill.b.x -= rwidth*(1-m.buttons[1]*m.buttons[1]);

	if(rbutton_box.a.y > box.a.y)
	{
		whitgl_sys_draw_iaabb(right_box_fill, inner_col);
		whitgl_sys_draw_hollow_iaabb(rbutton_box, 1, col);
		whitgl_ivec rtext_pos = {rbutton_box.a.x+box_size.x/4, rbutton_box.b.y-(24-6)/2-6};
		draw_string("no way", rtext_pos, box_size.x-12, little_font, true);
	}
}
