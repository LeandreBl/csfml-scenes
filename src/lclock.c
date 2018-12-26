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
  clock->prevlap = 0;
  clock->prevtime = 0;
  return (0);
}

void lclock_reset(lclock_t *clock)
{
  sfClock_restart(clock->clock);
  clock->time = 0;
  clock->prevlap = 0;
  clock->prevtime = 0;
}

void lclock_destroy(lclock_t *clock)
{
  sfClock_destroy(clock->clock);
}

void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec)
{
  clock->fps = frame_per_sec;
  clock->tick = 1.0 / (double)clock->fps;
  lclock_lap(clock);
}

double lclock_time(lclock_t *clock)
{
  return (clock->time);
}

double lclock_rtime(lclock_t *clock)
{
  double rtime = sfTime_asSeconds(sfClock_getElapsedTime(clock->clock));

  clock->time += (rtime - clock->prevtime) * clock->timescale;
  clock->prevtime = rtime;
  return (rtime);
}

double lclock_delta_time(lclock_t *clock)
{
  return (clock->tick * clock->timescale);
}

void lclock_lap(lclock_t *clock)
{
  clock->prevlap = lclock_rtime(clock);
}

void lclock_timescale(lclock_t *clock, double scale)
{
  clock->timescale = scale;
}

void lclock_wait_delta(lclock_t *clock)
{
  double now = lclock_rtime(clock);
  double elapsed = now - clock->prevlap;

  if (elapsed <= clock->tick)
    sfSleep(sfSeconds(clock->tick - elapsed));
  clock->prevlap = lclock_rtime(clock);
}