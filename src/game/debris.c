#include "debris.h"

space_debris space_debris_zero()
{
	space_debris d;
	whitgl_int i;
	for(i=0; i<MAX_PIECES; i++)
		d.pieces[i] = space_debris_piece_zero;
	d.next = 0;
	return d;
}
space_debris space_debris_create(space_debris d, space_entity e, whitgl_fvec speed)
{
	whitgl_int i;
	for(i=0; i<e.sprite.num_lines; i++)
	{
		d.pieces[d.next].active = true;
		d.pieces[d.next].timer = 0;
		d.pieces[d.next].angle_speed = (whitgl_randfloat()-0.5)/4.0;
		d.pieces[d.next].e = e;
		d.pieces[d.next].e.sprite.num_points = 2;
		d.pieces[d.next].e.sprite.num_lines = 1;
		d.pieces[d.next].e.sprite.points[0] = e.sprite.points[e.sprite.lines[i].x];
		d.pieces[d.next].e.sprite.points[1] = e.sprite.points[e.sprite.lines[i].y];
		d.pieces[d.next].e.sprite.lines[0].x = 0;
		d.pieces[d.next].e.sprite.lines[0].y = 1;
		d.pieces[d.next].speed = speed;
		d.pieces[d.next].speed.x += (whitgl_randfloat()-0.5)/10.0;
		d.pieces[d.next].speed.y += (whitgl_randfloat()-0.5)/10.0;
		d.next = whitgl_iwrap(d.next+1, 0, MAX_PIECES);
	}
	return d;
}
space_debris space_debris_update(space_debris d)
{
	whitgl_int i;
	for(i=0; i<MAX_PIECES; i++)
	{
		if(!d.pieces[i].active)
			continue;
		d.pieces[i].e.angle = whitgl_fwrap(d.pieces[i].e.angle + d.pieces[i].angle_speed, 0, whitgl_pi*2);
		d.pieces[i].e.pos = whitgl_fvec_add(d.pieces[i].e.pos, d.pieces[i].speed);
		d.pieces[i].timer = whitgl_fclamp(d.pieces[i].timer + 0.01, 0, 1);
		if(d.pieces[i].timer >= 1.0)
			d.pieces[i].active = false;
	}
	return d;
}
void space_debris_draw(space_debris d, space_camera camera)
{
	whitgl_int i;
	for(i=0; i<MAX_PIECES; i++)
		if(d.pieces[i].active)
			space_entity_draw(d.pieces[i].e, camera);
}
