#include "capture.h"

#include <stdio.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sys.h>

capture_info capture_info_update(capture_info capture)
{
	if(whitgl_input_pressed(WHITGL_INPUT_Y))
		capture.capturing = !capture.capturing;
	if(!capture.capturing)
	{
		capture.frame = 0;
		capture.timer = 0;
		return capture;
	}
	if(capture.frame == 0)
		capture.n++;
	capture.timer--;
	if(capture.timer > 0)
		return capture;
	capture.timer = 2;
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "../../gifsrc/seq%04d_%04d.png", (int)capture.n, (int)capture.frame);
	WHITGL_LOG("capturing %s", buffer);
	whitgl_sys_capture_frame(buffer);
	capture.frame++;
	return capture;
}
