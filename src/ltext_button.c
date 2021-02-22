#include <stdlib.h>

#include "LSCENE/lgameobject_types.h"
#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/ltext_button.h"
#include "LSCENE/ltext.h"

static void set(lbutton_t *button, ltext_t *text)
{
	const sfFloatRect button_rect = sfSprite_getGlobalBounds(button->base_object.sprite);
    const sfFloatRect text_rect = sfText_getLocalBounds(text->text);
	const sfVector2f offset = sfSprite_getPosition(button->base_object.sprite);
    const sfVector2f position = {
        .x = offset.x - text_rect.left / 2 + (button_rect.width - text_rect.width) / 2,
        .y = offset.y - text_rect.top / 2 + (button_rect.height - text_rect.height) / 2,
    };
    sfText_setPosition(text->text, position);
}

lgameobject_t *ltext_button_create(const char *name, sfVector2f position, const sfTexture *texture, const sfFont *font,
			      void (*caller)(lbutton_t *, void *data), void *data)
{
	lgameobject_t *button = lbutton_create(position, texture, caller, data);
	lgameobject_t *text;

	text = ltext_create(name, position, font);
	if (text == NULL)
		return NULL;
	lgameobject_add_child(button, text);
	button->type = LTEXT_BUTTON_TYPE;
	set((lbutton_t *)button, (ltext_t *)text);
	return ((lgameobject_t *)button);
}

ltext_t *ltext_button_get_text(lgameobject_t *object)
{
	ltext_t *button = (typeof(button))object;

	return (ltext_t *)button->base_object.childs.arr[0];
}