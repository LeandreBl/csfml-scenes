#ifndef _LNAVBAR_H_
#define _LNAVBAR_H_

#include <SFML/Graphics.h>
#include <stdbool.h>
#include <sys/cdefs.h>
#include <LSCENE/lgameobject.h>

typedef struct lsfhnavbar_s {
	lgameobject_t base_object;
	sfRectangleShape *cursor;
	sfRectangleShape *background;
	sfVector2f size;
	float offset;
	float hoffset;
	float ymouse;
	float up;
	float range;
	sfColor color;
	bool moved;
	bool clicked;
} lhnavbar_t;

lgameobject_t *lhnavbar_create(sfVector2f position, sfVector2f size, float range) __THROW;
float lhnavbar_get_scale(lgameobject_t *object) __THROW __nonnull((1));
void lhnavbar_set_range(lgameobject_t *object, float range) __THROW __nonnull((1));
void lhnavbar_set_color(lgameobject_t *object, sfColor color) __THROW __nonnull((1));

#endif /* !_LNAVBAR_H_ */