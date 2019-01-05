#ifndef _LTRANSFORM_H_
# define _LTRANSFORM_H_

# include <SFML/System.h>
# include <sys/cdefs.h>

typedef struct lsftransform_s {
	float rotation;
	sfVector2f position;
	sfVector2f movement;
} ltransform_t;

void ltransform_create(ltransform_t *transform, sfVector2f position, sfVector2f movement) __THROW __nonnull((1));
void ltransform_move(ltransform_t *transform, float coef) __THROW __nonnull((1));

#endif /* !_LTRANSFORM_H_ */