#ifndef _LGAMEOBJECT_H_
# define _LGAMEOBJECT_H_

# include <stdint.h>
# include <SFML/Window/Event.h>
# include <SFML/Graphics/Sprite.h>
# include <lgtab.h>

# include "ltransform.h"

struct lsf_scene_s;

typedef struct lsfgameobject_s {
  struct lsfgameobject_s *parent;
  sfSprite *sprite;
  struct lsf_scene_s *scene;
  char *name;
  void *data;
  int tag;
  uint32_t layer;
  gtab_t childs;
  void (* start)(struct lsfgameobject_s *this);
  void (* update)(struct lsfgameobject_s *this);
  void (* catch_event)(struct lsfgameobject_s *this, const sfEvent *event);
  void (* destroy_data)(void *data);
} lgameobject_t;

lgameobject_t *lgameobject_create(const char *name, struct lsf_scene_s *scene, void *data);
void lgameobject_destroy(lgameobject_t *obj);
void lgameobject_start(lgameobject_t *obj);
void lgameobject_update(lgameobject_t *obj);
void lgameobject_display(lgameobject_t *obj);
void lgameobject_catch_event(lgameobject_t *obj, const sfEvent *event);
int lgameobject_add_child(lgameobject_t *obj, lgameobject_t *new_obj);
void lgameobject_set_position(lgameobject_t *obj, sfVector2f position);
sfVector2f lgameobject_get_position(lgameobject_t *obj);
void lgameobject_move(lgameobject_t *obj, sfVector2f offset);

#endif /* !_LGAMEOBJECT_H_ */
