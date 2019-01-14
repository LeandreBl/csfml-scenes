#include <string.h>

#include "LSCENE/ltransform.h"

void ltransform_create(ltransform_t *transform, sfVector2f position, sfVector2f movement)
{
  transform->position = position;
  transform->movement = movement;
}

void ltransform_move(ltransform_t *transform, float coef)
{
  transform->position.x += transform->movement.x * coef;
  transform->position.y += transform->movement.y * coef;
}
