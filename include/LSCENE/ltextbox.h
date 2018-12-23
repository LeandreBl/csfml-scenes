#ifndef LTEXT_BOX_H_
# define LTEXT_BOX_H_

# include <LSCENE/lscene.h>
# include <lstr.h>

typedef struct lsftextbox_s {
  sfText *render_text;
  const sfFont *font;
  sfColor color;
  float elapsed;
  lstr_t string;
  lstr_t placeholder;
  bool clicked;
  bool blink;
  bool selected;
} ltextbox_t;

void ltextbox_set_font(lgameobject_t *textbox, const sfFont *font);
void ltextbox_set_character_size(lgameobject_t *textbox, uint32_t size);
void ltextbox_set_color(lgameobject_t *textbox, sfColor color);
void ltextbox_set_position(lgameobject_t *textbox, sfVector2f position);
lgameobject_t *ltextbox_create(lscene_t *scene, sfVector2f position, const char *placeholder);

#endif /* !LTEXT_BOX_H_ */