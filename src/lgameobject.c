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

static void default_destroy_data(__attribute__((unused)) void *data)
{
}

lgameobject_t *lgameobject_create(const char *name, lscene_t *scene, void *data)
{
  lgameobject_t *new_obj = malloc(sizeof(*new_obj));

  if (new_obj == NULL)
    return (NULL);
  new_obj->name = strdup(name);
  if (new_obj->name == NULL || gtab_create(&new_obj->childs, 0) == -1)
    return (NULL);
  ltransform_create(&new_obj->transform, (sfVector2f){0, 0}, (sfVector2f){0, 0});
  new_obj->sprite = sfSprite_create();
  new_obj->scene = scene;
  new_obj->data = data;
  new_obj->tag = 0;
  new_obj->layer = 0;
  new_obj->parent = NULL;
  new_obj->start = &default_start;
  new_obj->update = &default_update;
  new_obj->catch_event = &default_catch_event;
  new_obj->destroy_data = &default_destroy_data;
  return (new_obj);
}

void lgameobject_destroy(lgameobject_t *obj)
{
  lgameobject_t *child;

  sfSprite_destroy(obj->sprite);
  if (obj->destroy_data != NULL)
    obj->destroy_data(obj->data);
  for (size_t i = 0; i < obj->childs.len; ++i) {
    child = obj->childs.i[i];
    child->parent = NULL;
  }
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
