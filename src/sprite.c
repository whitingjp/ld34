#include "sprite.h"

#include <whitgl/sys.h>

void space_sprite_draw(space_sprite sprite, whitgl_fvec position, whitgl_float angle, space_camera camera)
{
	whitgl_int i;
	for(i=0; i<sprite.num_lines; i++)
	{
		whitgl_fvec a = sprite.points[sprite.lines[i].x];
		whitgl_fvec b = sprite.points[sprite.lines[i].y];
		a = whitgl_rotate_point_around_point(a, whitgl_fvec_zero, angle);
		b = whitgl_rotate_point_around_point(b, whitgl_fvec_zero, angle);
		a = whitgl_fvec_add(a, position);
		b = whitgl_fvec_add(b, position);
		whitgl_faabb line;
		line.a = space_camera_point(a, camera);
		line.b = space_camera_point(b, camera);
		whitgl_sys_color col = {0x9f,0xfd,0x3b,0xff};
		whitgl_sys_draw_line(whitgl_faabb_to_iaabb(line), col);
	}
}

void space_entity_draw(space_entity e, space_camera camera)
{
	space_sprite_draw(e.sprite, e.pos, e.angle, camera);
}

whitgl_bool space_entity_colliding(space_entity ea, space_entity eb)
{
	whitgl_int i, j;
	for(i=0; i<ea.sprite.num_lines; i++)
	{
		for(j=0; j<eb.sprite.num_lines; j++)
		{
			whitgl_fvec a = ea.sprite.points[ea.sprite.lines[i].x];
			whitgl_fvec b = ea.sprite.points[ea.sprite.lines[i].y];
			a = whitgl_rotate_point_around_point(a, whitgl_fvec_zero, ea.angle);
			b = whitgl_rotate_point_around_point(b, whitgl_fvec_zero, ea.angle);
			a = whitgl_fvec_add(a, ea.pos);
			b = whitgl_fvec_add(b, ea.pos);

			whitgl_fvec c = eb.sprite.points[eb.sprite.lines[j].x];
			whitgl_fvec d = eb.sprite.points[eb.sprite.lines[j].y];
			c = whitgl_rotate_point_around_point(c, whitgl_fvec_zero, eb.angle);
			d = whitgl_rotate_point_around_point(d, whitgl_fvec_zero, eb.angle);
			c = whitgl_fvec_add(c, eb.pos);
			d = whitgl_fvec_add(d, eb.pos);

			float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
			float numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
			float numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));

			if (denominator == 0)
			{
				if(numerator1 == 0 && numerator2 == 0)
					return true;
				continue;
			}

			float r = numerator1 / denominator;
			float s = numerator2 / denominator;

			if((r >= 0 && r <= 1) && (s >= 0 && s <= 1))
			return true;
		}
	}
	return false;
}
