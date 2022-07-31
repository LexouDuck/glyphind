/*============================================================================*\
||                                            | 1 | .  '   '  .               ||
||  glyphind                                  | 2 | :  '. .'  : '' ..         ||
||                                            | 3 | :. :' ': .:               ||
||  https://github.com/LexouDuck/glyphind.git | 4 | ::                        ||
||                                            |   |                           ||
\*============================================================================*/

#include <libccc.h>
#include <libccc/memory.h>
#include <libccc/math/float.h>

#include <glyphind.hpp>



t_bool	point::Equals(s_point const* self, s_point const* other)
{
	return (
		self->x == other->x &&
		self->y == other->y);
}
