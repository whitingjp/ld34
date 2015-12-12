#include <stdbool.h>
#include <stddef.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <whitgl/timer.h>

#define MAX_POINTS (32)
#define MAX_LINES (32)
typedef struct
{
	whitgl_fvec points[MAX_POINTS];
	whitgl_ivec lines[MAX_LINES];
	whitgl_int num_points;
	whitgl_int num_lines;
} space_sprite;

space_sprite ship_sprite = {
	{{0.5,0},{1.0,1.0},{0.0,1.0},{0.5,0.8}},
	{{0,1},{1,2},{0,2},{1,3},{2,3},{0,3}},
	4,
	6,
};

typedef struct
{
	whitgl_fvec pos;
	whitgl_float scale;
} space_camera;

whitgl_fvec space_camera_point(whitgl_fvec p, space_camera cam)
{
	p = whitgl_fvec_add(p, cam.pos);
	p = whitgl_fvec_scale_val(p, cam.scale);
	return p;
}

void draw_sprite(space_sprite sprite, space_camera camera)
{
	whitgl_int i;
	for(i=0; i<sprite.num_lines; i++)
	{
		whitgl_faabb line;
		line.a = space_camera_point(sprite.points[sprite.lines[i].x], camera);
		line.b = space_camera_point(sprite.points[sprite.lines[i].y], camera);
		whitgl_sys_color col = {0x00,0xff,0x00,0xff};
		whitgl_sys_draw_line(whitgl_faabb_to_iaabb(line), col);
	}
}

int main()
{
	WHITGL_LOG("Starting main.");

	whitgl_sys_setup setup = whitgl_sys_setup_zero;
	setup.size.x = 280;
	setup.size.y = 180;
	setup.pixel_size = 3;
	setup.name = "space";

	if(!whitgl_sys_init(&setup))
		return 1;

	whitgl_sound_init();
	whitgl_input_init();

	whitgl_timer_init();

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
		}
		whitgl_sys_draw_init();
		space_camera camera = {{1.5,1}, 64};
		draw_sprite(ship_sprite, camera);
		whitgl_sys_draw_finish();
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
