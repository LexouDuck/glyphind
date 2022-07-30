/*============================================================================*\
||                                            | 1 | .  '   '  .               ||
||  glyphind.h                                | 2 | :  '. .'  : '' ..         ||
||                                            | 3 | :. :' ': .:               ||
||  https://github.com/LexouDuck/glyphind.git | 4 | ::                        ||
||                                            |   |                           ||
\*============================================================================*/

#define VERSION	1

/*!
	Here is an explanation of the format:

	<grid_size> <stroke_count> <strokes:[ { start:(x1,y1), end:(x2,y2), type:? }... ]>

	| field name     | size |
	+----------------+------+
	| total          | 2 + 6 * n |
	| grid_size      | 1 |
	| stroke_amount  | 1 |
	| stroke         | 6 * n |
	| stroke.type    | 2 |
	| stroke.start   | 2 * 1 |
	| stroke.start.x | 1 |
	| stroke.start.y | 1 |
	| stroke.end     | 2 * 1 |
	| stroke.end.x   | 1 |
	| stroke.end.y   | 1 |
*/

#include <libccc.h>
#include <libccc/memory.h>
#include <libccc/math/float.h>



//! A range between two values
template<typename T>
struct range
{
	T  min;
	T  max;
};
#define s_range	struct range

//! A point coordinate vector on the glyph grid
typedef struct point
{
	t_s8  x;
	t_s8  y;

	inline
	t_bool	operator == (s_point const p1, s_point const p2)
	{
		return (p1.x == p2.x && p1.y == p2.y);
	}
}	s_point;

typedef enum stroke_type
{
	STROKETYPE_LINE    = 0, //!< basic stroke, straight line
	STROKETYPE_CURVE_H = 1, //!< elliptic curved stroke that starts horizontally
	STROKETYPE_CURVE_V = 2, //!< elliptic curved stroke that starts vertically
//	STROKETYPE_SERIF   = 3, //!< line stroke that gets thinner, ends halfway
}	e_stroke_type;

typedef struct stroke	s_stroke;
struct stroke
{
	e_stroke_type  type;
	s_point        start;
	s_point        end;

	inline
	s_point	Vector(s_stroke const stroke)
	{
		return ((s_point)
		{
			.x = stroke.end.x - stroke.start.x,
			.y = stroke.end.y - stroke.start.y,
		});
	}

	inline
	t_float	Length(s_stroke const stroke)
	{
		s_point v = Stroke_Vector(stroke);
		return (Float_SquareRoot((t_float)(v.x * v.x) + (t_float)(v.y * v.y)));
	}

	t_bool	Stroke_IsValid(s_stroke const stroke, s_glyph const* glyph)
	{
		if (stroke.start.x >= glyph->grid_size.x)	return (FALSE);
		if (stroke.start.y >= glyph->grid_size.y)	return (FALSE);
		if (stroke.end.x   >= glyph->grid_size.x)	return (FALSE);
		if (stroke.end.y   >= glyph->grid_size.y)	return (FALSE);
		return (TRUE);
	}

	t_bool	IsWithin(s_stroke const stroke, s_stroke const target)
	{
		if (Stroke_Equals(stroke, target))
			return (TRUE);
		if (stroke.type == target.type &&
			stroke.type == STROKETYPE_LINE)
		{
			s_point vector_stroke = Stroke_Vector(stroke);
			s_point vector_target = Stroke_Vector(target);
			t_float ratio_stroke = (t_float)vector_stroke.x / (t_float)vector_stroke.y;
			t_float ratio_target = (t_float)vector_target.x / (t_float)vector_target.y;
			t_bool colinear = (ratio_stroke == ratio_target);
			if (colinear && (Point_Equals(stroke.start, target.start) || Point_Equals(stroke.end, target.end)))
			{
				return (TRUE);
			}
		}
		return (FALSE);
	}

	t_bool	operator == (s_stroke const s1, s_stroke const s2)
	{
		if (Point_Equals(s1.start, s2.start) && Point_Equals(s1.end, s2.end))
		{
			return (s1.type == s2.type);
		}
		else if (Point_Equals(s1.start, s2.end) && Point_Equals(s1.end, s2.start))
		{
			return ((s1.type == STROKETYPE_LINE    && s2.type == STROKETYPE_LINE) ||
					(s1.type == STROKETYPE_CURVE_H && s2.type == STROKETYPE_CURVE_V) ||
					(s1.type == STROKETYPE_CURVE_V && s2.type == STROKETYPE_CURVE_H));
		}
		return (FALSE);
	}
};



#define GRID_SIZE_MIN	0
#define GRID_SIZE_MAX	128

//! Define a glyph made up a various strokes on a square grid of points, X×Y in size
typedef struct glyph
{
	t_u8       version;
	s_point    grid_size;
	t_u8       stroke_amount;
	s_stroke*  strokes;
}	s_glyph;

//#accessor (s_glyph glyph)[t_uint x, t_uint y]	Grid_Get(x, y)

typedef t_u8	t_symmetry;
#define SYMMETRY_POINT	(1 << 0)
#define SYMMETRY_AXIS_H	(1 << 1)
#define SYMMETRY_AXIS_V	(1 << 2)

typedef struct glyph_random
{
//	basic metrics
	s_point			grid_minsize;
	s_point			grid_maxsize;
	s_range<t_u8>	stroke_amount;
	t_bool			allow_stroke_lines;
	t_bool			allow_stroke_curves;
//	found metrics
	s_range<t_float>	blockiness;
	s_range<t_float>	complexity;
	t_symmetry			symmetry;
	s_glyph				similar_to;
	t_float				similar_delta;
}	s_glyph_random;



t_uint	Glyph_AddStroke(s_glyph* glyph, s_stroke const stroke)
{
	if (!stroke.IsValid(glyph))
		goto failure;
	for (t_uint i = 0; i < glyph->stroke_amount; ++i)
	{
		if (Stroke_IsWithin(stroke, glyph->strokes[i]))
			goto failure;
	}
	glyph->stroke_amount += 1;
	glyph->strokes = (s_stroke*)Memory_Reallocate(glyph->strokes, glyph->stroke_amount);
failure:
	return (0);
}



static const t_uint sizeof_header = 4;
static const t_uint sizeof_stroke = 6;

s_glyph	Glyph_Decode(t_u8* data)
{
	s_glyph result;
	result.version       = data[0];
	result.grid_size.x   = data[1];
	result.grid_size.y   = data[2];
	result.stroke_amount = data[3];
	t_size offset = sizeof_header;
	result.strokes = (s_stroke*)Memory_Allocate(result.stroke_amount * sizeof(s_stroke));
	for (t_uint i = 0; i < result.stroke_amount; ++i)
	{
		result.strokes[i] = (s_stroke)
		{
			.type  = (e_stroke_type)data[offset + 1],
			.start = (s_point){ .x = data[offset + 2], .y = data[offset + 3] },
			.end   = (s_point){ .x = data[offset + 4], .y = data[offset + 5] },
		};
		offset += sizeof_stroke;
	}
	return (result);
}

t_u8*	Glyph_Encode(s_glyph const* glyph)
{
	t_u8* result;
	result = (t_u8*)Memory_Allocate(sizeof_header + sizeof_stroke * glyph->stroke_amount);
	result[0] = VERSION;
	result[1] = glyph->grid_size.x;
	result[2] = glyph->grid_size.y;
	result[3] = glyph->stroke_amount;
	t_size offset = sizeof_header;
	for (t_uint i = 0; i < glyph->stroke_amount; ++i)
	{
		result[offset + 1] = glyph->strokes[i].type;
		result[offset + 2] = glyph->strokes[i].start.x;
		result[offset + 3] = glyph->strokes[i].start.y;
		result[offset + 4] = glyph->strokes[i].end.x;
		result[offset + 5] = glyph->strokes[i].end.y;
		offset += sizeof_stroke;
	}
	return (result);
}



s_glyph	Glyph_GetPossibilities(s_glyph_random const* possibles);
s_glyph	Glyph_GetRandom(s_glyph_random const* possibles);
t_uint	Glyph_GetAmount(s_glyph_random const* possibles);
t_symmetry	Glyph_IsSymmetric(s_glyph glyph);
t_float	Glyph_GetBlockiness(s_glyph glyph);
t_float	Glyph_GetSimplicity(s_glyph glyph);
t_float	Glyph_GetSimilarity(s_glyph glyph, s_glyph other);


