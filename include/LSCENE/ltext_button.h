#ifndef LTEXT_BUTTON_H_
#define LTEXT_BUTTON_H_

#include <stdbool.h>
#include <sys/cdefs.h>
#include <LSCENE/lgameobject.h>

typedef struct lsftext_button_s {
	lgameobject_t base_object;
	void (*tocall)(struct lsftext_button_s *, void *data);
    void *data;
    sfText *text;
	sfVector2f click_pos;
	sfVector2f move_pos;
	bool moved;
	bool pressed;
} ltext_button_t;

lgameobject_t *ltext_button_create(const char *name, sfVector2f position, const sfTexture *texture,
			      void (*caller)(ltext_button_t *, void *data)) __THROW __nonnull((1, 3));
void ltext_button_set_userdata(lgameobject_t *button, void *data) __THROW __nonnull((1));
void ltext_button_set_text(lgameobject_t *object, const sfFont *font, sfColor color, unsigned int size);
void ltext_button_set_texture(lgameobject_t *button, const sfTexture *texture) __THROW __nonnull((1, 2));

#endif /* !LTEXT_BUTTON_H_ */