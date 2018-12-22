#include <stdlib.h>

#include "ltextbox.h"

void ltextbox_set_font(lgameobject_t *textbox, const sfFont *font)
{
  ltextbox_t *data = textbox->data;

  data->font = font;
  sfText_setFont(data->render_text, font);
}

void ltextbox_set_character_size(lgameobject_t *textbox, uint32_t size)
{
  ltextbox_t *data = textbox->data;

  sfText_setCharacterSize(data->render_text, size);
}

void ltextbox_set_color(lgameobject_t *textbox, sfColor color)
{
  ltextbox_t *data = textbox->data;

  data->color = color;
  sfText_setFillColor(data->render_text, color);
}

void ltextbox_set_position(lgameobject_t *textbox, sfVector2f position)
{
  ltextbox_t *data = textbox->data;

  textbox->transform.position = position;
  sfText_setPosition(data->render_text, position);
}

static void update(lgameobject_t *obj)
{
  ltextbox_t *data = obj->data;

  if (lscene_time(obj->scene) - data->elapsed >= 1.0) {
    data->blink = !data->blink;
    data->elapsed = lscene_time(obj->scene);
  }
  if (data->string.len == 0 && !data->selected) {
    sfText_setString(data->render_text, data->placeholder.i);
    sfText_setFillColor(data->render_text, sfColor_fromRGB(data->color.r / 2, data->color.g / 2, data->color.b / 2));
  }
  else {
    ltextbox_set_color(obj, data->color);
    if (data->blink && data->selected)
      lstr_addch(&data->string, '|');
    sfText_setString(data->render_text, data->string.i);
    if (data->blink && data->selected)
      lstr_erase(&data->string, data->string.len - 1, 1);
  }
  sfRenderWindow_drawText(obj->scene->window, data->render_text, NULL);
}

static void catch_event(lgameobject_t *obj, const sfEvent *event)
{
  ltextbox_t *data = obj->data;
  sfFloatRect rect = sfText_getGlobalBounds(data->render_text);
  char value;

  if (event->type == sfEvtMouseButtonPressed) {
    if (sfFloatRect_contains(&rect, event->mouseButton.x, event->mouseButton.y))
      data->clicked = true;
    else {
      data->clicked = false;
      data->selected = false;
    }
  }
  else if (event->type == sfEvtMouseButtonReleased
      && sfFloatRect_contains(&rect, event->mouseButton.x, event->mouseButton.y)
      && data->clicked == true)
    data->selected = true;
  if (data->selected == true) {
    if (event->type == sfEvtKeyPressed) {
      if (data->string.len > 0 && event->key.code == sfKeyBack)
        lstr_erase(&data->string, data->string.len - 1, 1);
      else if (event->key.code == sfKeyReturn) {
        data->selected = false;
        data->clicked = false;
      }
    }
    else if (event->type == sfEvtTextEntered) {
      value = event->text.unicode;
      if (value >= ' ' && value < 127)
        lstr_addch(&data->string, value);
    }
  }
}

static void start(lgameobject_t *obj)
{
  ltextbox_t *data = obj->data;

  if (data->font == NULL && obj->scene->fonts.len > 0)
    data->font = obj->scene->fonts.i[0];
  if (data->font == NULL)
    fprintf(stderr, "%s: error: no font set\n", obj->name);
  else {
    ltextbox_set_position(obj, obj->transform.position);
    ltextbox_set_color(obj, data->color);
    ltextbox_set_font(obj, data->font);
    ltextbox_set_character_size(obj, 20);
  }
}

static void destroy_data(void *obj_data)
{
  ltextbox_t *data = obj_data;

  lstr_destroy(&data->string);
  lstr_destroy(&data->placeholder);
  sfText_destroy(data->render_text);
  free(data);
}

lgameobject_t *ltextbox_create(lscene_t *scene, sfVector2f position, const char *placeholder)
{
  ltextbox_t *data = calloc(1, sizeof(*data));
  lgameobject_t *tb = lgameobject_create("textbox", scene, data);

  if (data == NULL || tb == NULL)
    return (NULL);
  tb->data = data;
  data->render_text = sfText_create();
  if (data->render_text == NULL)
    return (NULL);
  data->color = sfBlack;
  tb->transform.position = position;
  if (lstr_create(&data->string, "") == -1
      || lstr_create(&data->placeholder, placeholder) == -1)
    return (NULL);
  tb->start = &start;
  tb->update = &update;
  tb->catch_event = &catch_event;
  tb->destroy_data = &destroy_data;
  return (tb);
}