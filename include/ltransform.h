#ifndef _LTRANSFORM_H_
# define _LTRANSFORM_H_

# include <SFML/System.h>

typedef struct lsftransform_s {
	float rotation;
	sfVector2f position;
	sfVector2f movement;
} ltransform_t;

void ltransform_create(ltransform_t *transform, sfVector2f position, sfVector2f movement);
void ltransform_move(ltransform_t *transform, float coef);

#endif /* !_LTRANSFORM_H_ */