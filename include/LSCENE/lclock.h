#ifndef LCLOCK_H_
# define LCLOCK_H_

# include <stdint.h>
# include <SFML/System/Clock.h>
# include <sys/cdefs.h>

typedef struct lsf_clock_s
{
  sfClock *clock;
  double prevlap;
  double tick;
  double timescale;
  double time;
  double prevtime;
  uint32_t fps;
} lclock_t;

int lclock_create(lclock_t *clock, uint32_t frame_per_sec) __THROW __nonnull((1));
void lclock_reset(lclock_t *clock) __THROW __nonnull((1));
void lclock_destroy(lclock_t *clock) __THROW __nonnull((1));

void lclock_timescale(lclock_t *clock, double scale) __THROW __nonnull((1));
void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec) __THROW __nonnull((1));
double lclock_time(lclock_t *clock) __THROW __nonnull((1));
double lclock_rtime(lclock_t *clock) __THROW __nonnull((1));
double lclock_delta_time(lclock_t *clock) __THROW __nonnull((1));
void lclock_lap(lclock_t *clock) __THROW __nonnull((1));
void lclock_wait_delta(lclock_t *clock) __THROW __nonnull((1));

#endif /* !LCLOCK_H_ */