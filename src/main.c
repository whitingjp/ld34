#include <stdbool.h>
#include <stddef.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <whitgl/timer.h>
#include <capture.h>

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

void draw_sprite(space_sprite sprite, whitgl_fvec position, whitgl_float angle, space_camera camera)
{
	whitgl_int i;
	for(i=0; i<sprite.num_lines; i++)
	{
		whitgl_fvec a = sprite.points[sprite.lines[i].x];
		whitgl_fvec b = sprite.points[sprite.lines[i].y];
		a = whitgl_rotate_point_around_point(a, whitgl_fvec_val(0.5), angle);
		b = whitgl_rotate_point_around_point(b, whitgl_fvec_val(0.5), angle);
		a = whitgl_fvec_add(a, position);
		b = whitgl_fvec_add(b, position);
		a = whitgl_fvec_sub(a, whitgl_fvec_val(0.5));
		b = whitgl_fvec_sub(b, whitgl_fvec_val(0.5));
		whitgl_faabb line;
		line.a = space_camera_point(a, camera);
		line.b = space_camera_point(b, camera);
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

	capture_info capture = capture_info_zero;
	bool capturing = false;
	whitgl_float angle;
	whitgl_fvec position = {2,2};
	whitgl_fvec speed = whitgl_fvec_zero;
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
			whitgl_bool l = whitgl_input_down(WHITGL_INPUT_LEFT);
			whitgl_bool r = whitgl_input_down(WHITGL_INPUT_RIGHT);
			if(l&&r)
			{
				whitgl_fvec impulse = {0,-0.005};
				impulse = whitgl_rotate_point_around_point(impulse, whitgl_fvec_zero, angle);
				speed = whitgl_fvec_add(speed, impulse);
			}
			if(l)
				angle = whitgl_fwrap(angle-0.1, 0, whitgl_pi*2);
			if(r)
				angle = whitgl_fwrap(angle+0.1, 0, whitgl_pi*2);
			position = whitgl_fvec_add(position, speed);
		}
		whitgl_sys_draw_init();
		space_camera camera = {{0.0,0.0}, 32};
		whitgl_iaabb screen_rect = {whitgl_ivec_zero, setup.size};
		whitgl_sys_color blank_col = {0x00, 0x00, 0x00, 0xff};
		whitgl_sys_draw_iaabb(screen_rect, blank_col);
		draw_sprite(ship_sprite, position, angle, camera);
		whitgl_sys_draw_finish();
		capture = capture_info_update(capture, capturing);
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
