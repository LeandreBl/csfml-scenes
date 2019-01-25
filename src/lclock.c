#include <stddef.h>
#include <unistd.h>
#include <SFML/System.h>

#include "LSCENE/lclock.h"

int lclock_create(lclock_t *clock, uint32_t frame_per_sec)
{
  clock->clock = sfClock_create();
  if (clock->clock == NULL)
    return (-1);
  lclock_framerate(clock, frame_per_sec);
  clock->timescale = 1.0;
  clock->time = 0;
  clock->prevtime = 0;
  clock->laps = 0;
  return (0);
}

void lclock_reset(lclock_t *clock)
{
  sfClock_restart(clock->clock);
  clock->time = 0;
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
}

double lclock_time(const lclock_t *clock)
{
  return (clock->time);
}

double lclock_rtime(lclock_t *clock)
{
  double rtime = sfTime_asSeconds(sfClock_getElapsedTime(clock->clock));

  clock->time += (rtime - clock->prevtime) * clock->timescale;
  clock->prevtime = rtime;
  ++clock->laps;
  if (rtime > clock->rfps + 1) {
    clock->fps = clock->laps;
    clock->laps = 0;
    ++clock->rfps;
  }
  return (rtime);
}

double lclock_delta_time(const lclock_t *clock)
{
  return (clock->tick * clock->timescale);
}

void lclock_timescale(lclock_t *clock, double scale)
{
  clock->timescale = scale;
}

uint32_t lclock_get_framerate(const lclock_t *clock)
{
  return (clock->fps);
}