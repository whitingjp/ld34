#ifndef HUD_H_
#define HUD_H_

#include <camera.h>
#include <mission.h>
#include <sprite.h>

typedef struct
{
	space_entity e;
	const char* name;
} space_hud_marker;

#define MAX_MARKERS (8)
typedef struct
{
	space_hud_marker markers[MAX_MARKERS];
	whitgl_int num;
} space_hud_markers;
static const space_hud_markers space_hud_markers_zero = {{}, 0};

void space_hud_draw(space_entity src, space_hud_markers markers, mission_trade hold, space_camera camera);

#endif // HUD_H_
