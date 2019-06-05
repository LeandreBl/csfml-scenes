#include <stdlib.h>
#include <string.h>

#include "LSCENE/lgameobject.h"
#include "LSCENE/lscene.h"

static void default_start(__attribute__((unused)) lgameobject_t *object)
{
}

static void default_update(__attribute__((unused)) lgameobject_t *object)
{
}

static void default_catch_event(__attribute__((unused)) lgameobject_t *object,
                                __attribute__((unused)) const sfEvent *event)
{
}

static void default_destroy(__attribute__((unused)) lgameobject_t *object)
{
}

lgameobject_t *lgameobject_empty(const char *name)
{
  lgameobject_t *new_obj = calloc(1, sizeof(*new_obj));

  if (new_obj == NULL || lgameobject_create(new_obj, name) == -1)
    return (NULL);
  return (new_obj);
}

int lgameobject_create(lgameobject_t *new_obj, const char *name)
{
  new_obj->name = strdup(name);
  new_obj->sprite = sfSprite_create();
  if (new_obj->name == NULL || new_obj->sprite == NULL)
    return (-1);
  lvector_create(new_obj->childs, 0, NULL);
  lvector_create(new_obj->subscribed_events, 0, NULL);
  new_obj->tag = 0;
  new_obj->type = LEMPTY_GAMEOBJECT_TYPE;
  new_obj->layer = 0;
  new_obj->parent = NULL;
  new_obj->start = &default_start;
  new_obj->update = &default_update;
  new_obj->catch_event = &default_catch_event;
  new_obj->destroy = &default_destroy;
  return (0);
}

void lgameobject_destroy(lgameobject_t *obj)
{
  sfSprite_destroy(obj->sprite);
  if (obj->destroy != NULL)
    obj->destroy(obj);
  lvector_foreach(object, obj->childs) {
    (*object)->parent = NULL;
  }
  if (obj->parent != NULL)
    lvector_erase_item(obj->parent->childs, obj);
  lvector_destroy(obj->childs);
  lvector_foreach(event, obj->subscribed_events) {
    lgameobject_unsubscribe(obj, *event);
  }
  lvector_destroy(obj->subscribed_events);
  free(obj->name);
  free(obj);
}

void lgameobject_start(lgameobject_t *obj)
{
  obj->start(obj);
}

void lgameobject_update(lgameobject_t *obj)
{
  obj->update(obj);
}

void lgameobject_catch_event(lgameobject_t *obj, const sfEvent *event)
{
  obj->catch_event(obj, event);
}

void lgameobject_display(lgameobject_t *obj)
{
  sfRenderWindow_drawSprite(obj->scene->window, obj->sprite, NULL);
}

void lgameobject_add_child(lgameobject_t *obj, lgameobject_t *new_obj)
{
  lscene_add_gameobject(obj->scene, new_obj);
  lvector_push_back(obj->childs, new_obj);
  new_obj->parent = obj;
}

void lgameobject_set_position(lgameobject_t *obj, sfVector2f position)
{
  sfSprite_setPosition(obj->sprite, position);
}

sfVector2f lgameobject_get_position(lgameobject_t *obj)
{
  return (sfSprite_getPosition(obj->sprite));
}

void lgameobject_move(lgameobject_t *obj, sfVector2f offset)
{
  sfSprite_move(obj->sprite, offset);
}

void lgameobject_subscribe(lgameobject_t *object, sfEventType type)
{
  lvector_push_back(object->subscribed_events, type);
}

void lgameobject_unsubscribe(lgameobject_t *object, sfEventType type)
{
  lvector_erase_item(object->subscribed_events, type);
  lvector_erase_item(object->scene->subscribe_events[type], object);
}