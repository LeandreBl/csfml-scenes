#ifndef LBUTTON_H_
# define LBUTTON_H_

# include <stdbool.h>
# include <LSCENE/lgameobject.h>
# include <sys/cdefs.h>

typedef struct lsfbutton_s {
  lgameobject_t base_object;
  void (*tocall)(lgameobject_t *);
  sfVector2i at;
  bool moved;
  bool pressed;
} lbutton_t;

lgameobject_t *lbutton_create(sfVector2f position, const sfTexture *texture, void (*caller)(lgameobject_t *)) __THROW __nonnull((3));
void lbutton_set_texture(lgameobject_t *button, const sfTexture *texture) __THROW __nonnull((1, 2));

#endif /* !LBUTTON_H_ */