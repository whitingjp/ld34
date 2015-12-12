#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <whitgl/timer.h>
#include <game/player.h>
#include <game/station.h>
#include <game/starfield.h>
#include <camera.h>
#include <capture.h>
#include <resource.h>

int main(int argc, char* argv[])
{
	whitgl_bool fullscreen = false;
	whitgl_bool vsync = true;
	whitgl_bool can_capture = false;
	int currentarg;
	for(currentarg=1; currentarg<argc; currentarg++)
	{
		if(strncmp(argv[currentarg], "fullscreen", 10)==0)
			fullscreen = true;
		if(strncmp(argv[currentarg], "novsync", 7)==0)
			vsync = false;
		if(strncmp(argv[currentarg], "capture", 7)==0)
			can_capture = true;
	};

	whitgl_sys_setup setup = whitgl_sys_setup_zero;
	setup.size.x = 280;
	setup.size.y = 180;
	setup.pixel_size = 3;
	setup.name = "space";
	setup.fullscreen = fullscreen;
	setup.vsync = vsync;
	setup.cursor = CURSOR_HIDE;


	if(!whitgl_sys_init(&setup))
		return 1;

	whitgl_sound_init();
	whitgl_sound_volume(0.25);
	whitgl_input_init();

	whitgl_sound_add(SOUND_THRUST_L, "data/sound/thrust_left.ogg");
	whitgl_sound_add(SOUND_THRUST_R, "data/sound/thrust_right.ogg");
	whitgl_loop_add(SOUND_POWER_L, "data/sound/power_left.ogg");
	whitgl_loop_add(SOUND_POWER_R, "data/sound/power_right.ogg");

	whitgl_timer_init();

	capture_info capture = capture_info_zero;
	space_camera camera = {{0.0,0.0}, 1, whitgl_ivec_to_fvec(setup.size), {0,0}};
	space_player player = space_player_zero;
	space_station station = space_station_zero();
	space_starfield starfield = space_starfield_zero();
	bool docked = false;
	bool running = true;
	while(running)
	{
		whitgl_sound_update();

		whitgl_timer_tick();
		while(whitgl_timer_should_do_frame(60))
		{
			whitgl_input_update();
			if(whitgl_input_pressed(WHITGL_INPUT_ESC))
				running = false;
			if(whitgl_sys_should_close())
				running = false;

			player = space_player_update(player);
			station = space_station_update(station);
			starfield = space_starfield_update(starfield, camera.speed, camera);

			whitgl_float diff = whitgl_fvec_magnitude(whitgl_fvec_sub(player.pos, station.pos));
			docked = diff < 1 && player.engine_thrust[0]+player.engine_thrust[1] < 0.2;
			if(docked)
			{
				player.pos = whitgl_fvec_interpolate(player.pos, station.pos, 0.05);
				player.angle = whitgl_angle_lerp(player.angle, station.angle, 0.05);
			}

			space_camera_focus focus;
			focus.num_foci = 0;
			focus.foci[focus.num_foci].a = whitgl_fvec_sub(player.pos, whitgl_fvec_val(2));
			focus.foci[focus.num_foci].b = whitgl_fvec_add(player.pos, whitgl_fvec_val(2));
			focus.num_foci++;
			if(diff < 16)
			{
				focus.foci[1].a = whitgl_fvec_sub(station.pos, whitgl_fvec_val(2));
				focus.foci[1].b = whitgl_fvec_add(station.pos, whitgl_fvec_val(2));
				focus.num_foci++;
			}
			camera = space_camera_update(camera, focus, setup.size);

		}
		whitgl_sys_draw_init();

		whitgl_iaabb screen_rect = {whitgl_ivec_zero, setup.size};
		whitgl_sys_color blank_col = {0x0b, 0x1c, 0x1c, 0xff};
		whitgl_sys_draw_iaabb(screen_rect, blank_col);
		space_player_draw(player, camera);
		space_station_draw(station, camera);
		space_starfield_draw(starfield, camera);
		whitgl_sys_draw_finish();
		if(can_capture)
			capture = capture_info_update(capture);
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
