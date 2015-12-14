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
		g.pirates[i] = space_pirate_zero;

	space_station_style zunus = {5, 1, {true,true,true,false,false}};
	g.stations[0] = space_station_zero("Zunus", zunus, 0, 0, MISSION_INTRO);
	space_station_style hutov = {4, 1, {true,false,true,false}};
	g.stations[1] = space_station_zero("Hutov", hutov, -28, -48, MISSION_SNACK);
	space_station_style yutis = {7, 1, {true,true,true,true,true,false,false}};
	g.stations[2] = space_station_zero("Yutis", yutis, -64, 44, MISSION_YUCTIS);
	space_station_style alclov = {6, 1, {true,false,true,false,true,false}};
	g.stations[3] = space_station_zero("Alclov", alclov, 120, -24, MISSION_ALCLOV);
	space_station_style oskao = {4, 2, {true,false,true,false,true,false,true,false}};
	g.stations[4] = space_station_zero("Oskao", oskao, -180, 20, MISSION_OSKAO);
	space_station_style tagawa = {8, 3, {false,true,true,true,true,false,false,false, true,true,true,true,false,false,false,false, true,true,true,false,false,false,false,true}};
	g.stations[5] = space_station_zero("Tagawa", tagawa, 120, -250, MISSION_TAGAWA);
	for(i=0; i<NUM_ASTEROIDS; i++)
		g.asteroids[i] = space_asteroid_zero();
	g.starfield = space_starfield_zero();
	g.debris = space_debris_zero();
	g.hud = space_hud_zero;
	g.target_asteroids = 16;
	g.target_pirates = 0;
	return g;
}

space_save space_game_save(space_save save, space_game game)
{
	save.hold = game.player.hold;
	whitgl_int i;
	for(i=0; i<NUM_STATIONS; i++)
		if(game.stations[i].mission_id != MISSION_RESTORED)
			save.mission_ids[i] = game.stations[i].mission_id;
	return save;
}
space_game space_game_load(space_game game, space_save save)
{
	game.player.hold = save.hold;
	whitgl_int i;
	for(i=0; i<NUM_STATIONS; i++)
		game.stations[i].mission_id = save.mission_ids[i];
	return game;
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


whitgl_fvec _space_game_get_safe_spawn(whitgl_fvec player_pos)
{
	whitgl_float angle = whitgl_randfloat()*whitgl_pi*2;
	whitgl_float dist = whitgl_randfloat()*50+50;
	whitgl_fvec pos = whitgl_fvec_add(player_pos, whitgl_fvec_scale_val(whitgl_angle_to_fvec(angle), dist));
	return pos;
}

space_game space_game_update(space_game g, whitgl_ivec screen_size, whitgl_fvec camera_offset, whitgl_bool in_menu)
{
	whitgl_int i;
	g.target_pirates = whitgl_fclamp(g.target_pirates+whitgl_randfloat()*0.00015, 0, NUM_PIRATES);
	if(!in_menu && g.player.was_in_menu)
	{
		g.target_asteroids = whitgl_fclamp(g.target_asteroids+3, 0, NUM_ASTEROIDS);
		g.target_pirates = whitgl_fclamp(g.target_pirates+whitgl_randfloat()*0.499, 0, NUM_PIRATES);
		whitgl_int asteroids = 0;
		for(i=0; i<NUM_ASTEROIDS; i++)
			if(g.asteroids[i].e.active)
				asteroids++;

		whitgl_int target = g.target_asteroids-asteroids;
		for(i=0; i<NUM_ASTEROIDS; i++)
		{
			if(target <= 0)
				continue;
			if(g.asteroids[i].e.active)
				continue;
			g.asteroids[i] = space_asteroid_zero();
			g.asteroids[i].e.active = true;
			g.asteroids[i].e.pos = _space_game_get_safe_spawn(g.player.e.pos);
			target--;
		}

		whitgl_int pirates = 0;
		for(i=0; i<NUM_PIRATES; i++)
			if(g.pirates[i].e.active)
				pirates++;

		target = g.target_pirates-pirates;
		for(i=0; i<NUM_PIRATES; i++)
		{
			if(target <= 0)
				continue;
			if(g.pirates[i].e.active)
				continue;
			g.pirates[i] = space_pirate_zero;
			g.pirates[i].e.active = true;
			g.pirates[i].e.pos = _space_game_get_safe_spawn(g.player.e.pos);
			target--;
			g.target_pirates--;
		}
	}
	g.hud = space_hud_update(g.hud, g.player.hold);
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

	g.hud.mark = space_hud_markers_zero;
	for(i=0; i<NUM_STATIONS; i++)
	{
		if(!g.stations[i].known)
			continue;
		if(whitgl_fvec_magnitude(whitgl_fvec_sub(g.player.e.pos, g.stations[i].e.pos)) < 15)
			continue;

		g.hud.mark.markers[g.hud.mark.num].e = g.stations[i].e;
		g.hud.mark.markers[g.hud.mark.num].name = g.stations[i].name;
		g.hud.mark.num++;
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

	for(i=0; i<NUM_ASTEROIDS; i++)
	{
		whitgl_float target = 1;
		if(!g.asteroids[i].e.active)
			target = 0;
		if(!g.player.e.active)
			target = 0;
		if(g.player.hold.good != GOOD_TRACTOR)
			target = 0;
		if(g.player.docked != -1)
			target = 0;
		whitgl_fvec diff = whitgl_fvec_sub(g.player.e.pos, g.asteroids[i].e.pos);
		whitgl_float sqmagnitude = whitgl_fvec_sqmagnitude(diff);
		if(sqmagnitude > 10*10)
			target = 0;
		g.asteroids[i].tractor = g.asteroids[i].tractor*0.8 + target*0.2;
		if(sqmagnitude > 10*10)
			continue;
		whitgl_fvec dir = whitgl_fvec_normalize(diff);
		whitgl_fvec thrust = whitgl_fvec_scale_val(dir, g.asteroids[i].tractor*0.001);
		g.asteroids[i].speed = whitgl_fvec_add(g.asteroids[i].speed, thrust);
		g.asteroids[i].speed = whitgl_fvec_interpolate(g.asteroids[i].speed, whitgl_fvec_zero, 0.01);
	}

	for(i=0; i<NUM_ASTEROIDS; i++)
	{
		if(!g.asteroids[i].e.active)
			continue;
		whitgl_int j;
		for(j=0; j<NUM_STATIONS; j++)
		{
			if(!g.stations[j].e.active)
				continue;
			whitgl_fvec diff = whitgl_fvec_sub(g.stations[j].e.pos, g.asteroids[i].e.pos);
			whitgl_float sqmagnitude = whitgl_fvec_sqmagnitude(diff);
			if(sqmagnitude > 10*10)
				continue;
			if(sqmagnitude < 1)
			{
				g.asteroids[i].e.active = false;
				g.debris = space_debris_create(g.debris, g.asteroids[i].e, g.asteroids[i].speed);
			}
			whitgl_fvec dir = whitgl_fvec_normalize(diff);
			whitgl_fvec thrust = whitgl_fvec_scale_val(dir, 0.0005);
			g.asteroids[i].speed = whitgl_fvec_add(g.asteroids[i].speed, thrust);
			g.asteroids[i].speed = whitgl_fvec_interpolate(g.asteroids[i].speed, whitgl_fvec_zero, 0.01);
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
	for(i=0; i<NUM_ASTEROIDS; i++)
	{
		if(g.asteroids[i].tractor <= 0)
			continue;

		whitgl_faabb line;
		line.a = space_camera_point(g.player.e.pos, g.camera);
		line.b = space_camera_point(g.asteroids[i].e.pos, g.camera);
		whitgl_sys_color tractor_col = {0x27,0xd0,0xaa,0xff*g.asteroids[i].tractor};
		whitgl_sys_draw_line(whitgl_faabb_to_iaabb(line), tractor_col);
	}
}
