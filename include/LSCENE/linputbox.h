#ifndef LTEXT_BOX_H_
#define LTEXT_BOX_H_

#include <sys/cdefs.h>
#include <LSCENE/lscene.h>
#include <LSCENE/lgameobject.h>
#include <lstr.h>

typedef struct lsfinputbox_s {
	lgameobject_t base_object;
	sfText *render_text;
	const sfFont *font;
	sfColor color;
	float elapsed;
	lstr_t string;
	lstr_t placeholder;
	bool clicked;
	bool blink;
	bool selected;
} linputbox_t;

void linputbox_set_font(lgameobject_t *textbox, const sfFont *font) __THROW __nonnull((1, 2));
void linputbox_set_character_size(lgameobject_t *textbox, uint32_t size) __THROW __nonnull((1));
void linputbox_set_color(lgameobject_t *textbox, sfColor color) __THROW __nonnull((1));
lgameobject_t *linputbox_create(sfVector2f position, const char *placeholder,
			       uint32_t character_size, sfColor color) __THROW __nonnull((2));
const lstr_t *linputbox_get_string(lgameobject_t *textbox) __THROW __nonnull((1));

#endif /* !LTEXT_BOX_H_ */