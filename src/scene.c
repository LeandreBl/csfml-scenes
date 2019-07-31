#include <stdlib.h>
#include <lbl-libc.h>
#include <string.h>

#include "LSCENE/lgameobject.h"
#include "LSCENE/lscene.h"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(*array))

static void asset_destroy(lasset_t *asset)
{
	free(asset->name);
	asset->destructor(asset->data);
}

static void lgameobject_ptr_destroy(lgameobject_t **go)
{
	lgameobject_destroy(*go);
}

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec)
{
	size_t count;
	const sfVideoMode *modes = sfVideoMode_getFullscreenModes(&count);

	scene->window = sfRenderWindow_create(modes[0], name, sfClose | sfFullscreen, NULL);
	if (scene->window == NULL) {
		sfRenderWindow_destroy(scene->window);
		return (-1);
	}
	lvector_create(scene->objects, 20, lgameobject_ptr_destroy);
	lvector_create(scene->to_remove, 20, NULL);
	lvector_create(scene->to_add, 20, NULL);
	lvector_create(scene->fonts, 5, asset_destroy);
	lvector_create(scene->images, 5, asset_destroy);
	lvector_create(scene->textures, 10, asset_destroy);
	if (scene->window == NULL || lclock_create(&scene->clock, frame_per_sec) == -1) {
		lscene_destroy(scene);
		return (-1);
	}
	for (size_t i = 0; i < ARRAY_LEN(scene->layered_objects); ++i)
		lvector_create(scene->layered_objects[i], 5, NULL);
	for (size_t i = 0; i < ARRAY_LEN(scene->subscribe_events); ++i)
		lvector_create(scene->subscribe_events[i], 5, NULL);
	sfRenderWindow_setFramerateLimit(scene->window, frame_per_sec + 1);
	return (0);
}

static int asset_create(lasset_t *asset, const char *name, void *data, void (*destructor)(void *))
{
	asset->name = strdup(name);
	if (asset->name == NULL)
		return -1;
	asset->destructor = destructor;
	asset->data = data;
	return 0;
}

void lscene_destroy(lscene_t *scene)
{
	sfRenderWindow_destroy(scene->window);
	lclock_destroy(&scene->clock);
	lvector_destroy(scene->objects);
	scene->to_add.destr = lgameobject_ptr_destroy;
	lvector_destroy(scene->to_add);
	lvector_destroy(scene->to_remove);
	for (size_t i = 0; i < ARRAY_LEN(scene->layered_objects); ++i)
		lvector_destroy(scene->layered_objects[i]);
	for (size_t i = 0; i < ARRAY_LEN(scene->subscribe_events); ++i)
		lvector_destroy(scene->subscribe_events[i]);
	lvector_destroy(scene->fonts);
	lvector_destroy(scene->images);
	lvector_destroy(scene->textures);
}

void lscene_add_gameobject(lscene_t *scene, lgameobject_t *new_obj)
{
	lvector_push_back(scene->to_add, new_obj);
}

void lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj)
{
	lvector_push_back(scene->to_remove, obj);
}

int lscene_get_objects_by_name(lscene_t *scene, gtab_t *empty_tab, const char *name)
{
	if (gtab_create(empty_tab, 10, NULL) == -1)
		return (-1);
	lvector_foreach(object,
			scene->objects) if (strcmp((*object)->name, name) == 0
					    && gtab_append(empty_tab, *object) == -1) return (-1);
	lvector_foreach(object,
			scene->to_add) if (strcmp((*object)->name, name) == 0
					   && gtab_append(empty_tab, *object) == -1) return (-1);
	return (0);
}

int lscene_get_objects_by_tag(lscene_t *scene, gtab_t *empty_tab, int tag)
{
	if (gtab_create(empty_tab, 10, NULL) == -1)
		return (-1);
	lvector_foreach(object,
			scene->objects) if ((*object)->tag == tag
					    && gtab_append(empty_tab, *object) == -1) return (-1);
	lvector_foreach(object,
			scene->to_add) if ((*object)->tag == tag
					   && gtab_append(empty_tab, *object) == -1) return (-1);
	return (0);
}

int lscene_get_objects_by_type(lscene_t *scene, gtab_t *empty_tab, enum lgameobject_type type)
{
	if (gtab_create(empty_tab, 10, NULL) == -1)
		return (-1);
	lvector_foreach(object,
			scene->objects) if ((*object)->type == type
					    && gtab_append(empty_tab, *object) == -1) return (-1);
	lvector_foreach(object,
			scene->to_add) if ((*object)->type == type
					   && gtab_append(empty_tab, *object) == -1) return (-1);
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

static void deploy_add_objects(lscene_t *scene)
{
	while (scene->to_add.len > 0) {
		scene->to_add.arr[0]->scene = scene;
		lvector_push_back(scene->objects, scene->to_add.arr[0]);
		lvector_push_back(scene->layered_objects[scene->to_add.arr[0]->layer],
				  scene->to_add.arr[0]);
		lvector_foreach(event, scene->to_add.arr[0]->subscribed_events)
			lvector_push_back(scene->subscribe_events[*event], scene->to_add.arr[0]);
		lgameobject_start(scene->to_add.arr[0]);
		lvector_erase(scene->to_add, 0);
	}
}

static void delete_standby_objects(lscene_t *scene)
{
	lvector_foreach(object, scene->to_remove)
	{
		lvector_erase_item(scene->layered_objects[(*object)->layer], *object);
		lvector_erase_item(scene->objects, *object);
	}
	lvector_clear(scene->to_remove);
}

static void call_subscribed_events(lscene_t *scene)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(scene->window, &event))
		lvector_foreach(object, scene->subscribe_events[event.type])
			lgameobject_catch_event(*object, &event);
}

void lscene_run(lscene_t *scene)
{
	scene->running = true;
	lclock_reset(&scene->clock);
	while (scene->running) {
		sfRenderWindow_clear(scene->window, sfBlack);
		call_subscribed_events(scene);
		deploy_add_objects(scene);
		delete_standby_objects(scene);
		for (size_t i = 0; i < LSF_MAXIMUM_LAYERS; ++i)
			lvector_foreach(object, scene->layered_objects[LSF_MAXIMUM_LAYERS - i - 1])
			{
				lgameobject_update(*object);
				lgameobject_display(*object);
			}
		sfRenderWindow_display(scene->window);
		lclock_rtime(&scene->clock);
	}
	sfRenderWindow_close(scene->window);
}

static lasset_t *find_asset(void *p, const char *name)
{
	lvector(lasset_t) *list = p;

	lvector_foreach(asset, *list)
	{
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
	lvector_emplace_back(scene->fonts, asset_create, name, font,
			     (void (*)(void *))sfFont_destroy);
	return (font);
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
	lvector_emplace_back(scene->images, asset_create, name, image,
			     (void (*)(void *))sfImage_destroy);
	return (image);
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
	lvector_emplace_back(scene->textures, asset_create, name, texture,
			     (void (*)(void *))sfTexture_destroy);
	return (texture);
}
