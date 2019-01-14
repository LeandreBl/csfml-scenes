#include <stdlib.h>

#include "LSCENE/lgameobject_types.h"
#include "LSCENE/ltextbox.h"

void ltextbox_set_font(lgameobject_t *textbox, const sfFont *font)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;

  obj->font = font;
  sfText_setFont(obj->render_text, font);
}

void ltextbox_set_character_size(lgameobject_t *textbox, uint32_t size)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;

  sfText_setCharacterSize(obj->render_text, size);
}

void ltextbox_set_color(lgameobject_t *textbox, sfColor color)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;

  obj->color = color;
  sfText_setFillColor(obj->render_text, color);
}

void ltextbox_set_position(lgameobject_t *textbox, sfVector2f position)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;

  lgameobject_set_position(textbox, position);
  sfText_setPosition(obj->render_text, position);
}

static void update(lgameobject_t *textbox)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;

  if (lscene_time(textbox->scene) - obj->elapsed >= 1.0) {
    obj->blink = !obj->blink;
    obj->elapsed = lscene_time(textbox->scene);
  }
  if (obj->string.len == 0 && !obj->selected) {
    sfText_setString(obj->render_text, obj->placeholder.i);
    sfText_setFillColor(obj->render_text,
                        sfColor_fromRGB(obj->color.r / 2, obj->color.g / 2, obj->color.b / 2));
  }
  else {
    ltextbox_set_color(textbox, obj->color);
    if (obj->blink && obj->selected)
      lstr_addch(&obj->string, '|');
    sfText_setString(obj->render_text, obj->string.i);
    if (obj->blink && obj->selected)
      lstr_erase(&obj->string, obj->string.len - 1, 1);
  }
  sfRenderWindow_drawText(textbox->scene->window, obj->render_text, NULL);
}

static void catch_event(lgameobject_t *textbox, const sfEvent *event)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;
  sfFloatRect rect = sfText_getGlobalBounds(obj->render_text);
  char value;

  if (event->type == sfEvtMouseButtonPressed) {
    if (sfFloatRect_contains(&rect, event->mouseButton.x, event->mouseButton.y))
      obj->clicked = true;
    else {
      obj->clicked = false;
      obj->selected = false;
    }
  }
  else if (event->type == sfEvtMouseButtonReleased
           && sfFloatRect_contains(&rect, event->mouseButton.x, event->mouseButton.y)
           && obj->clicked == true)
    obj->selected = true;
  if (obj->selected == true) {
    if (event->type == sfEvtKeyPressed) {
      if (obj->string.len > 0 && event->key.code == sfKeyBack)
        lstr_erase(&obj->string, obj->string.len - 1, 1);
      else if (event->key.code == sfKeyReturn) {
        obj->selected = false;
        obj->clicked = false;
      }
    }
    else if (event->type == sfEvtTextEntered) {
      value = event->text.unicode;
      if (value >= ' ' && value < 127)
        lstr_addch(&obj->string, value);
    }
  }
}

static void start(lgameobject_t *textbox)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;
  lasset_t *font_asset;

  if (obj->font == NULL) {
    if (textbox->scene->fonts.len != 0) {
      font_asset = textbox->scene->fonts.i[0];
      ltextbox_set_font(textbox, font_asset->data);
    }
    else {
      fprintf(stderr, "Textbox: error: not font set\n");
    }
  }
}

static void destroy(lgameobject_t *object)
{
  ltextbox_t *obj = (ltextbox_t *)object;

  lstr_destroy(&obj->string);
  lstr_destroy(&obj->placeholder);
  sfText_destroy(obj->render_text);
}

lgameobject_t *ltextbox_create(sfVector2f position, const char *placeholder,
                               uint32_t character_size, sfColor color)
{
  ltextbox_t *obj = calloc(1, sizeof(*obj));

  if (obj == NULL || lgameobject_create(&obj->base_object, "textbox") == -1)
    return (NULL);
  obj->render_text = sfText_create();
  if (obj->render_text == NULL)
    return (NULL);
  obj->color = sfBlack;
  if (lstr_create(&obj->string, "") == -1 || lstr_create(&obj->placeholder, placeholder) == -1)
    return (NULL);
  lgameobject_subscribe(&obj->base_object, sfEvtMouseMoved);
  lgameobject_subscribe(&obj->base_object, sfEvtMouseButtonPressed);
  lgameobject_subscribe(&obj->base_object, sfEvtMouseButtonReleased);
  lgameobject_subscribe(&obj->base_object, sfEvtKeyPressed);
  lgameobject_subscribe(&obj->base_object, sfEvtTextEntered);
  ltextbox_set_position((lgameobject_t *)obj, position);
  ltextbox_set_character_size((lgameobject_t *)obj, character_size);
  ltextbox_set_color((lgameobject_t *)obj, color);
  obj->base_object.type = LTEXTBOX_TYPE;
  obj->base_object.start = &start;
  obj->base_object.update = &update;
  obj->base_object.catch_event = &catch_event;
  obj->base_object.destroy = &destroy;
  return ((lgameobject_t *)obj);
}

const lstr_t *ltextbox_get_string(lgameobject_t *textbox)
{
  ltextbox_t *obj = (ltextbox_t *)textbox;

  return (&obj->string);
}