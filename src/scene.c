#include <stdlib.h>
#include <lbl-libc.h>
#include <string.h>

#include "lscene.h"

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec)
{
	size_t count;
	size_t index = 0;
	const sfVideoMode *modes = sfVideoMode_getFullscreenModes(&count);

	for (size_t i = 0; i < count; ++i)
		if (modes[i].height > modes[index].height)
			index = i;
	scene->window = sfRenderWindow_create(modes[index], name, sfClose | sfFullscreen, NULL);
	scene->objects = new_gtab(20);
	scene->layered_objects = new_gtab(5);
	scene->clock = malloc(sizeof(*scene->clock));
	if (scene->window == NULL || scene->objects == NULL || scene->layered_objects == NULL
		|| scene->clock == NULL || lclock_create(scene->clock, frame_per_sec) == -1) {
		lscene_destroy(scene);
		return (-1);
	}
	for (size_t i = 0; i < scene->objects->len; ++i) {
		scene->objects->i[i] = new_gtab(4);
		if (scene->objects->i[i] == NULL) {
			lscene_destroy(scene);
			return (-1);
		}
	}
	return (0);
}

void lscene_destroy(lscene_t *scene)
{
	gtab_destroy(scene->objects, (void (*)(void *))lgameobject_destroy);
	gtab_destroy(scene->layered_objects, del_gtab);
	lclock_destroy(scene->clock);
	nfree(3, scene->objects, scene->layered_objects, scene->clock);
}

gtab_t *new_gtab(size_t size)
{
	gtab_t *new = malloc(sizeof(*new));

	if (new == NULL)
		return (NULL);
	if (gtab_create(new, size) == -1) {
		free(new);
		return (NULL);
	}
	return (new);
}

void del_gtab(void *lbuffer)
{
	gtab_destroy(lbuffer, NULL);
	free(lbuffer);
}

int lscene_add_gameobject(lscene_t *scene, lgameobject_t *new)
{
	uint32_t layer = new->layer;
	size_t prev_size;

	if (gtab_append(scene->objects, new) == -1)
		return (-1);
	lgameobject_start(new);
	if (scene->layered_objects->len <= layer) {
		prev_size = scene->layered_objects->len;
		if (gtab_resize(scene->layered_objects, layer + 1, del_gtab) == -1)
			return (-1);
		for (size_t i = prev_size; prev_size <= layer; ++i) {
			scene->layered_objects->i[i] = malloc(sizeof(*scene->layered_objects));
			if (scene->layered_objects->i[i] == NULL
				|| gtab_create(scene->layered_objects->i[i], 5) == -1) /* scene->layered_objects->i[layer] == NULL */
				return (-1);
		}
	}
	return (gtab_append(scene->layered_objects->i[layer], new));
}

void lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj)
{
	gtab_remove(scene->layered_objects->i[obj->layer], obj, NULL);
	gtab_remove(scene->objects, obj, (void (*)(void *))lgameobject_destroy);
}

void lscene_get_objects_by_name(lscene_t *scene, gtab_t *empty_tab, const char *name)
{
	lgameobject_t *obj;

	for (size_t i = 0; i < scene->objects->len; ++i) {
		obj = scene->objects->i[i];
		if (strcmp(obj->name, name) == 0)
			gtab_append(empty_tab, obj);
	}
}

void lscene_get_objects_by_tag(lscene_t *scene, gtab_t *empty_tab, int tag)
{
	lgameobject_t *obj;

	for (size_t i = 0; i < scene->objects->len; ++i) {
		obj = scene->objects->i[i];
		if (obj->tag == tag)
			gtab_append(empty_tab, obj);
	}
}

double lscene_delta_time(lscene_t *scene)
{
	return (lclock_delta_time(scene->clock));
}

double lscene_time(lscene_t *scene)
{
	return (lclock_time(scene->clock));
}

void lscene_close(lscene_t *scene)
{
	scene->running = false;
}

void lscene_set_framerate(lscene_t *scene, uint32_t framerate)
{
	scene->clock->fps = framerate;
	sfRenderWindow_setFramerateLimit(scene->window, framerate * 2);
}

static void update_display_objects(gtab_t *layer_objs)
{
	lgameobject_t *obj;

	for (size_t i = 0; layer_objs->len; ++i) {
		obj = layer_objs->i[i];
		lgameobject_update(obj);
		lgameobject_display(obj);
	}
}

void lscene_run(lscene_t *scene)
{
	sfEvent event;
	lgameobject_t *obj;

	scene->running = true;
	while (scene->running) {
		while (sfRenderWindow_pollEvent(scene->window, &event)) {
			for (size_t i = 0; i < scene->objects->len; ++i) {
				obj = scene->objects->i[i];
				lgameobject_catch_event(obj, &event);
			}
		}
		sfRenderWindow_clear(scene->window, sfBlack);
		for (size_t i = 0; i < scene->layered_objects->len; ++i)
			update_display_objects(scene->layered_objects->i[i]);
		sfRenderWindow_display(scene->window);
		lclock_wait_delta(scene->clock);
	}
	sfRenderWindow_close(scene->window);
}