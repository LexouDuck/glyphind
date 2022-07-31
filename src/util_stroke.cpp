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



inline
s_point	stroke::Vector() const
{
	return ((s_point)
	{
		.x = this->end.x - this->start.x,
		.y = this->end.y - this->start.y,
	});
}

inline
t_float	stroke::Length() const
{
	s_point v = this->Vector();
	return (Float_SquareRoot((t_float)(v.x * v.x) + (t_float)(v.y * v.y)));
}

t_bool	stroke::IsValid(s_point const grid_size) const
{
	if (this->start.x >= grid_size.x)	return (FALSE);
	if (this->start.y >= grid_size.y)	return (FALSE);
	if (this->end.x   >= grid_size.x)	return (FALSE);
	if (this->end.y   >= grid_size.y)	return (FALSE);
	return (TRUE);
}

t_bool	stroke::IsWithin(s_stroke const* target) const
{
	if (this == target)
		return (TRUE);
	if (this->type == target->type &&
		this->type == STROKETYPE_LINE)
	{
		s_point vector_stroke = this->Vector();
		s_point vector_target = target->Vector();
		t_float ratio_stroke = (t_float)vector_stroke.x / (t_float)vector_stroke.y;
		t_float ratio_target = (t_float)vector_target.x / (t_float)vector_target.y;
		t_bool colinear = (ratio_stroke == ratio_target);
		if (colinear && (point::Equals(&this->start, &target->start) || point::Equals(&this->end, &target->end)))
		{
			return (TRUE);
		}
	}
	return (FALSE);
}

t_bool	stroke::Equals(s_stroke const* self, s_stroke const* other)
{
	if (point::Equals(&self->start, &other->start) && point::Equals(&self->end, &other->end))
	{
		return (self->type == other->type);
	}
	else if (point::Equals(&self->start, &other->end) && point::Equals(&self->end, &other->start))
	{
		return ((self->type == STROKETYPE_LINE    && other->type == STROKETYPE_LINE) ||
				(self->type == STROKETYPE_CURVE_H && other->type == STROKETYPE_CURVE_V) ||
				(self->type == STROKETYPE_CURVE_V && other->type == STROKETYPE_CURVE_H));
	}
	return (FALSE);
}
