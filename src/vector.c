#include "LSCENE/lvector.h"

sfVector2f vector2f(float x, float y)
{
	return ((sfVector2f){x, y});
}

sfVector2u vector2u(unsigned int x, unsigned int y)
{
	return ((sfVector2u){x, y});
}

sfVector2i vector2i(int x, int y)
{
	return ((sfVector2i){x, y});
}
