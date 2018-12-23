#ifndef LCLOCK_H_
# define LCLOCK_H_

# include <stdint.h>
# include <SFML/System/Clock.h>

typedef struct lsf_clock_s
{
  sfClock *clock;
  float prevlap;
  float tick;
  float timescale;
  uint32_t fps;
} lclock_t;

int lclock_create(lclock_t *clock, uint32_t frame_per_sec);
void lclock_destroy(lclock_t *clock);

void lclock_timescale(lclock_t *clock, float scale);
void lclock_framerate(lclock_t *clock, uint32_t frame_per_sec);
void lclock_reset(lclock_t *clock);
float lclock_time(lclock_t *clock);
float lclock_delta_time(lclock_t *clock);
void lclock_lap(lclock_t *clock);
void lclock_wait_delta(lclock_t *clock);

#endif /* !LCLOCK_H_ */