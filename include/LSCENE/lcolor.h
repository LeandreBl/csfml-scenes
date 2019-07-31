#ifndef _LCOLOR_H_
#define _LCOLOR_H_

#include <SFML/Graphics/Color.h>
#include <stdint.h>

sfColor color_div(sfColor color, float coef);
sfColor color_mul(sfColor color, float coef);
sfColor color_add(sfColor color, uint8_t add);
sfColor color_sub(sfColor color, uint8_t sub);

#endif /* !_LCOLOR_H_ */
