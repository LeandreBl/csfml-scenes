#include <stdlib.h>

#include "LSCENE/lgameobject_types.h"
#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/lbutton.h"

static void update(lgameobject_t *obj)
{
  lbutton_t *button = (lbutton_t *)obj;
  sfFloatRect rect;

  rect = sfSprite_getGlobalBounds(obj->sprite);
  if (button->pressed) {
    button->pressed = sfFloatRect_contains(&rect, button->click_pos.x, button->click_pos.y);
    if (button->pressed) {
      sfSprite_setColor(obj->sprite, sfColor_fromRGB(110, 110, 110));
      button->tocall(button);
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
}

static void catch_event(lgameobject_t *obj, const sfEvent *event)
{
  lbutton_t *button = (lbutton_t *)obj;

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

lgameobject_t *lbutton_create(sfVector2f position, const sfTexture *texture,
                              void (*caller)(lbutton_t *))
{
  lbutton_t *button = calloc(1, sizeof(*button));

  if (button == NULL || lgameobject_create(&button->base_object, "button") == -1)
    return (NULL);
  button->tocall = caller;
  lgameobject_set_position(&button->base_object, position);
  lbutton_set_texture(&button->base_object, texture);
  lgameobject_subscribe(&button->base_object, sfEvtMouseButtonPressed);
  lgameobject_subscribe(&button->base_object, sfEvtMouseButtonReleased);
  lgameobject_subscribe(&button->base_object, sfEvtMouseMoved);
  button->base_object.type = LBUTTON_TYPE;
  button->base_object.catch_event = &catch_event;
  button->base_object.update = &update;
  return ((lgameobject_t *)button);
}

void lbutton_set_texture(lgameobject_t *button, const sfTexture *texture)
{
  sfSprite_setTexture(button->sprite, texture, sfTrue);
}
