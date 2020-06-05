#include <stddef.h>
#include <unistd.h>
#include <SFML/System.h>

#include "LSCENE/lclock.h"

int lclock_create(lclock_t *clock, uint32_t frame_per_sec)
{
	clock->clock = sfClock_create();
	if (clock->clock == NULL)
		return (-1);
	clock->deltatime = 1.0 / frame_per_sec;
	clock->timescale = 1.0;
	clock->time = 0;
	clock->prevtime = 0;
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

double lclock_time(const lclock_t *clock)
{
	return (clock->time);
}

double lclock_rtime(lclock_t *clock)
{
	double rtime = sfTime_asSeconds(sfClock_getElapsedTime(clock->clock));

	clock->time += (rtime - clock->prevtime) * clock->timescale;
	clock->deltatime = rtime - clock->prevtime;
	clock->prevtime = rtime;
	return (rtime);
}

double lclock_delta_time(const lclock_t *clock)
{
	return (clock->deltatime * clock->timescale);
}

void lclock_timescale(lclock_t *clock, double scale)
{
	clock->timescale = scale;
}

uint32_t lclock_get_framerate(const lclock_t *clock)
{
	return (clock->fps);
}