#include <stdlib.h>
#include <lbl-libc.h>
#include <string.h>

#include "LSCENE/lgameobject.h"
#include "LSCENE/lscene.h"

static void asset_destroy(lasset_t *asset)
{
  free(asset->name);
  asset->destructor(asset->data);
  free(asset);
}

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec)
{
  size_t count;
  const sfVideoMode *modes = sfVideoMode_getFullscreenModes(&count);

  scene->window = sfRenderWindow_create(modes[0], name, sfClose | sfFullscreen, NULL);
  if (scene->window == NULL
      || gtab_create(&scene->objects, 20, (void (*)(void *))lgameobject_destroy) == -1
      || gtab_create(&scene->to_remove, 20, NULL) == -1
      || gtab_create(&scene->to_add, 20, NULL) == -1
      || gtab_create(&scene->fonts, 5, (void (*)(void *))asset_destroy) == -1
      || gtab_create(&scene->images, 5, (void (*)(void *))asset_destroy) == -1
      || gtab_create(&scene->textures, 10, (void (*)(void *))asset_destroy) == -1) {
    sfRenderWindow_destroy(scene->window);
    return (-1);
  }
  if (scene->window == NULL
      || lclock_create(&scene->clock, frame_per_sec) == -1) {
    lscene_destroy(scene);
    return (-1);
  }
  for (size_t i = 0;
       i < sizeof(scene->layered_objects) / sizeof(*scene->layered_objects);
       ++i)
    if (gtab_create(&scene->layered_objects[i], 5, NULL) == -1) {
      lscene_destroy(scene);
      return (-1);
    }
  for (size_t i = 0;
       i < sizeof(scene->subscribe_events) / sizeof(*scene->subscribe_events);
       ++i) {
    if (gtab_create(&scene->subscribe_events[i], 5, NULL) == -1) {
      lscene_destroy(scene);
      return (-1);
    }
  }
  sfRenderWindow_setFramerateLimit(scene->window, frame_per_sec);
  return (0);
}

static lasset_t *asset_create(const char *name, void (*destructor)(void *))
{
  lasset_t *asset = malloc(sizeof(*asset));

  if (asset == NULL)
    return (NULL);
  asset->name = strdup(name);
  if (asset->name == NULL)
    return (NULL);
  asset->destructor = destructor;
  return (asset);
}

void lscene_destroy(lscene_t *scene)
{
  sfRenderWindow_destroy(scene->window);
  lclock_destroy(&scene->clock);
  gtab_destroy(&scene->objects);
  scene->to_add.destructor = (void (*)(void *))lgameobject_destroy;
  gtab_destroy(&scene->to_add);
  gtab_destroy(&scene->to_remove);
  for (size_t i = 0;
       i < sizeof(scene->layered_objects) / sizeof(*scene->layered_objects);
       ++i)
    gtab_destroy(&scene->layered_objects[i]);
  for (size_t i = 0;
       i < sizeof(scene->subscribe_events) / sizeof(*scene->subscribe_events);
       ++i)
    gtab_destroy(&scene->subscribe_events[i]);
  gtab_destroy(&scene->fonts);
  gtab_destroy(&scene->images);
  gtab_destroy(&scene->textures);
}

int lscene_add_gameobject(lscene_t *scene, lgameobject_t *new_obj)
{
  if (gtab_append(&scene->to_add, new_obj) == -1)
    return (-1);
  return (0);
}

int lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj)
{
  if (gtab_append(&scene->to_remove, obj) == -1)
    return (-1);
  return (0);
}

int lscene_get_objects_by_name(lscene_t *scene, gtab_t *empty_tab,
                               const char *name)
{
  lgameobject_t *obj;

  if (gtab_create(empty_tab, 10, NULL) == -1)
    return (-1);
  for (size_t i = 0; i < scene->objects.len; ++i) {
    obj = scene->objects.i[i];
    if (strcmp(obj->name, name) == 0 && gtab_append(empty_tab, obj) == -1)
      return (-1);
  }
  for (size_t i = 0; i < scene->to_add.len; ++i) {
    obj = scene->to_add.i[i];
    if (strcmp(obj->name, name) == 0 && gtab_append(empty_tab, obj) == -1)
      return (-1);
  }
  return (0);
}

int lscene_get_objects_by_tag(lscene_t *scene, gtab_t *empty_tab, int tag)
{
  lgameobject_t *obj;

  if (gtab_create(empty_tab, 10, NULL) == -1)
    return (-1);
  for (size_t i = 0; i < scene->objects.len; ++i) {
    obj = scene->objects.i[i];
    if (obj->tag == tag && gtab_append(empty_tab, obj) == -1)
      return (-1);
  }
  for (size_t i = 0; i < scene->to_add.len; ++i) {
    obj = scene->to_add.i[i];
    if (obj->tag == tag && gtab_append(empty_tab, obj) == -1)
      return (-1);
  }
  return (0);
}

int lscene_get_objects_by_type(lscene_t *scene, gtab_t *empty_tab,
                               enum lgameobject_type type)
{
  lgameobject_t *obj;

  if (gtab_create(empty_tab, 10, NULL) == -1)
    return (-1);
  for (size_t i = 0; i < scene->objects.len; ++i) {
    obj = scene->objects.i[i];
    if (obj->type == type && gtab_append(empty_tab, obj) == -1)
      return (-1);
  }
  for (size_t i = 0; i < scene->to_add.len; ++i) {
    obj = scene->to_add.i[i];
    if (obj->type == type && gtab_append(empty_tab, obj) == -1)
      return (-1);
  }
  return (0);
}

double lscene_delta_time(lscene_t *scene)
{
  return (lclock_delta_time(&scene->clock));
}

double lscene_time(lscene_t *scene)
{
  return (lclock_time(&scene->clock));
}

double lscene_rtime(lscene_t *scene)
{
  return (lclock_rtime(&scene->clock));
}

void lscene_timescale(lscene_t *scene, float timescale)
{
  lclock_timescale(&scene->clock, timescale);
}

void lscene_close(lscene_t *scene)
{
  scene->running = false;
}

void lscene_set_framerate(lscene_t *scene, uint32_t framerate)
{
  scene->clock.fps = framerate;
  sfRenderWindow_setFramerateLimit(scene->window, framerate * 2);
}

static void update_display_objects(gtab_t *vector)
{
  lgameobject_t *obj;

  for (size_t i = 0; i < vector->len; ++i) {
    obj = vector->i[i];
    lgameobject_update(obj);
    lgameobject_display(obj);
  }
}

static int deploy_add_objects(lscene_t *scene)
{
  lgameobject_t *go;
  sfEventType type;

  while (scene->to_add.len > 0) {
    go = scene->to_add.i[0];
    go->scene = scene;
    if (gtab_append(&scene->objects, go) == -1
        || gtab_append(&scene->layered_objects[go->layer], go) == -1)
      return (-1);
    for (size_t i = 0; i < go->subscribed_events.len; ++i) {
      type = (long)go->subscribed_events.i[i];
      if (gtab_append(&scene->subscribe_events[type], go) == -1)
        return (-1);
    }
    lgameobject_start(go);
    gtab_remove_at(&scene->to_add, 0);
  }
  return (0);
}

static void delete_standby_objects(lscene_t *scene)
{
  lgameobject_t *go;

  for (size_t i = 0; i < scene->to_remove.len; ++i) {
    go = scene->to_remove.i[i];
    gtab_remove(&scene->layered_objects[go->layer], go);
    gtab_remove(&scene->objects, go);
  }
  gtab_clear(&scene->to_remove);
}

static void call_subscribed_events(lscene_t *scene)
{
  sfEvent event;
  lgameobject_t *obj;

  while (sfRenderWindow_pollEvent(scene->window, &event)) {
    for (size_t i = 0; i < scene->subscribe_events[event.type].len; ++i) {
      obj = scene->subscribe_events[event.type].i[i];
      lgameobject_catch_event(obj, &event);
    }
  }
}

void lscene_run(lscene_t *scene)
{
  scene->running = true;
  lclock_reset(&scene->clock);
  while (scene->running) {
    sfRenderWindow_clear(scene->window, sfBlack);
    call_subscribed_events(scene);
    if (deploy_add_objects(scene) == -1)
      fprintf(stderr, "Error: Not enough memory, can't add new object\n");
    delete_standby_objects(scene);
    for (size_t i = 0; i < LSF_MAXIMUM_LAYERS; ++i)
      update_display_objects(
              &scene->layered_objects[LSF_MAXIMUM_LAYERS - i - 1]);
    sfRenderWindow_display(scene->window);
    lclock_rtime(&scene->clock);
  }
  sfRenderWindow_close(scene->window);
}

static lasset_t *find_asset(gtab_t *list, const char *name)
{
  lasset_t *asset;

  for (size_t i = 0; i < list->len; ++i) {
    asset = list->i[i];
    if (strcmp(asset->name, name) == 0)
      return (asset);
  }
  return (NULL);
}

const sfFont *lscene_get_font(lscene_t *scene, const char *name)
{
  lasset_t *asset = find_asset(&scene->fonts, name);
  sfFont *font;

  if (asset != NULL)
    return (asset->data);
  font = sfFont_createFromFile(name);
  if (font == NULL)
    return (NULL);
  asset = asset_create(name, (void (*)(void *))sfFont_destroy);
  if (asset == NULL)
    return (NULL);
  asset->data = font;
  gtab_append(&scene->fonts, asset);
  return (asset->data);
}

const sfImage *lscene_get_image(lscene_t *scene, const char *name)
{
  lasset_t *asset = find_asset(&scene->images, name);
  sfImage *image;

  if (asset != NULL)
    return (asset->data);
  image = sfImage_createFromFile(name);
  if (image == NULL)
    return (NULL);
  asset = asset_create(name, (void (*)(void *))sfImage_destroy);
  if (asset == NULL)
    return (NULL);
  asset->data = image;
  gtab_append(&scene->images, asset);
  return (asset->data);
}

const sfTexture *lscene_get_texture(lscene_t *scene, const char *name)
{
  lasset_t *asset = find_asset(&scene->textures, name);
  sfTexture *texture;

  if (asset != NULL)
    return (asset->data);
  texture = sfTexture_createFromFile(name, NULL);
  if (texture == NULL)
    return (NULL);
  asset = asset_create(name, (void (*)(void *))sfTexture_destroy);
  if (asset == NULL)
    return (NULL);
  asset->data = texture;
  gtab_append(&scene->textures, asset);
  return (asset->data);
}
