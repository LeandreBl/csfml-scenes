#ifndef LSCENES_H_
# define LSCENES_H_

# include <SFML/Graphics.h>
# include <lclock.h>
# include <lgameobject.h>
# include <lgtab.h>
# include <stdbool.h>

typedef struct lsf_scene_s
{
	sfRenderWindow *window;
	gtab_t *objects;
	gtab_t *layered_objects;
	lclock_t *clock;
	bool running;
} lscene_t;

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec);
void lscene_destroy(lscene_t *scene);

int lscene_add_gameobject(lscene_t *scene, lgameobject_t *new);
void lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj);
void lscene_get_objects_by_name(lscene_t *scene, gtab_t *empty_tab, const char *name);
void lscene_get_objects_by_tag(lscene_t *scene, gtab_t *empty_tab, int tag);
double lscene_delta_time(lscene_t *scene);
double lscene_time(lscene_t *scene);
void lscene_close(lscene_t *scene);
void lscene_set_framerate(lscene_t *scene, uint32_t framerate);
void lscene_run(lscene_t *scene);

/* internal use */

gtab_t *new_gtab(size_t size);
void del_gtab(void *lbuffer);

#endif /* !LSCENES_H_ */
