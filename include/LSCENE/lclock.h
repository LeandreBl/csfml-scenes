#ifndef LCLOCK_H_
# define LCLOCK_H_

# include <stdint.h>
# include <SFML/System/Clock.h>
# include <sys/cdefs.h>

typedef struct lsf_clock_s
{
  sfClock *clock;
  double tick;
  double timescale;
  double time;
  double prevtime;
  uint32_t fps;
  uint64_t laps;
  uint64_t rfps;
} lclock_t;

int lclock_create(lclock_t *clock, uint32_t frame_per_sec) __THROW __nonnull((1));
void lclock_reset(lclock_t *clock) __THROW __nonnull((1));
void lclock_destroy(lclock_t *clock) __THROW __nonnull((1));

void lclock_timescale(lclock_t *clock, double scale) __THROW __nonnull((1));
void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec) __THROW __nonnull((1));
uint32_t lclock_get_framerate(const lclock_t *clock) __THROW __nonnull((1));
double lclock_time(const lclock_t *clock) __THROW __nonnull((1));
double lclock_rtime(lclock_t *clock) __THROW __nonnull((1));
double lclock_delta_time(const lclock_t *clock) __THROW __nonnull((1));

#endif /* !LCLOCK_H_ */