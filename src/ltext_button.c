#include <stdlib.h>

#include "LSCENE/lgameobject_types.h"
#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/ltext_button.h"

static void place_text(ltext_button_t *button)
{
    const sfFloatRect button_rect = sfSprite_getGlobalBounds(button->base_object.sprite);
    const sfFloatRect text_rect = sfText_getGlobalBounds(button->text);
    const sfVector2f position = {
        .x = (button_rect.width - text_rect.width) / 2,
        .y = (button_rect.height - text_rect.height) / 2,
    };
    sfText_setPosition(button->text, position);
}

static void update(lgameobject_t *obj)
{
	ltext_button_t *button = (ltext_button_t *)obj;
	sfFloatRect rect;

	rect = sfSprite_getGlobalBounds(obj->sprite);
	if (button->pressed) {
		button->pressed =
			sfFloatRect_contains(&rect, button->click_pos.x, button->click_pos.y);
		if (button->pressed) {
			sfSprite_setColor(obj->sprite, sfColor_fromRGB(110, 110, 110));
			button->tocall(button, button->data);
		}
		else
			button->moved = false;
	}
	else if (button->moved) {
		button->moved = sfFloatRect_contains(&rect, button->move_pos.x, button->move_pos.y);
		if (button->moved)
			sfSprite_setColor(obj->sprite, sfColor_fromRGB(170, 170, 170));
	}
	else
		sfSprite_setColor(obj->sprite, sfWhite);
    place_text(button);
}

static void catch_event(lgameobject_t *obj, const sfEvent *event)
{
	ltext_button_t *button = (ltext_button_t *)obj;

	if (event->type == sfEvtMouseMoved) {
		button->moved = true;
		button->move_pos = vector2f(event->mouseMove.x, event->mouseMove.y);
	}
	else if (event->type == sfEvtMouseButtonReleased) {
		button->pressed = false;
		button->click_pos = vector2f(event->mouseButton.x, event->mouseButton.y);
	}
	else if (event->type == sfEvtMouseButtonPressed) {
		button->pressed = true;
		button->click_pos = vector2f(event->mouseButton.x, event->mouseButton.y);
	}
}

lgameobject_t *ltext_button_create(const char *name, sfVector2f position, const sfTexture *texture,
			      void (*caller)(ltext_button_t *, void *data))
{
	ltext_button_t *button = calloc(1, sizeof(*button));

	if (button == NULL || lgameobject_create(&button->base_object, name) == -1)
		return (NULL);
    button->text = sfText_create();
    if (button->text == NULL)
        return NULL;
    sfText_setString(button->text, name);
	button->tocall = caller;
	lgameobject_set_position(&button->base_object, position);
	ltext_button_set_texture(&button->base_object, texture);
	lgameobject_subscribe(&button->base_object, sfEvtMouseButtonPressed);
	lgameobject_subscribe(&button->base_object, sfEvtMouseButtonReleased);
	lgameobject_subscribe(&button->base_object, sfEvtMouseMoved);
	button->base_object.type = LBUTTON_TYPE;
	button->base_object.catch_event = &catch_event;
	button->base_object.update = &update;
	return ((lgameobject_t *)button);
}

void ltext_button_set_texture(lgameobject_t *button, const sfTexture *texture)
{
	sfSprite_setTexture(button->sprite, texture, sfTrue);
}


void ltext_button_set_userdata(lgameobject_t *object, void *data)
{
    ltext_button_t *button = (typeof(button))object;

    button->data = data;
}

void ltext_button_set_text(lgameobject_t *object, const sfFont *font, sfColor color, unsigned int size)
{
    ltext_button_t *button = (typeof(button))object;

    sfText_setFont(button->text, font);
    sfText_setFillColor(button->text, color);
    sfText_setCharacterSize(button->text, size);
}
