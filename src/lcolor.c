#include "lcolor.h"

sfColor color_div(sfColor color, float coef)
{
  return (sfColor_fromRGB(color.r / coef, color.g / coef, color.b / coef));
}

sfColor color_mul(sfColor color, float coef)
{
  return (sfColor_fromRGB(color.r * coef, color.g * coef, color.b * coef));
}

sfColor color_add(sfColor color, uint8_t add)
{
  return (sfColor_fromRGB(color.r + add, color.g + add, color.b + add));
}

sfColor color_sub(sfColor color, uint8_t sub)
{
  return (sfColor_fromRGB(color.r - sub, color.g - sub, color.b - sub));
}
