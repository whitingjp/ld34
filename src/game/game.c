#include "game.h"

#include <whitgl/logging.h>

space_game space_game_zero(whitgl_ivec screen_size)
{
	space_game g;
	space_camera camera = {{0.0,0.0}, 1, whitgl_ivec_to_fvec(screen_size), {0,0}};
	g.camera = camera;
	g.player = space_player_zero;
	g.station = space_station_zero();
	g.starfield = space_starfield_zero();
	g.docked = false;
	g.debris = space_debris_zero();
	return g;
}
space_game space_game_update(space_game g, whitgl_ivec screen_size)
{
	g.player = space_player_update(g.player);
	g.station = space_station_update(g.station);
	g.starfield = space_starfield_update(g.starfield, g.camera.speed, g.camera);

	whitgl_float diff = whitgl_fvec_magnitude(whitgl_fvec_sub(g.player.e.pos, g.station.e.pos));
	g.docked = diff < 1 && g.player.engine_thrust[0]+g.player.engine_thrust[1] < 0.2;
	if(g.docked)
	{
		g.player.e.pos = whitgl_fvec_interpolate(g.player.e.pos, g.station.e.pos, 0.05);
		g.player.e.angle = whitgl_angle_lerp(g.player.e.angle, g.station.e.angle, 0.05);
	}
	g.debris = space_debris_update(g.debris);

	space_camera_focus focus;
	focus.num_foci = 0;
	focus.foci[focus.num_foci].a = whitgl_fvec_sub(g.player.e.pos, whitgl_fvec_val(2));
	focus.foci[focus.num_foci].b = whitgl_fvec_add(g.player.e.pos, whitgl_fvec_val(2));
	focus.num_foci++;
	if(diff < 16)
	{
		focus.foci[1].a = whitgl_fvec_sub(g.station.e.pos, whitgl_fvec_val(2));
		focus.foci[1].b = whitgl_fvec_add(g.station.e.pos, whitgl_fvec_val(2));
		focus.num_foci++;
	}
	g.camera = space_camera_update(g.camera, focus, screen_size);

	whitgl_bool colliding = space_entity_colliding(g.player.e, g.station.e);
	if(colliding)
	{
		g.debris = space_debris_create(g.debris, g.player.e, g.player.speed);
		g.player = space_player_zero;

	}
	return g;
}
void space_game_draw(space_game g)
{
	space_debris_draw(g.debris, g.camera);
	space_player_draw(g.player, g.camera);
	space_station_draw(g.station, g.camera);
	space_starfield_draw(g.starfield, g.camera);
}
