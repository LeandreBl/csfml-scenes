#include <stddef.h>
#include <unistd.h>
#include <SFML/System.h>

#include "lclock.h"

int lclock_create(lclock_t *clock, uint32_t frame_per_sec)
{
  clock->clock = sfClock_create();
  if (clock->clock == NULL)
    return (-1);
  lclock_framerate(clock, frame_per_sec);
  clock->timescale = 1.0;
  return (0);
}

void lclock_destroy(lclock_t *clock)
{
  sfClock_destroy(clock->clock);
}

void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec)
{
  clock->fps = frame_per_sec;
  clock->tick = sfSeconds(1.0 / (double)clock->fps);
  lclock_lap(clock);
}

void lclock_reset(lclock_t *clock)
{
  sfClock_restart(clock->clock);
}

double lclock_time(lclock_t *clock)
{
  return ((double)(sfTime_asSeconds(sfClock_getElapsedTime(clock->clock))));
}

double lclock_delta_time(lclock_t *clock)
{
  return (sfTime_asSeconds(clock->tick));
}

void lclock_lap(lclock_t *clock)
{
  clock->prevlap = sfClock_getElapsedTime(clock->clock);
}

void lclock_timescale(lclock_t *clock, float scale)
{
  clock->timescale = scale;
}

void lclock_wait_delta(lclock_t *clock)
{
  sfTime now = sfClock_getElapsedTime(clock->clock);
  sfTime elapsed = sfMicroseconds(now.microseconds - clock->prevlap.microseconds);

  if (now.microseconds * clock->timescale <= clock->tick.microseconds)
    sfSleep(sfMicroseconds(clock->tick.microseconds - elapsed.microseconds));
  clock->prevlap = sfMicroseconds(now.microseconds + clock->tick.microseconds - elapsed.microseconds);
}