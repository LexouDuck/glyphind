/*============================================================================*\
||                                            | 1 | .  '   '  .               ||
||  glyphind                                  | 2 | :  '. .'  : '' ..         ||
||                                            | 3 | :. :' ': .:               ||
||  https://github.com/LexouDuck/glyphind.git | 4 | ::                        ||
||                                            |   |                           ||
\*============================================================================*/

#include <libccc.h>
#include <libccc/memory.h>
#include <libccc/random/random.h>
#include <libccc/math/float.h>
#include <libccc/sys/io.h>

#include <glyphind.hpp>



s_glyph	glyph_random::GetRandom() const
{
	s_point	grid_size = (s_point)
	{
		.x = (t_s8)Random_SInt_Range(glyphind.rng, this->grid_size_min.x, this->grid_size_max.x),
		.y = (t_s8)Random_SInt_Range(glyphind.rng, this->grid_size_min.y, this->grid_size_max.y),
	};
	t_u8	amount = Random_UInt_Range(glyphind.rng,
		this->stroke_amount.min,
		this->stroke_amount.max);
	s_stroke* strokes = (s_stroke*)Memory_Allocate(amount * sizeof(s_stroke));
	for (t_u8 i = 0; i < amount; ++i)
	{
		t_bool valid = FALSE;
		while (valid == FALSE)
		{
			e_stroke_type type = STROKETYPE_LINE;
			if (this->allow_stroke_curves)
			{
				type = (e_stroke_type)Random_SInt_Range(glyphind.rng, 0, ENUMLENGTH_STROKETYPE);
			}
			s_point p1 = (s_point)
			{
				.x = (t_s8)Random_SInt_Range(glyphind.rng, 0, this->grid_size_max.x),
				.y = (t_s8)Random_SInt_Range(glyphind.rng, 0, this->grid_size_max.y),
			};
			s_point p2 = (s_point)
			{
				.x = (t_s8)Random_SInt_Range(glyphind.rng, 0, this->grid_size_max.x),
				.y = (t_s8)Random_SInt_Range(glyphind.rng, 0, this->grid_size_max.y),
			};
			if (type == STROKETYPE_LINE || !this->allow_stroke_lines)
			{
				p2 = p1;
			}
			strokes[i] = (s_stroke)
			{
				.type  = type,
				.start = p1,
				.end   = p2,
			};
			if (!strokes[i].IsValid(grid_size))
				continue;
			for (t_sint j = i - 1; j >= 0; --j)
			{
//				if (strokes[i].IsWithin(&strokes[j]))
				if (stroke::Equals(&strokes[i], &strokes[j]))
					continue;
			}
			valid = TRUE;
		}
	}
	return (s_glyph)
	{
		.version = VERSION, // TODO better versioning logic ?
		.grid_size = grid_size,
		.stroke_amount = amount,
		.strokes = strokes,
	};
}

//s_glyph	glyph_random::GetPossibilities() const {}
