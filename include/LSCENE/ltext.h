#ifndef LTEXT_H_
#define LTEXT_H_

#include <stdbool.h>
#include <sys/cdefs.h>
#include <LSCENE/lgameobject.h>

typedef struct lsftext_s {
	lgameobject_t base_object;
    sfText *text;
} ltext_t;

lgameobject_t *ltext_create(const char *name, sfVector2f position, const sfFont *font) __THROW __nonnull((1, 3));
sfText *ltext_get_text(lgameobject_t *object) __THROW __nonnull((1));

#endif /* !LTEXT_H_ */