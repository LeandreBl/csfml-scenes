#include <stdlib.h>
#include <lbl-libc.h>
#include <string.h>

#include "lscene.h"

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec)
{
	size_t count;
	size_t index = 0;
	const sfVideoMode *modes = sfVideoMode_getFullscreenModes(&count);

	for (size_t i = 0; i < count; ++i) {
		if (modes[i].height > modes[index].height)
			index = i;
	}
	scene->window = sfRenderWindow_create(modes[index], name, sfClose | sfFullscreen, NULL);
	scene->objects = new_gtab(20);
	scene->to_add = new_gtab(20);
	scene->to_remove = new_gtab(20);
	scene->clock = malloc(sizeof(*scene->clock));
	if (scene->window == NULL || scene->objects == NULL|| scene->clock == NULL
		|| scene->to_add == NULL || scene->to_remove == NULL
		|| lclock_create(scene->clock, frame_per_sec) == -1) {
		lscene_destroy(scene);
		return (-1);
	}
	for (size_t i = 0; i < LSF_MAXIMUM_LAYERS; ++i)
		if (gtab_create(&scene->layered_objects[i], 5) == -1) {
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
	gtab_destroy(scene->to_add, (void (*)(void *))lgameobject_destroy);
	gtab_destroy(scene->to_remove, (void (*)(void *))lgameobject_destroy);
	lclock_destroy(scene->clock);
	for (size_t i = 0; i < LSF_MAXIMUM_LAYERS; ++i) {
		gtab_destroy(&scene->layered_objects[i], NULL);
	}
	nfree(3, scene->objects, scene->clock, scene->to_add, scene->to_remove);
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
	if (gtab_append(scene->to_add, new) == -1)
		return (-1);
	return (0);
}

int lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj)
{
	if (gtab_append(scene->to_remove, obj) == -1)
		return (-1);
	return (0);
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

	for (size_t i = 0; i < scene->to_add->len; ++i) {
		go = scene->to_add->i[i];
		if (gtab_append(scene->objects, go) == -1 ||
			gtab_append(&scene->layered_objects[go->layer], go) == -1)
			return (-1);
		lgameobject_start(go);		
	}
	gtab_clear(scene->to_add, NULL);
	return (0);
}

static void delete_standby_objects(lscene_t *scene)
{
	lgameobject_t *go;

	for (size_t i = 0; i < scene->to_remove->len; ++i) {
		go = scene->to_remove->i[i];
		gtab_remove(scene->objects, go, (void (*)(void *))lgameobject_destroy);
		gtab_remove(&scene->layered_objects[go->layer], go, NULL);
	}
	gtab_clear(scene->to_remove, NULL);
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
		if (deploy_add_objects(scene) == -1)
			fprintf(stderr, "Error: Not enough memory, can't add new object\n");
		delete_standby_objects(scene);
		for (size_t i = 0; i < LSF_MAXIMUM_LAYERS; ++i)
			update_display_objects(&scene->layered_objects[i]);
		sfRenderWindow_display(scene->window);
		lclock_wait_delta(scene->clock);
	}
	sfRenderWindow_close(scene->window);
}