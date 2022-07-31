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



t_uint	glyph::AddStroke(s_stroke const* stroke)
{
	if (!stroke->IsValid(this->grid_size))
		goto failure;
	for (t_uint i = 0; i < this->stroke_amount; ++i)
	{
		if (stroke->IsWithin(&this->strokes[i]))
			goto failure;
	}
	this->stroke_amount += 1;
	this->strokes = (s_stroke*)Memory_Reallocate(this->strokes, this->stroke_amount);
failure:
	return (0);
}



static const t_uint sizeof_header = 4;
static const t_uint sizeof_stroke = 6;

s_glyph	glyph::Decode(t_u8* data)
{
	s_glyph result;
	result.version       = (t_u8)data[0];
	result.grid_size.x   = (t_s8)data[1];
	result.grid_size.y   = (t_s8)data[2];
	result.stroke_amount = (t_u8)data[3];
	t_size offset = sizeof_header;
	result.strokes = (s_stroke*)Memory_Allocate(result.stroke_amount * sizeof(s_stroke));
	for (t_u8 i = 0; i < result.stroke_amount; ++i)
	{
		result.strokes[i] = (s_stroke)
		{
			.type  = (e_stroke_type)data[offset + 1],
			.start = (s_point){ .x = (t_s8)data[offset + 2], .y = (t_s8)data[offset + 3] },
			.end   = (s_point){ .x = (t_s8)data[offset + 4], .y = (t_s8)data[offset + 5] },
		};
		offset += sizeof_stroke;
	}
	return (result);
}

t_u8*	glyph::Encode(s_glyph const* glyph)
{
	t_u8* result;
	result = (t_u8*)Memory_Allocate(sizeof_header + sizeof_stroke * glyph->stroke_amount);
	result[0] = (t_u8)VERSION;
	result[1] = (t_u8)glyph->grid_size.x;
	result[2] = (t_u8)glyph->grid_size.y;
	result[3] = (t_u8)glyph->stroke_amount;
	t_size offset = sizeof_header;
	for (t_u8 i = 0; i < glyph->stroke_amount; ++i)
	{
		result[offset + 1] = (t_u8)glyph->strokes[i].type;
		result[offset + 2] = (t_u8)glyph->strokes[i].start.x;
		result[offset + 3] = (t_u8)glyph->strokes[i].start.y;
		result[offset + 4] = (t_u8)glyph->strokes[i].end.x;
		result[offset + 5] = (t_u8)glyph->strokes[i].end.y;
		offset += sizeof_stroke;
	}
	return (result);
}



//t_symmetry	glyph::IsSymmetric() const;
//t_float	glyph::CalculateMinStrokes() const;
//t_float	glyph::CalculateBlockiness() const;
//t_float	glyph::CalculateSimplicity() const;
//t_float	glyph::CalculateSimilarity(s_glyph const* other) const;
