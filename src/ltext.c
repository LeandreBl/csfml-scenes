#include <stdlib.h>

#include "LSCENE/lgameobject_types.h"
#include "LSCENE/lvector.h"
#include "LSCENE/lscene.h"
#include "LSCENE/ltext.h"

static void update(lgameobject_t *object)
{
    ltext_t *go = (typeof(go))object;

    sfRenderWindow_drawText(object->scene->window, go->text, NULL);
}

static void destroy(lgameobject_t *object)
{
    ltext_t *go = (typeof(go))object;

    sfText_destroy(go->text);
}

lgameobject_t *ltext_create(const char *name, sfVector2f position, const sfFont *font)
{
    ltext_t *go = calloc(1, sizeof(*go));

    if (go == NULL || lgameobject_create(&go->base_object, name) == -1)
        return NULL;
    go->text = sfText_create();
    if (go->text == NULL)
        return NULL;
    sfText_setFont(go->text, font);
    sfText_setString(go->text, name);
    sfText_setPosition(go->text, position);
    sfText_setCharacterSize(go->text, 14);
    sfText_setFillColor(go->text, sfBlack);
    go->base_object.update = &update;
    go->base_object.destroy = &destroy;
    go->base_object.type = LTEXT_TYPE;
    return &go->base_object;
}

sfText *ltext_get_text(lgameobject_t *object)
{
    ltext_t *go = (typeof(go))object;

    return go->text;
}