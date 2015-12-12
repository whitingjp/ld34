#include "menu.h"

space_menu space_menu_update(space_menu menu, space_game game)
{
	if(game.docked != -1)
		menu.transition = whitgl_fclamp(menu.transition + 0.05, 0, 1);
	else
		menu.transition = whitgl_fclamp(menu.transition - 0.05, 0, 1);
	return menu;
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
	if(title_box.b.y-title_box.a.y > 13) title_box.b.y = title_box.a.y+13;
	whitgl_sys_draw_iaabb(title_box, col);

}
