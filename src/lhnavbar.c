#include <stdlib.h>
#include <math.h>

#include "LSCENE/lgameobject_types.h"
#include "LSCENE/lcolor.h"
#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/lnavbar.h"

static void catch_event(lgameobject_t *object, const sfEvent *event)
{
	lhnavbar_t *navbar = (lhnavbar_t *)object;
	sfFloatRect rect = sfRectangleShape_getGlobalBounds(navbar->cursor);

	if (event->type == sfEvtMouseMoved) {
		navbar->moved = sfFloatRect_contains(&rect, event->mouseMove.x, event->mouseMove.y);
		navbar->ymouse = event->mouseMove.y;
	}
	else if (event->type == sfEvtMouseButtonReleased)
		navbar->clicked = false;
	else {
		navbar->clicked =
			sfFloatRect_contains(&rect, event->mouseButton.x, event->mouseButton.y);
		navbar->hoffset = event->mouseButton.y - lgameobject_get_position(object).y;
		navbar->ymouse = event->mouseButton.y;
	}
}

static void update(lgameobject_t *object)
{
	lhnavbar_t *navbar = (lhnavbar_t *)object;
	sfVector2f position;

	if (navbar->clicked) {
		sfRectangleShape_setFillColor(navbar->cursor, color_div(navbar->color, 1.5));
		position = lgameobject_get_position(object);
		position.y = navbar->ymouse - navbar->hoffset;
		if (position.y <= navbar->up + navbar->offset / 2)
			position.y = navbar->up + navbar->offset / 2;
		else if (position.y
			 >= navbar->range + navbar->up - navbar->size.y + navbar->offset / 2)
			position.y =
				navbar->range + navbar->up - navbar->size.y + navbar->offset / 2;
		lgameobject_set_position(object, position);
	}
	else if (navbar->moved)
		sfRectangleShape_setFillColor(navbar->cursor, color_div(navbar->color, 1.25));
	else
		sfRectangleShape_setFillColor(navbar->cursor, navbar->color);
	sfRectangleShape_setPosition(navbar->cursor, sfSprite_getPosition(object->sprite));
	sfRenderWindow_drawRectangleShape(object->scene->window, navbar->background, NULL);
	sfRenderWindow_drawRectangleShape(object->scene->window, navbar->cursor, NULL);
}

static void lhnavbar_destroy(lgameobject_t *object)
{
	lhnavbar_t *navbar = (lhnavbar_t *)object;

	sfRectangleShape_destroy(navbar->cursor);
	sfRectangleShape_destroy(navbar->background);
}

lgameobject_t *lhnavbar_create(sfVector2f position, sfVector2f size, float range)
{
	lhnavbar_t *object = calloc(1, sizeof(*object));

	if (object == NULL || lgameobject_create(&object->base_object, "hnavbar") == -1)
		return (NULL);
	object->cursor = sfRectangleShape_create();
	object->background = sfRectangleShape_create();
	if (object->cursor == NULL) {
		lhnavbar_destroy(&object->base_object);
		return (NULL);
	}
	object->size = size;
	object->range = range;
	object->offset = size.x * 0.3 + fmodf(size.x * 0.3, 2);
	object->up = position.y;
	object->color = sfWhite;
	object->base_object.type = LHNAVBAR_TYPE;
	lhnavbar_set_color(&object->base_object, sfWhite);
	lgameobject_subscribe(&object->base_object, sfEvtMouseButtonPressed);
	lgameobject_subscribe(&object->base_object, sfEvtMouseButtonReleased);
	lgameobject_subscribe(&object->base_object, sfEvtMouseMoved);
	sfRectangleShape_setSize(object->cursor,
				 vector2f(size.x - object->offset, size.y - object->offset));
	lgameobject_set_position(&object->base_object, vector2f(position.x + object->offset / 2,
								object->up + object->offset / 2));
	sfRectangleShape_setSize(object->background, vector2f(object->size.x, range));
	sfRectangleShape_setPosition(object->background, position);
	object->base_object.update = &update;
	object->base_object.catch_event = &catch_event;
	object->base_object.destroy = &lhnavbar_destroy;
	return ((lgameobject_t *)object);
}

float lhnavbar_get_scale(lgameobject_t *object)
{
	lhnavbar_t *navbar = (lhnavbar_t *)object;
	sfVector2f position = lgameobject_get_position(object);

	return ((position.y - navbar->offset / 2 - navbar->up) / (navbar->range - navbar->size.y));
}

void lhnavbar_set_range(lgameobject_t *object, float range)
{
	lhnavbar_t *navbar = (lhnavbar_t *)object;

	navbar->range = range;
	sfRectangleShape_setSize(navbar->background,
				 vector2f(sfRectangleShape_getSize(navbar->background).x, range));
}

void lhnavbar_set_color(lgameobject_t *object, sfColor color)
{
	lhnavbar_t *navbar = (lhnavbar_t *)object;

	navbar->color = color;
	color = color_div(color, 1.5);
	color.a = 100;
	sfRectangleShape_setFillColor(navbar->background, color);
}
