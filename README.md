# csfml-scenes
A quick implementation of scenes gesture in C using CSFML

```c

#include <LSCENE/lscene.h>
#include <LSCENE/lvector.h>
#include <LSCENE/ltextbox.h>

static void exit_handler_start(struct lsfgameobject_s *self)
{
        /* Subscribe to the 'Closed' event (alt + F4, click on the window's [X] button) */
        lgameobject_subscribe(self, sfEvtClosed);

        /* Subscribe to the 'KeyPressed' event, triggered when a key is pressed */
        lgameobject_subscribe(self, sfEvtKeyPressed);
}

static void exit_handler_catch_event(struct lsfgameobject_s *self, const sfEvent *event)
{
        /* If the window is closed or if the ESCAPE key is pressed, stops the lscene_run() function */
        if (event->type == sfEvtClosed || (event->type == sfEvtKeyPressed && event->key.code == sfKeyEscape))
                lscene_close(self->scene);
}

int main(void)
{
        lscene_t scene;

        /* Create a position structure where x = 100 and y = 150 */
        sfVector2f position = vector2f(100, 150);

        /* 
        ** Create an input box gameobject at position 'position', with a placeholder that contains "I'm an input box",
        ** with a character size of 50 pixels with a white text color
        */
        lgameobject_t *input = linputbox_create(position, "I'm an input box", 50, sfWhite);

        /* Create a custom gameobject to handle exit events */
        lgameobject_t *exit_handler = lgameobject_empty("exit_handler");

        /* Set it's start function (called when the gameobject is added to the scene */
        exit_handler->start = &exit_handler_start;

        /* Set it's catch_event function (called each time a subscribed event is triggered) */
        exit_handler->catch_event = &exit_handler_catch_event;
        
        /* We now create a scene, the window's name will be "example", and the framerate is set to 60 (can change at runtime) */
        lscene_create(&scene, "example", 60);

        /* We get a font into the scene, 'commodore-64.ttf' it will load it into the scene if not already loaded */
        lscene_get_font(&scene, "commodore-64.ttf");

        /* We add both gameobjects */
        lscene_add_gameobject(&scene, input);
        lscene_add_gameobject(&scene, exit_handler);

        /* Keep in mind that, if not changed, the textbox will use the first scene loaded font as it's own */

        lscene_run(&scene);
        lscene_destroy(&scene);
        return 0;
}
```