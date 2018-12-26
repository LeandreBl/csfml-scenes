#include <stdlib.h>
#include <string.h>

#include "lscene.h"

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

int lgameobject_create(lgameobject_t *new_obj, const char *name)
{
  new_obj->name = strdup(name);
  if (new_obj->name == NULL || gtab_create(&new_obj->childs, 0) == -1)
    return (-1);
  new_obj->sprite = sfSprite_create();
  new_obj->tag = 0;
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
  lgameobject_t *child;

  sfSprite_destroy(obj->sprite);
  if (obj->destroy != NULL)
    obj->destroy(obj);
  for (size_t i = 0; i < obj->childs.len; ++i) {
    child = obj->childs.i[i];
    child->parent = NULL;
  }
  if (obj->parent != NULL)
    gtab_remove(&obj->parent->childs, obj, NULL);
  gtab_destroy(&obj->childs, NULL);
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

int lgameobject_add_child(lgameobject_t *obj, lgameobject_t *new_obj)
{
  if (lscene_add_gameobject(obj->scene, new_obj) == -1 ||
      gtab_append(&obj->childs, new_obj) == -1)
    return (-1);
  new_obj->parent = obj;
  return (0);
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