#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <whitgl/timer.h>
#include <game/game.h>
#include <menu/menu.h>
#include <capture.h>
#include <resource.h>

#include <menu/text.h>

int main(int argc, char* argv[])
{
	whitgl_bool fullscreen = true;
	whitgl_bool vsync = true;
	whitgl_bool can_capture = false;
	int currentarg;
	for(currentarg=1; currentarg<argc; currentarg++)
	{
		if(strncmp(argv[currentarg], "windowed", 8)==0)
			fullscreen = false;
		if(strncmp(argv[currentarg], "novsync", 7)==0)
			vsync = false;
		if(strncmp(argv[currentarg], "capture", 7)==0)
			can_capture = true;
	};

	whitgl_sys_setup setup = whitgl_sys_setup_zero;
	setup.size.x = 336;
	setup.size.y = 216;
	setup.pixel_size = can_capture ? 1 : 2;
	setup.name = "space";
	setup.fullscreen = fullscreen;
	setup.vsync = vsync;
	setup.cursor = CURSOR_HIDE;


	if(!whitgl_sys_init(&setup))
		return 1;

	whitgl_sound_init();
	whitgl_sound_volume(0.25);
	whitgl_input_init();

	whitgl_sys_add_image(IMAGE_FONT, "data/image/font.png");

	whitgl_sound_add(SOUND_THRUST_L, "data/sound/thrust_left.ogg");
	whitgl_sound_add(SOUND_THRUST_R, "data/sound/thrust_right.ogg");
	whitgl_loop_add(SOUND_POWER_L, "data/sound/power_left.ogg");
	whitgl_loop_add(SOUND_POWER_R, "data/sound/power_right.ogg");
	whitgl_sound_add(SOUND_EXPLODE, "data/sound/explode.ogg");

	double now = whitgl_sys_get_time();
	whitgl_randseed(now*10000);

	whitgl_timer_init();

	space_game game = space_game_zero(setup.size);
	space_menu menu = space_menu_zero;
	space_save save;
	save = space_game_save(save, game);

	capture_info capture = capture_info_zero;
	whitgl_float exit_timer = 0;
	bool running = true;
	while(running)
	{
		whitgl_sound_update();

		whitgl_timer_tick();
		while(whitgl_timer_should_do_frame(60))
		{
			whitgl_input_update();

			if(whitgl_input_down(WHITGL_INPUT_ESC))
				exit_timer += 0.02;
			else
				exit_timer -= 0.02;
			exit_timer = whitgl_fclamp(exit_timer, 0, 1);
			if(exit_timer == 1)
				running = false;
			if(whitgl_sys_should_close())
				running = false;

			whitgl_fvec camera_offset = whitgl_fvec_zero;
			camera_offset.x = -0.2*menu.transition;
			whitgl_bool in_menu = menu.transition > 0.5 && menu.buttons[2] < 1;
			game = space_game_update(game, setup.size, camera_offset, in_menu);
			space_station* station = NULL;
			if(game.player.docked != -1)
			{
				station = &game.stations[game.player.docked];
				save = space_game_save(save, game);
			}
			menu = space_menu_update(menu, game, station, &game.player, save.mission_ids[0]);
			if(menu.mission_id != NUM_MISSIONS)
			{
				const whitgl_int* known = &kMissions[menu.mission_id].known_stations[0];
				while(*known != -1)
				{
					game.stations[*known].known = true;
					known++;
				}
			}


			whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
			whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
			if(game.player.dead_timer >= 1 && (r || l))
			{
				game.player = space_player_zero;
				game = space_game_load(game, save);
				menu = space_menu_zero;
				game.stations[0].mission_id = MISSION_RESTORED;
			}
		}
		whitgl_sys_draw_init();

		whitgl_iaabb screen_rect = {whitgl_ivec_zero, setup.size};
		whitgl_sys_color blank_col = {0x0b, 0x1c, 0x1c, 0xff};
		whitgl_sys_draw_iaabb(screen_rect, blank_col);
		space_game_draw(game);
		space_menu_draw(menu, setup.size);

		if(exit_timer > 0)
		{
			whitgl_sys_color col = {0x13,0x89,0x58,0xff};
			whitgl_iaabb inner_box = {{0,0},{48*exit_timer,12}};
			whitgl_sys_color inner_col = {0x0f,0x52,0x3a,0xff};
			whitgl_sys_draw_iaabb(inner_box, inner_col);
			whitgl_iaabb exit_box = {{0,0},{48,12}};

			whitgl_sys_draw_hollow_iaabb(exit_box, 1, col);

			whitgl_ivec exit_text_pos = {24,3};
			text_draw("exiting", exit_text_pos, 100000, FONT_SMALL_BRIGHT, true, -1);
		}


		whitgl_sys_draw_finish();
		if(can_capture)
			capture = capture_info_update(capture);
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
