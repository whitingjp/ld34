#include "game.h"

#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <resource.h>

space_game space_game_zero(whitgl_ivec screen_size)
{
	whitgl_int i;
	space_game g;
	space_camera camera = {{0.0,0.0}, 8, whitgl_ivec_to_fvec(screen_size), {0,0}};
	g.camera = camera;
	g.player = space_player_zero;
	for(i=0; i<NUM_PIRATES; i++)
	{
		g.pirates[i] = space_pirate_zero;
		g.pirates[i].e.pos.x += i*4;
	}
	whitgl_fvec diso_pos = {0,0};
	g.stations[0] = space_station_zero("diso", 5, diso_pos, MISSION_INTRO);
	whitgl_fvec centurai_pos = {-10,-40};
	g.stations[1] = space_station_zero("centurai", 7, centurai_pos, MISSION_SNACK);
	whitgl_fvec alpha_pos = {-50,75};
	g.stations[2] = space_station_zero("alpha", 11, alpha_pos, MISSION_LAUNCH);
	for(i=0; i<NUM_ASTEROIDS; i++)
		g.asteroids[i] = space_asteroid_zero();
	g.starfield = space_starfield_zero();
	g.debris = space_debris_zero();
	return g;
}

void _space_game_consider_focus(space_game* g, space_camera_focus* focus, space_entity* e, whitgl_float range, whitgl_float size, whitgl_float focus_dist)
{
	if(!e->active)
		return;
	whitgl_fvec focus_pos = whitgl_fvec_add(g->player.e.pos, whitgl_fvec_scale_val(g->player.speed, focus_dist));
	whitgl_float diff = whitgl_fvec_magnitude(whitgl_fvec_sub(focus_pos, e->pos));
	whitgl_float mult = e->seen ? 1.25 : 1;
	e->seen = false;
	if(diff < range*mult)
	{
		if(focus->num_foci >= MAX_FOCI)
			WHITGL_PANIC("Run out of foci");
		e->seen = true;
		if(e == &g->player.e)
		{
			focus->foci[focus->num_foci].a = whitgl_fvec_sub(focus_pos, whitgl_fvec_val(size));
			focus->foci[focus->num_foci].b = whitgl_fvec_add(focus_pos, whitgl_fvec_val(size));
		} else
		{
			focus->foci[focus->num_foci].a = whitgl_fvec_sub(e->pos, whitgl_fvec_val(size));
			focus->foci[focus->num_foci].b = whitgl_fvec_add(e->pos, whitgl_fvec_val(size));
		}
		focus->num_foci++;
	}
}

void _space_game_collide_handler(space_game* g, space_entity* e, whitgl_fvec speed)
{
	whitgl_int i;

	if(!e->active)
		return;
	whitgl_float player_sq_dist = whitgl_fvec_sqmagnitude(whitgl_fvec_sub(g->player.e.pos, e->pos));
	if(player_sq_dist > 20*20)
		return;
	whitgl_bool colliding = false;
	for(i=0; i<NUM_STATIONS; i++)
		colliding |= space_entity_colliding(*e, g->stations[i].e);
	for(i=0; i<NUM_ASTEROIDS; i++)
	{
		if(!g->asteroids[i].e.active)
			continue;
		if(&g->asteroids[i].e == e)
			continue;
		if(space_entity_colliding(*e, g->asteroids[i].e))
		{
			colliding = true;
			g->debris = space_debris_create(g->debris, g->asteroids[i].e, g->asteroids[i].speed);
			g->asteroids[i].e.active = false;
		}
	}
	for(i=0; i<NUM_PIRATES; i++)
	{
		if(&g->pirates[i].e == e)
			continue;
		if(!g->pirates[i].e.active)
			continue;
		if(space_entity_colliding(*e, g->pirates[i].e))
		{
			colliding = true;
			g->debris = space_debris_create(g->debris, g->pirates[i].e, g->pirates[i].speed);
			g->pirates[i].e.active = false;
		}
	}
	if(colliding)
	{
		g->debris = space_debris_create(g->debris, *e, speed);
		e->active = false;
		whitgl_sound_play(SOUND_EXPLODE, whitgl_randfloat()/2+0.75);
	}

}

space_game space_game_update(space_game g, whitgl_ivec screen_size, whitgl_fvec camera_offset, whitgl_bool in_menu)
{
	whitgl_int i;
	g.player = space_player_update(g.player, !in_menu);
	for(i=0; i<NUM_PIRATES; i++)
		g.pirates[i] = space_pirate_update(g.pirates[i], g.player.e.pos);
	for(i=0; i<NUM_STATIONS; i++)
		g.stations[i] = space_station_update(g.stations[i]);
	for(i=0; i<NUM_ASTEROIDS; i++)
		g.asteroids[i] = space_asteroid_update(g.asteroids[i], g.player.e.pos);
	g.starfield = space_starfield_update(g.starfield, g.camera.speed, g.camera);
	g.debris = space_debris_update(g.debris);

	g.player.docked = -1;
	for(i=0; i<NUM_STATIONS; i++)
	{
		whitgl_float diff = whitgl_fvec_magnitude(whitgl_fvec_sub(g.player.e.pos, g.stations[i].e.pos));
		if(diff < 1 && g.player.engine_thrust[0]+g.player.engine_thrust[1] < 2.0 && g.player.launch_timer == 0)
			g.player.docked = i;
	}
	if(g.player.docked != -1)
	{
		g.player.e.pos = whitgl_fvec_interpolate(g.player.e.pos, g.stations[g.player.docked].e.pos, 0.1);
		g.player.e.angle = whitgl_angle_lerp(g.player.e.angle, g.stations[g.player.docked].e.angle, 0.1);
	}

	g.hud = space_hud_markers_zero;
	for(i=0; i<NUM_STATIONS; i++)
	{
		if(whitgl_fvec_magnitude(whitgl_fvec_sub(g.player.e.pos, g.stations[i].e.pos)) > 15)
		{
			g.hud.markers[g.hud.num].e = g.stations[i].e;
			g.hud.markers[g.hud.num].name = g.stations[i].name;
			g.hud.num++;
		}
	}

	space_camera_focus focus;

	focus.num_foci = 0;
	_space_game_consider_focus(&g, &focus, &g.player.e, 10000000, 3, 25);
	_space_game_consider_focus(&g, &focus, &g.player.e, 10000000, 3, 25);
	if(!in_menu)
	{
		for(i=0; i<NUM_PIRATES; i++)
			_space_game_consider_focus(&g, &focus, &g.pirates[i].e, 20, 3, 55);
		for(i=0; i<NUM_STATIONS; i++)
			_space_game_consider_focus(&g, &focus, &g.stations[i].e, 20, 2, 55);
		for(i=0; i<NUM_ASTEROIDS; i++)
			_space_game_consider_focus(&g, &focus, &g.asteroids[i].e, 12, 1, 55);
		for(i=0; i<MAX_PIECES; i++)
		{
			if(g.debris.pieces[i].timer < 0.9)
				continue;
			_space_game_consider_focus(&g, &focus, &g.debris.pieces[i].e, 8, 1, 0);
		}
	}
	g.camera = space_camera_update(g.camera, focus, screen_size, camera_offset);

	_space_game_collide_handler(&g, &g.player.e, g.player.speed);
	for(i=0; i<NUM_PIRATES; i++)
		_space_game_collide_handler(&g, &g.pirates[i].e, g.pirates[i].speed);
	for(i=0; i<NUM_ASTEROIDS; i++)
		_space_game_collide_handler(&g, &g.asteroids[i].e, g.asteroids[i].speed);

	for(i=0; i<NUM_STATIONS; i++)
	{
		if(!g.stations[i].e.active)
			continue;
		g.stations[i].defensive = false;
		whitgl_int j;
		for(j=0; j<NUM_ASTEROIDS; j++)
		{
			if(!g.asteroids[j].e.active)
				continue;
			whitgl_float sqmag = whitgl_fvec_sqmagnitude(whitgl_fvec_sub(g.stations[i].e.pos, g.asteroids[j].e.pos));
			if(sqmag < 5*5)
				g.stations[i].defensive = true;
		}
		for(j=0; j<NUM_PIRATES; j++)
		{
			if(!g.pirates[j].e.active)
				continue;
			whitgl_float sqmag = whitgl_fvec_sqmagnitude(whitgl_fvec_sub(g.stations[i].e.pos, g.pirates[j].e.pos));
			if(sqmag < 5*5)
				g.stations[i].defensive = true;
		}
	}

	return g;
}

void space_game_draw(space_game g)
{
	whitgl_int i;
	space_debris_draw(g.debris, g.camera);
	space_player_draw(g.player, g.camera);
	for(i=0; i<NUM_PIRATES; i++)
		space_pirate_draw(g.pirates[i], g.camera);
	for(i=0; i<NUM_STATIONS; i++)
		space_station_draw(g.stations[i], g.camera);
	for(i=0; i<NUM_ASTEROIDS; i++)
		space_asteroid_draw(g.asteroids[i], g.camera);
	space_starfield_draw(g.starfield, g.camera);
	space_hud_draw(g.player.e, g.hud, g.camera);

}
