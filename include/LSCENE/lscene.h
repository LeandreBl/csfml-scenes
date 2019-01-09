#ifndef LSCENES_H_
# define LSCENES_H_

# include <SFML/Graphics.h>
# include <LSCENE/lclock.h>
# include <lgtab.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/cdefs.h>

# include <LSCENE/lgameobject_types.h>

# define LSF_MAXIMUM_LAYERS (10)

typedef struct lsfasset_s {
  char *name;
  void *data;
  void (* destructor)(void *);
} lasset_t;

typedef struct lsf_scene_s
{
  sfRenderWindow *window;
  lclock_t clock;
  gtab_t objects;
  gtab_t to_add;
  gtab_t to_remove;
  gtab_t fonts;
  gtab_t images;
  gtab_t textures;
  gtab_t layered_objects[LSF_MAXIMUM_LAYERS];
  gtab_t subscribe_events[sfEvtCount + 1];
	bool running;
} lscene_t;

typedef struct lsfgameobject_s lgameobject_t;

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec) __THROW __nonnull((1, 2));
void lscene_run(lscene_t *scene) __THROW __nonnull((1));
void lscene_close(lscene_t *scene) __THROW __nonnull((1));
void lscene_destroy(lscene_t *scene) __THROW __nonnull((1));

int lscene_add_gameobject(lscene_t *scene, lgameobject_t *new_obj) __THROW __nonnull((1, 2));
int lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj) __THROW __nonnull((1, 2));

int lscene_get_objects_by_name(lscene_t *scene, gtab_t *empty_tab, const char *name) __THROW __nonnull((1, 2, 3));
int lscene_get_objects_by_tag(lscene_t *scene, gtab_t *empty_tab, int tag) __THROW __nonnull((1, 2));
int lscene_get_objects_by_type(lscene_t *scene, gtab_t *empty_tab, enum lgameobject_type type) __THROW __nonnull((1, 2));

double lscene_delta_time(lscene_t *scene) __THROW __nonnull((1));
double lscene_time(lscene_t *scene) __THROW __nonnull((1));
double lscene_rtime(lscene_t *scene) __THROW __nonnull((1));
void lscene_timescale(lscene_t *scene, float timescale) __THROW __nonnull((1));
void lscene_set_framerate(lscene_t *scene, uint32_t framerate) __THROW __nonnull((1));

const sfFont *lscene_get_font(lscene_t *scene, const char *name) __THROW __nonnull((1, 2));
const sfImage *lscene_get_image(lscene_t *scene, const char *name) __THROW __nonnull((1, 2));
const sfTexture *lscene_get_texture(lscene_t *scene, const char *name) __THROW __nonnull((1, 2));

#endif /* !LSCENES_H_ */
