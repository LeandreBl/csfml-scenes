#include <stddef.h>
#include <unistd.h>

#include "lclock.h"

int lclock_create(lclock_t *clock, uint32_t frame_per_sec)
{
	clock->clock = sfClock_create();
	if (clock->clock == NULL)
		return (-1);
	lclock_framerate(clock, frame_per_sec);
	return (0);
}

void lclock_destroy(lclock_t *clock)
{
	sfClock_destroy(clock->clock);
}

void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec)
{
	clock->fps = frame_per_sec;
	clock->tick = (1.0 / (double)clock->fps);
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
	return ((double)clock->tick);
}

void lclock_lap(lclock_t *clock)
{
	clock->prevlap = lclock_time(clock);
}

void lclock_wait_delta(lclock_t *clock)
{
	double now = lclock_time(clock);
	double elapsed = now - clock->prevlap;

	if (elapsed <= clock->tick) {
		usleep((useconds_t)((clock->tick - elapsed) * 1000000));
	}
}