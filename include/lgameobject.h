#ifndef _LGAMEOBJECT_H_
# define _LGAMEOBJECT_H_

# include <SFML/Window/Event.h>
# include <SFML/Graphics/Sprite.h>
# include <lgtab.h>

# include "ltransform.h"

struct lsf_scene_s;

typedef struct lsfgameoject_s {
	struct lsfgameobject_s *parent;
	sfSprite *sprite;
	struct lsf_scene_s *scene;
	char *name;
	void *data;
	gtab_t *childs;
	int tag;
	uint32_t layer;
	void (* start)(struct lsfgameoject_s *this);
	void (* update)(struct lsfgameoject_s *this);
	void (* catch_event)(struct lsfgameoject_s *this, const sfEvent *event);
	void (* destroy_data)(void *data);
} lgameobject_t;

lgameobject_t *lgameobject_create(const char *name, struct lsf_scene_s *scene, void *data);
void lgameobject_destroy(lgameobject_t *obj);
void lgameobject_start(lgameobject_t *obj);
void lgameobject_update(lgameobject_t *obj);
void lgameobject_display(lgameobject_t *obj);
void lgameobject_catch_event(lgameobject_t *obj, const sfEvent *event);


#endif /* !_LGAMEOBJECT_H_ */