/*============================================================================*\
||                                            | 1 | .  '   '  .               ||
||  glyphind.hpp                              | 2 | :  '. .'  : '' ..         ||
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
#include <libccc/random/random.h>



// forward declarations
#define s_glyph   struct glyph
#define s_stroke  struct stroke



//! A range between two values
template<typename T>
#define s_range	\
struct range
struct range
{
	T  min;
	T  max;
};

//! A point coordinate vector on the glyph grid
#define s_point	\
struct point
struct point
{
	t_s8  x;
	t_s8  y;
//	methods
//	operators
	static t_bool	Equals(s_point const* self, s_point const* other);
};

typedef t_u8	t_symmetry;
#define SYMMETRY_POINT	(1 << 0)
#define SYMMETRY_AXIS_H	(1 << 1)
#define SYMMETRY_AXIS_V	(1 << 2)



#define GRID_SIZE_MIN	0
#define GRID_SIZE_MAX	128



#define e_stroke_type \
enum stroke_type
enum stroke_type
{
	STROKETYPE_LINE    = 0, //!< basic stroke, straight line
	STROKETYPE_CURVE_H = 1, //!< elliptic curved stroke that starts horizontally
	STROKETYPE_CURVE_V = 2, //!< elliptic curved stroke that starts vertically
//	STROKETYPE_SERIF   = 3, //!< line stroke that gets thinner, ends halfway
	ENUMLENGTH_STROKETYPE
};

#define s_stroke \
struct stroke
struct stroke
{
	e_stroke_type  type;
	s_point        start;
	s_point        end;
//	methods
	s_point	Vector() const;
	t_float	Length() const;
	t_bool	IsValid(s_point const grid_size) const;
	t_bool	IsWithin(s_stroke const* target) const;
//	operators
	static t_bool	Equals(s_stroke const* self, s_stroke const* other);
};

//#accessor (s_glyph glyph)[t_uint x, t_uint y]	Grid_Get(x, y)



//! Define a glyph made up a various strokes on a square grid of points, X×Y in size
#define s_glyph \
struct glyph
struct glyph
{
	t_u8       version;
	s_point    grid_size;
	t_s8       stroke_amount;
	s_stroke*  strokes;

//	methods
	t_uint	AddStroke(s_stroke const* stroke);
	s_glyph*	Simplify();

	t_symmetry	IsSymmetric() const;
	t_float	CalculateMinStrokes() const;
	t_float	CalculateBlockiness() const;
	t_float	CalculateSimplicity() const;
	t_float	CalculateSimilarity(s_glyph const* other) const;

	static s_glyph	Decode(t_u8* data);
	static t_u8*	Encode(s_glyph const* glyph);
};



#define s_glyph_random \
struct glyph_random
struct glyph_random
{
//	basic metrics
	s_point			grid_size_min;
	s_point			grid_size_max;
	s_range<t_u8>	stroke_amount;
	t_bool			allow_stroke_lines;
	t_bool			allow_stroke_curves;
//	found metrics
	s_range<t_float>	blockiness;
	s_range<t_float>	complexity;
	t_symmetry			symmetry;
	s_glyph				similar_to;
	t_float				similar_delta;
//	methods
	s_glyph	GetRandom() const;
	s_glyph	GetPossibilities() const;
};



//! Stores all internal application state
#define s_glyphind \
struct glyphind
struct glyphind
{
	t_prng*	rng;
};
//! Global variable which stores all application state
extern s_glyphind glyphind;
