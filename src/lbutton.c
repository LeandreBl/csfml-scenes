#include <stdlib.h>

#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/lbutton.h"

static void update(lgameobject_t *obj)
{
  lbutton_t *data = obj->data;
  sfFloatRect rect;

  rect = sfSprite_getGlobalBounds(obj->sprite);
  if (data->pressed) {
    data->pressed = sfFloatRect_contains(&rect, data->at.x, data->at.y);
    if (data->pressed) {
      sfSprite_setColor(obj->sprite, sfColor_fromRGB(110, 110, 110));
      data->tocall(obj);
    }
    else
      data->moved = false;
  } else if (data->moved) {
    if (sfFloatRect_contains(&rect, data->at.x, data->at.y))
      sfSprite_setColor(obj->sprite, sfColor_fromRGB(170, 170, 170));
  } else
    sfSprite_setColor(obj->sprite, sfWhite);
}

static void catch_event(lgameobject_t *obj, const sfEvent *event)
{
  lbutton_t *data = obj->data;

  if (event->type == sfEvtMouseMoved) {
    data->moved = true;
    data->at = vector2i(event->mouseMove.x, event->mouseMove.y);
  }
  else if (event->type == sfEvtMouseButtonReleased) {
    data->pressed = false;
    data->at = vector2i(event->mouseButton.x, event->mouseButton.y);
  }
  else if (event->type == sfEvtMouseButtonPressed) {
    data->pressed = true;
    data->at = vector2i(event->mouseButton.x, event->mouseButton.y);
  }
}

lgameobject_t *lbutton_create(lscene_t *scene, sfVector2f position, const sfTexture *texture, void (*caller)(lgameobject_t *))
{
  lbutton_t *data = calloc(1, sizeof(*data));
  lgameobject_t *obj = lgameobject_create("button", scene, data);

  if (obj == NULL || data == NULL)
    return (NULL);
  data->tocall = caller;
  lgameobject_set_position(obj, position);
  lbutton_set_texture(obj, texture);
  obj->catch_event = &catch_event;
  obj->update = &update;
  obj->destroy_data = &free;
  return (obj);
}

void lbutton_set_texture(lgameobject_t *button, const sfTexture *texture)
{
  sfSprite_setTexture(button->sprite, texture, sfTrue);
}
