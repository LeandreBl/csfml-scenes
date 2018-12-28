#ifndef LSCENES_H_
# define LSCENES_H_

# include <SFML/Graphics.h>
# include <LSCENE/lclock.h>
# include <LSCENE/lgameobject.h>
# include <lgtab.h>
# include <stdbool.h>
# include <stdio.h>

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
  gtab_t layered_objects[LSF_MAXIMUM_LAYERS];
  gtab_t fonts;
  gtab_t images;
  gtab_t textures;
	bool running;
} lscene_t;

int lscene_create(lscene_t *scene, const char *name, uint32_t frame_per_sec);
void lscene_run(lscene_t *scene);
void lscene_close(lscene_t *scene);
void lscene_destroy(lscene_t *scene);

int lscene_add_gameobject(lscene_t *scene, lgameobject_t *new_obj);
int lscene_del_gameobject(lscene_t *scene, lgameobject_t *obj);

int lscene_get_objects_by_name(lscene_t *scene, gtab_t *empty_tab, const char *name);
int lscene_get_objects_by_tag(lscene_t *scene, gtab_t *empty_tab, int tag);
int lscene_get_objects_by_type(lscene_t *scene, gtab_t *empty_tab, enum lgameobject_type type);

double lscene_delta_time(lscene_t *scene);
double lscene_time(lscene_t *scene);
double lscene_rtime(lscene_t *scene);
void lscene_timescale(lscene_t *scene, float timescale);
void lscene_set_framerate(lscene_t *scene, uint32_t framerate);

const sfFont *lscene_get_font(lscene_t *scene, const char *name);
const sfImage *lscene_get_image(lscene_t *scene, const char *name);
const sfTexture *lscene_get_texture(lscene_t *scene, const char *name);

#endif /* !LSCENES_H_ */
