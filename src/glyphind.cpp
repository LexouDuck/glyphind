/*============================================================================*\
||                                            | 1 | .  '   '  .               ||
||  glyphind                                  | 2 | :  '. .'  : '' ..         ||
||                                            | 3 | :. :' ': .:               ||
||  https://github.com/LexouDuck/glyphind.git | 4 | ::                        ||
||                                            |   |                           ||
\*============================================================================*/

#include <libccc/sys/io.h>
#include <libccc/sys/time.h>
#include <libccc/random/random.h>

#include <glyphind.hpp>



s_glyphind glyphind;



int	main(int argc, char** argv)
{
	glyphind.rng = Random_New();
	Random_SetSeed(glyphind.rng, Time_Now());
	s_glyph_random query = (s_glyph_random)
	{
	//	basic metrics
		.grid_size_min       = (s_point){ .x=2, .y=2 },
		.grid_size_max       = (s_point){ .x=2, .y=2 },
		.stroke_amount       = (s_range<t_u8>){ .min=1, .max=9 },
		.allow_stroke_lines  = (t_bool)true,
		.allow_stroke_curves = (t_bool)false,
	//	found metrics
		.blockiness    = (s_range<t_float>){ .min=0., .max=0. },
		.complexity    = (s_range<t_float>){ .min=0., .max=0. },
		.symmetry      = (t_symmetry)0,
		.similar_to    = NULL,
		.similar_delta = (t_float)1.,
	};
	s_glyph glyph = query.GetRandom();
	IO_Output_Format("Glyph:"
		"\n""- version: %i"
		"\n""- grid_size: (%i,%i)"
		"\n""- stroke_amount: %i"
		"\n""- strokes:",
		glyph.version,
		glyph.grid_size.x,
		glyph.grid_size.y,
		glyph.stroke_amount);
	for (t_uint i = 0; i < glyph.stroke_amount; ++i)
	{
		IO_Output_Format("\n""\t""- %i:{ %i, (%i,%i) -> (%i,%i) }",
			i,
			glyph.strokes[i].type,
			glyph.strokes[i].start.x,
			glyph.strokes[i].start.y,
			glyph.strokes[i].end.x,
			glyph.strokes[i].end.y);
	}
	IO_Output_Format("\n");
}
