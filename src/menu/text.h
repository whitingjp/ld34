#ifndef TEXT_H_
#define TEXT_H_

#include <whitgl/sys.h>

typedef enum
{
	FONT_SMALL,
	FONT_BIG,
} text_font;

void text_draw(const char* string, whitgl_ivec pos, whitgl_int max_width, text_font font, whitgl_bool centered, whitgl_int visible_chars);

#endif // TEXT_H_
