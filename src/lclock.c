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
  clock->time = 0;
  return (0);
}

void lclock_destroy(lclock_t *clock)
{
  sfClock_destroy(clock->clock);
}

void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec)
{
  clock->fps = frame_per_sec;
  clock->tick = 1.0 / (float)clock->fps;
  lclock_lap(clock);
}

void lclock_reset(lclock_t *clock)
{
  sfClock_restart(clock->clock);
}

double lclock_time(lclock_t *clock)
{
  return (clock->time);
}

double lclock_rtime(lclock_t *clock)
{
  return (sfTime_asSeconds(sfClock_getElapsedTime(clock->clock)));
}

double lclock_delta_time(lclock_t *clock)
{
  return (clock->tick * clock->timescale);
}

void lclock_lap(lclock_t *clock)
{
  clock->prevlap = clock->time;
}

void lclock_timescale(lclock_t *clock, float scale)
{
  clock->timescale = scale;
  clock->tick = 1.0 / (float)clock->fps;
}

void lclock_wait_delta(lclock_t *clock)
{
  float now = lclock_rtime(clock);
  float elapsed = now - clock->prevlap;

  if (elapsed <= clock->tick)
    sfSleep(sfSeconds(clock->tick - elapsed));
  clock->prevlap = now + (clock->tick - elapsed);
  clock->time += clock->tick * clock->timescale;
}