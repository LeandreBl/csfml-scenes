#ifndef LBUTTON_H_
# define LBUTTON_H_

# include <stdbool.h>
# include <LSCENE/lgameobject.h>

typedef struct lsfbutton_s {
  lgameobject_t base_object;
  bool moved;
  bool pressed;
  sfVector2i at;
  void (*tocall)(lgameobject_t *);
} lbutton_t;

lgameobject_t *lbutton_create(sfVector2f position, const sfTexture *texture, void (*caller)(lgameobject_t *));
void lbutton_set_texture(lgameobject_t *button, const sfTexture *texture);

#endif /* !LBUTTON_H_ */