#include "menu.h"

#include <stdio.h>
#include <string.h>

#include <menu/text.h>
#include <whitgl/input.h>
#include <resource.h>

space_menu space_menu_update(space_menu m, space_game game, space_station* station, space_player* player, mission_index mission_resume)
{
	if(station)
	{
		m.mission_id = station->mission_id;
	}
	else
	{
		m.state = STATE_QUESTION;
		m.has_accepted = false;
	}

	if(game.player.docked != -1)
		m.transition = whitgl_fclamp(m.transition + 0.05, 0, 1);
	else
		m.transition = whitgl_fclamp(m.transition - 0.05, 0, 1);

	if(!station)
		return m;

	m.name = station->name;

	if(m.transition > 0.5)
		m.num_chars++;
	else
		m.num_chars = 0;

	whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
	whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
	whitgl_float speed = 0.05;
	if(l && !r && m.has_choice)
		m.buttons[0] += speed;
	else
		m.buttons[0] -= speed*2;
	if(r && !l && m.has_choice)
		m.buttons[1] += speed;
	else
		m.buttons[1] -= speed*2;
	if(l && r && m.can_launch)
		m.buttons[2] += speed;
	else
		m.buttons[2] -= speed*2;
	whitgl_int i;
	for(i=0; i<3; i++)
		m.buttons[i] = whitgl_fclamp(m.buttons[i], 0, 1);

	if(m.mission_id != NUM_MISSIONS)
	{
		mission_mission mission = kMissions[m.mission_id];
		if(mission.replacement == MISSION_RESUME)
			mission.replacement = mission_resume;
		m.have_required = true;
		if(mission.need.good != GOOD_NONE && game.player.hold.good != mission.need.good)
			m.have_required = false;
		if(mission.need.creds != 0 && game.player.hold.creds < mission.need.creds)
			m.have_required = false;
		if(station && mission.replacement != NUM_MISSIONS && m.have_required && mission.replacement_type == REPLACE_ON_MET_NEED)
		{
			station->mission_id = mission.replacement;
			m.state = STATE_QUESTION;
			m.num_chars = 0;
			return m;
		}
		mission_page page;
		if(m.state == STATE_QUESTION)
			page = m.have_required ? mission.have_page : mission.need_page;
		else if(m.state == STATE_ACCEPTED)
			page = mission.accepted;
		else
			page = mission.rejected;
		m.has_choice = strlen(page.left) > 0;
		m.can_launch = strlen(page.launch) > 0;

		if(m.has_choice)
		{
			if(m.buttons[0] >= 1 || m.buttons[1] >= 1)
			{
				whitgl_bool accepted = m.buttons[0] >= 1;
				if(accepted)
				{
					m.state = STATE_ACCEPTED;
					if(mission.need.good != GOOD_NONE)
						player->hold.good = GOOD_NONE;
					if(mission.need.creds != 0)
						player->hold.creds -= mission.need.creds;
					if(mission.have.good != GOOD_NONE)
						player->hold.good = mission.have.good;
					if(mission.have.creds != 0)
						player->hold.creds += mission.have.creds;
				} else
				{
					m.state = STATE_REJECTED;
				}

				if(station && mission.replacement != NUM_MISSIONS && mission.replacement_type == REPLACE_ON_ANY_ANSWER)
				{
					station->mission_id = mission.replacement;
					m.state = STATE_QUESTION;
					m.num_chars = 0;
					return m;
				}
				m.has_accepted = accepted;

				m.num_chars = 0;
			}
		}
		bool replace_on_leave = mission.replacement_type == REPLACE_ON_ANY_ANSWER || (mission.replacement_type == REPLACE_ON_ACCEPTED_LAUNCH && m.has_accepted);
		if(station && mission.replacement != NUM_MISSIONS && replace_on_leave && m.buttons[2] >= 1)
		{
			station->mission_id = mission.replacement;
			m.state = STATE_QUESTION;
			m.num_chars = 0;
			return m;
		}
	}

	return m;
}

void space_menu_draw(space_menu m, whitgl_ivec screen_size)
{
	if(m.transition <= 0)
		return;
	mission_mission mission = kMissions[m.mission_id];
	mission_page page;
	if(m.state == STATE_QUESTION)
		page = m.have_required ? mission.have_page : mission.need_page;
	else if(m.state == STATE_ACCEPTED)
		page = mission.accepted;
	else
		page = mission.rejected;

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

	if(title_box.b.y-title_box.a.y == 12)
	{

		whitgl_ivec title_pos = {(title_box.a.x+title_box.b.x)/2, title_box.a.y};
		text_draw(m.name, title_pos, box_size.x, FONT_BIG, true, -1);
	}
	if(box_size.y == 180)
	{
		whitgl_ivec text_pos = {title_box.a.x+2, title_box.a.y+2+12};
		text_draw(page.text, text_pos, box_size.x-12, FONT_SMALL, false, m.num_chars);
	}

	char buffer[128];
	whitgl_int button_height = 16;
	whitgl_iaabb launch_box = box;
	launch_box.a.y = box.b.y;
	if(m.can_launch)
	{
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
			snprintf(buffer, 128, "<%s>", page.launch);
			text_draw(buffer, ltext_pos, box_size.x-12, FONT_SMALL, true, -1);
		}
		launch_box.a.y+=1;
	}


	if(m.has_choice)
	{
		whitgl_iaabb lbutton_box = launch_box;
		lbutton_box.b.x -= box_size.x/2;
		lbutton_box.a.y = launch_box.a.y - 24-1;
		lbutton_box.b.y = launch_box.a.y;
		whitgl_iaabb left_box_fill = lbutton_box;
		whitgl_int lwidth = lbutton_box.b.x - lbutton_box.a.x;
		left_box_fill.a.x += lwidth*(1-m.buttons[0]*m.buttons[0]);


		if(lbutton_box.a.y > box.a.y)
		{
			whitgl_sys_draw_iaabb(left_box_fill, inner_col);
			whitgl_sys_draw_hollow_iaabb(lbutton_box, 1, col);
			whitgl_ivec ltext_pos = {lbutton_box.a.x+box_size.x/4, lbutton_box.b.y-(24-6)/2-6};
			snprintf(buffer, 128, "<%s", page.left);
			text_draw(buffer, ltext_pos, box_size.x-12, FONT_SMALL, true, -1);
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
			snprintf(buffer, 128, "%s>", page.right);
			text_draw(buffer, rtext_pos, box_size.x-12, FONT_SMALL, true, -1);
		}
	}
}
