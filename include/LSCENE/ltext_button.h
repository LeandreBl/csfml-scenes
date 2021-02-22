#ifndef LTEXT_BUTTON_H_
#define LTEXT_BUTTON_H_

#include <stdbool.h>
#include <sys/cdefs.h>
#include <LSCENE/lbutton.h>
#include <LSCENE/ltext.h>

lgameobject_t *ltext_button_create(const char *name, sfVector2f position, const sfTexture *texture, const sfFont *font,
			      void (*caller)(lbutton_t *, void *data), void *data) __THROW __nonnull((1, 3));
ltext_t *ltext_button_get_text(lgameobject_t *object) __THROW __nonnull((1));

#endif /* !LTEXT_BUTTON_H_ */