#include "text.h"


#include <stdio.h>
#include <string.h>

#include <resource.h>

whitgl_sprite big_font = {IMAGE_FONT, {36,0}, {12,12}};
whitgl_sprite little_font = {IMAGE_FONT, {0,0}, {6,6}};
whitgl_sprite little_bright_font = {IMAGE_FONT, {0,48}, {6,6}};

void text_draw(const char* string, whitgl_ivec pos, whitgl_int max_width, text_font font, whitgl_bool centered, whitgl_int visible_chars)
{
	whitgl_sprite sprite;
	if(font == FONT_BIG)
		sprite = big_font;
	else if(font == FONT_SMALL)
		sprite = little_font;
	else
		sprite = little_bright_font;
	whitgl_int len = strlen(string);
	if(centered)
		pos.x -= (sprite.size.x*len)/2;
	whitgl_ivec draw_pos = pos;
	while(*string && visible_chars)
	{
		int index = -1;
		if(*string >= 'a' && *string <= 'z')
			index = *string-'a';
		if(*string >= 'A' && *string <= 'Z')
			index = *string-'A';
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
		if(*string == '?')
			index = 42;
		if(*string == '-')
			index = 43;
		if(*string == '<')
			index = 44;
		if(*string == '>')
			index = 45;
		if(*string == '\n')
			draw_pos.x += 10000;
		whitgl_int chars_left_in_word = 0;
		const char* word = string;
		while(*word && *word != ' ' && *word != '\n')
		{
			chars_left_in_word++;
			word++;
		}
		if(draw_pos.x - pos.x + chars_left_in_word*sprite.size.x > max_width)
		{
			draw_pos.x = pos.x;
			draw_pos.y += sprite.size.y;
		}
		if(index != -1)
		{
			whitgl_ivec frame = {index%6, index/6};
			whitgl_sys_draw_sprite(sprite, frame, draw_pos);
		}

		if(*string != '\n')
		{
			draw_pos.x += sprite.size.x;
		}
		string++;
		visible_chars--;
	}
}
