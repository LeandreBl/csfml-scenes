#include <stdlib.h>

#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/lbutton.h"

static void update(lgameobject_t *obj)
{
  lbutton_t *button = (lbutton_t *)obj;
  sfFloatRect rect;

  rect = sfSprite_getGlobalBounds(obj->sprite);
  if (button->pressed) {
    button->pressed = sfFloatRect_contains(&rect, button->at.x, button->at.y);
    if (button->pressed) {
      sfSprite_setColor(obj->sprite, sfColor_fromRGB(110, 110, 110));
      button->tocall(obj);
    }
    else
      button->moved = false;
  }
  else if (button->moved) {
    button->moved = sfFloatRect_contains(&rect, button->at.x, button->at.y);
    if (button->moved)
      sfSprite_setColor(obj->sprite, sfColor_fromRGB(170, 170, 170));
  }
  else
    sfSprite_setColor(obj->sprite, sfWhite);
}

static void catch_event(lgameobject_t *obj, const sfEvent *event)
{
  lbutton_t *button = (lbutton_t *)obj;

  if (event->type == sfEvtMouseMoved) {
    button->moved = true;
    button->at = vector2i(event->mouseMove.x, event->mouseMove.y);
  }
  else if (event->type == sfEvtMouseButtonReleased) {
    button->pressed = false;
    button->at = vector2i(event->mouseButton.x, event->mouseButton.y);
  }
  else if (event->type == sfEvtMouseButtonPressed) {
    button->pressed = true;
    button->at = vector2i(event->mouseButton.x, event->mouseButton.y);
  }
}

lgameobject_t *lbutton_create(sfVector2f position, const sfTexture *texture,
                              void (*caller)(lgameobject_t *))
{
  lbutton_t *button = calloc(1, sizeof(*button));

  if (button == NULL
      || lgameobject_create(&button->base_object, "button") == -1)
    return (NULL);
  button->tocall = caller;
  lgameobject_set_position(&button->base_object, position);
  lbutton_set_texture(&button->base_object, texture);
  button->base_object.type = LBUTTON_TYPE;
  button->base_object.catch_event = &catch_event;
  button->base_object.update = &update;
  return ((lgameobject_t *)button);
}

void lbutton_set_texture(lgameobject_t *button, const sfTexture *texture)
{
  sfSprite_setTexture(button->sprite, texture, sfTrue);
}
